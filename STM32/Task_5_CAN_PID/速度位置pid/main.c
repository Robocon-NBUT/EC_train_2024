/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fdcan.h"
#include "memorymap.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */




/// @brief //////////////////////////////////////////////////////////////////////////////////

//这是在定义结构体
typedef struct {
    float kp;           // 比例系数
    float ki;           // 积分系数
    float kd;           // 微分系数
    float integral;     // 积分
    float previous_error; // 上次
    float max_output;     // 输出上限
    float min_output;     // 输出下限
    float max_integral;   // 积分项max
    float min_integral;   // 积分项min
    float deadzone;       // 死区范围
    float last_output;    // 上一次的控制输出
    float error;          // 当前误差
    float derivative;     // 当前微分值（未滤波）
    float alpha;          // 微分滤波系数aa
    float filtered_derivative; // 滤波后的微分
    float previous_output;//上一次
} PID;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CAN_Open(FDCAN_HandleTypeDef *can)
{
  FDCAN_FilterTypeDef filter;                    //<  can过滤器结构体
  filter.IdType = FDCAN_STANDARD_ID;             //< id设置为标准id
  filter.FilterIndex = 0;                        //< 筛选器的编号，标准id选择0-127
  filter.FilterType = FDCAN_FILTER_MASK;         //< 设置工作模式为掩码
  filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; //< 将经过过滤的数据存储
  filter.FilterID1 = 0x000;                      //< id
  filter.FilterID2 = 0x000;

  HAL_FDCAN_ConfigFilter(can, &filter); //< 配置过滤�?
  HAL_FDCAN_Start(can);
  HAL_FDCAN_ActivateNotification(can, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0); // 使能fifo0接收到新信息中断
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CAN函数，
void motor_speed(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{

  FDCAN_TxHeaderTypeDef txHeader;
  uint8_t txData[8];
  txHeader.Identifier = 0x200;
  txHeader.IdType = FDCAN_STANDARD_ID;
  txHeader.TxFrameType = FDCAN_DATA_FRAME;
  txHeader.DataLength = FDCAN_DLC_BYTES_8;
  txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  txHeader.BitRateSwitch = FDCAN_BRS_OFF;
  txHeader.FDFormat = FDCAN_CLASSIC_CAN;
  txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  txHeader.MessageMarker = 0x00;

  txData[0] = motor1 >> 8;
  txData[1] = motor1;
  txData[2] = motor2 >> 8;
  txData[3] = motor2;
  txData[4] = motor3 >> 8;
  txData[5] = motor3;
  txData[6] = motor4 >> 8;
  txData[7] = motor4;
  if ((hfdcan1.Instance->TXFQS & FDCAN_TXFQS_TFQF) == 0U)
  {
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &txHeader, txData) != HAL_OK)
    {
      Error_Handler();
    }
  }
}

void sendFloat(UART_HandleTypeDef *huart, float value, int precision)
{
  char buffer[50]; // 字符串缓冲区

  // 处理负数
  char *ptr = buffer;
  if (value < 0)
  {
    *ptr++ = '-';
    value = -value; // 转为正数处理
  }

  // 取整数部�?
  int intPart = (int)value;

  // 取小数部分，利用精度控制
  float fractionalPart = value - intPart;
  for (int i = 0; i < precision; i++)
  {
    fractionalPart *= 10;
  }
  int fractionalInt = (int)(fractionalPart + 0.5f); // 四舍五入

  // 将整数部分转换为字符?
  char temp[20]; // 临时存储整数部分
  int tempIndex = 0;
  if (intPart == 0)
  {
    temp[tempIndex++] = '0';
  }
  else
  {
    while (intPart > 0)
    {
      temp[tempIndex++] = (intPart % 10) + '0'; // 转为字符
      intPart /= 10;
    }
  }
  // 反转整数部分字符
  for (int i = tempIndex - 1; i >= 0; i--)
  {
    *ptr++ = temp[i];
  }

  // 添加小数
  if (precision > 0)
  {
    *ptr++ = '.';

    // 将小数部分转换为字符
    for (int i = precision - 1; i >= 0; i--)
    {
      ptr[i] = (fractionalInt % 10) + '0'; // 转为字符
      fractionalInt /= 10;
    }
    ptr += precision;
  }

  // 添加字符串结束符
  *ptr++ = ' '; // 添加空格
  *ptr = '\n';  // 字符串结束符

  // 通过串口发�?�字符串
  HAL_UART_Transmit(huart, (uint8_t *)buffer, ptr - buffer, HAL_MAX_DELAY);
}





/// @brief /////////////////////////////////////////////////////////////////////////////////////////////////////////
float pid_compute(PID *pid, float setpoint, float current_value, float dt)
{
  // 计算误差
  float error = setpoint - current_value;
  if (fabs(error) < pid->deadzone)
  {
    error = 0; // 误差在死区范围内，视�? 0
  }

  // 抗积分饱和�?�辑
  if (!(pid->previous_output >= pid->max_output && error > 0) && // 输出已到上限且误差为�?
      !(pid->previous_output <= pid->min_output && error < 0))   // 输出已到下限且误差为�?
  {
    pid->integral += error * dt;
    // 限制积分项范
    if (pid->integral > pid->max_integral)
      pid->integral = pid->max_integral;
    else if (pid->integral < pid->min_integral)
      pid->integral = pid->min_integral;
  }
  // 计算微分
  float derivative = (error - pid->previous_error) / dt;
  pid->filtered_derivative = pid->alpha * derivative + (1 - pid->alpha) * pid->filtered_derivative;
  // 计算输出
  float output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;

  // 限制输出范围
  if (output > pid->max_output)
    output = pid->max_output;
  else if (output < pid->min_output)
    output = pid->min_output;

  // 保存当前误差以备下次使用
  pid->previous_error = error;

  return output;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */









//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//pid计算和死区预防

float pid_compute(PID *pid, float setpoint, float current_value, float dt)
{
  // 计算误差
  float error = setpoint - current_value;
  if (fabs(error) < pid->deadzone)
  {
    error = 0; // 误差在死区范围内，视为 0
  }

  // 抗积分饱和逻辑
  if (!(pid->previous_output >= pid->max_output && error > 0) && // 输出已到上限且误差为正
      !(pid->previous_output <= pid->min_output && error < 0))   // 输出已到下限且误差为负
  {
    pid->integral += error * dt;
    // 限制积分项范围
    if (pid->integral > pid->max_integral)
      pid->integral = pid->max_integral;
    else if (pid->integral < pid->min_integral)
      pid->integral = pid->min_integral;
  }
  // 计算微分项
  float derivative = (error - pid->previous_error) / dt;
  pid->filtered_derivative = pid->alpha * derivative + (1 - pid->alpha) * pid->filtered_derivative;
  // 计算输出
  float output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;

  // 限制输出范围
  if (output > pid->max_output)
    output = pid->max_output;
  else if (output < pid->min_output)
    output = pid->min_output;

  // 保存当前误差以备下次使用
  pid->previous_error = error;

  return output;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//中断接收函数
PID pid,pid1;//定义两个结构体变量
float absolute_angle = 0.0;
float last_angle = 0.0;
float speed=0.0;
float angle = 0.0;
float setangle=40*360;//期望达到的角度
float rpm = 0.0;
float anglespeed=0.1;
float speeddt=0.001;
float targetspeed=0.0;
float max_speed = 1000.0;//限制的目标速度
typedef struct
{
  float angle;
  float rpm;  
} motor;
motor data;
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)//中断接收hfdcan回调
{
  FDCAN_RxHeaderTypeDef rxHeader;
  uint8_t rxData[8];//存放接收数据

  if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxHeader, rxData) == HAL_OK)
  {
    data.angle = ((rxData[0] << 8) | rxData[1]);//解码返回位置数据
    data.rpm = ((rxData[2] << 8) | rxData[3]);//解码返回转速数据
    angle = data.angle;
    angle = (angle / 8191) * 360;//将解码到的位置转为360°制
    float delta_angle = angle - last_angle;
    if (delta_angle > 180.0)
    {
      delta_angle -= 360.0;
    }
    else if (delta_angle < -180.0)
    {
      delta_angle += 360.0;
    }
    absolute_angle += delta_angle;
    last_angle = angle;                                                    // 计算得到角度的绝对角度值
    targetspeed = pid_compute(&pid, setangle, absolute_angle, anglespeed); // 对角度环进行pid计算
    speed = pid_compute(&pid1, targetspeed, data.rpm, speeddt);            // 计算速度pid
    motor_speed(targetspeed, 0, 0, 0);                                     // 输出函数
  }
}






//////////////////////////////////////////////////////////////////////////////////////////////////////////////






/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FDCAN1_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */
	CAN_Open(&hfdcan1);
	  PID_init(&pid,20.0, 0.003, 25.0,10000,-10000,900,-900,0.2,0.0,2000); //初始化位置pid
  PID_init(&pid1,14.0, 0.0003, 0.0061, 10000,-3000,30000,-30000,0.2,0.0,2000);//初始化速度pid
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

		   sendFloat(&huart4,speed,0);
       HAL_Delay(100);
   ///////////////////////////////////////////////////////////////////////////////////////////// 
      motor_speed(500,0,0,0);
			HAL_Delay(100);
/////////////////////////////////////////////////////////////////////////////////////////////////////////

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
