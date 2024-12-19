/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : 主程序入口
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * 保留所有权利。
  *
  * 本软件按LICENSE文件规定的条款进行许可（位于软件组件根目录）。
  * 若无LICENSE文件，则按AS-IS提供。
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "fdcan.h"
#include "memorymap.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "can_start.h"
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// 结构体定义已在其他文件进行，此处不再重复定义PID_Controller、motor_measure_t等结构体
// 假设已有：
// typedef struct { ... } motor_measure_t;
// typedef struct { ... } PID_Controller;
// 以及CAN_Message_ID定义等
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
typedef struct 
{ 
 uint16_t ecd; 
 int16_t speed_rpm; 
 int16_t given_current; 
 uint8_t temperate; 
 int16_t last_ecd; 
} motor_measure_t; 

/*CAN接收的ID*/
typedef enum
{
	//add by langgo
	CAN_2006Moto1_ID = 0x201,
	CAN_2006Moto2_ID = 0x202,
	CAN_2006Moto3_ID = 0x203,
	CAN_2006Moto4_ID = 0x204
	
}CAN_Message_ID;

#define get_motor_measure(ptr, data)\
 { \
 (ptr)->last_ecd = (ptr)->ecd; \
 (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]); \
 (ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]); \
 (ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]); \
 (ptr)->temperate = (data)[6]; \
 } 
typedef enum {
    CONTROL_MODE_SPEED,
    CONTROL_MODE_POSITION
} ControlMode_t;
/* 接收电机数据的宏定义（与外部结构、宏配合） */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

#define MOTOR_COUNT 4

motor_measure_t motor_chassis[MOTOR_COUNT] = {0};

PID_Controller position_pid_controllers[MOTOR_COUNT];
PID_Controller speed_pid_controllers[MOTOR_COUNT];

ControlMode_t control_mode = CONTROL_MODE_POSITION; // 初始为速度控制模式

// 函数声明


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// 初始化PID参数
void PID_Init(void) {
    for(int i = 0; i < MOTOR_COUNT; i++) {
        // 速度环参数，根据实际情况调节
        speed_pid_controllers[i].Kp = 2.0f;
        speed_pid_controllers[i].Ki = 4.0f;
        speed_pid_controllers[i].Kd = 0.0f;
        speed_pid_controllers[i].setpoint = 1000.0f;
        speed_pid_controllers[i].integral = 0.0f;
        speed_pid_controllers[i].last_error = 0.0f;

        // 位置环参数，根据实际情况调节
        // 假设目标位置为180度（可根据需求修改）
        position_pid_controllers[i].Kp = 1.0f;
        position_pid_controllers[i].Ki = 0.0f;
        position_pid_controllers[i].Kd = 0.1f;
        position_pid_controllers[i].setpoint = 100.0f;
        position_pid_controllers[i].integral = 0.0f;
        position_pid_controllers[i].last_error = 0.0f;
    }
}

// PID计算函数
float PID_Compute(PID_Controller *pid, float current_value, float dt) {
    float error = pid->setpoint - current_value;
    pid->integral += error * dt;
    float derivative = (error - pid->last_error) / dt;
    float output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;
    pid->last_error = error;
    return output;
}

/* USER CODE END 0 */

/**
  * @brief  程序主入口函数
  * @retval int
  */
int main(void)
{
  /* MPU 配置 */
  MPU_Config();

  /* HAL库初始化 */
  HAL_Init();

  /* 配置系统时钟 */
  SystemClock_Config();

  /* 初始化全部配置的外设 */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FDCAN1_Init();
  MX_FDCAN2_Init();
  MX_TIM14_Init();
  MX_UART5_Init();

  // 初始化PID控制器
  PID_Init();

  // 打开CAN总线（假设CAN_Open函数已在其他文件中定义）
  CAN_Open(&hfdcan1);

  while (1)
  {
      float dt = 0.05f; // 每次循环的时间间隔(单位：s)，假设50ms

    for(int i = 0; i < MOTOR_COUNT; i++) {
        if (control_mode == CONTROL_MODE_SPEED) {
            // 仅执行速度控制环
            float current_speed = (float)motor_chassis[i].speed_rpm;
            float pid_output = PID_Compute(&speed_pid_controllers[i], current_speed, dt);

            // 限幅
            if(pid_output > 10000.0f) pid_output = 10000.0f;
            if(pid_output < -10000.0f) pid_output = -10000.0f;

            motor_chassis[i].given_current = (int16_t)pid_output;
        }
        else if (control_mode == CONTROL_MODE_POSITION) {
            // 执行位置控制环
            float current_angle = (motor_chassis[i].ecd / 8192.0f) * 360.0f;
            float desired_speed = PID_Compute(&position_pid_controllers[i], current_angle, dt);

            float current_speed = (float)motor_chassis[i].speed_rpm;
            speed_pid_controllers[i].setpoint = desired_speed;
            float pid_output = PID_Compute(&speed_pid_controllers[i], current_speed, dt);

            // 限幅
            if(pid_output > 10000.0f) pid_output = 10000.0f;
            if(pid_output < -10000.0f) pid_output = -10000.0f;

            motor_chassis[i].given_current = (int16_t)pid_output;
        }
    }

    // 发送电流控制值到电机
    CAN_Send(&hfdcan1,
             motor_chassis[0].given_current,
             motor_chassis[1].given_current,
             motor_chassis[2].given_current,
             motor_chassis[3].given_current);

    HAL_Delay(50); // 50ms间隔
  }
}

/**
  * @brief 系统时钟配置
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** 使能LDO供电 */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** 配置主内部稳压输出电压 */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** 初始化RCC振荡器 */
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

  /** 初始化CPU, AHB和APB总线时钟 */
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

/* MPU配置函数 */
void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* 禁用MPU */
  HAL_MPU_Disable();

  /** 配置MPU区域保护 */
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

  /* 使能MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/* 错误处理函数 */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

/* FDCAN接收回调，用于接收电机编码器数据 */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != 0)
    {
        FDCAN_RxHeaderTypeDef rxHeader;
        uint8_t rxData[8];
        if(HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxHeader, rxData) == HAL_OK)
        {
            // 根据ID解析对应电机数据
            switch(rxHeader.Identifier)
            {
                case CAN_2006Moto1_ID:
                    get_motor_measure(&motor_chassis[0], rxData);
                    break;
                case CAN_2006Moto2_ID:
                    get_motor_measure(&motor_chassis[1], rxData);
                    break;
                case CAN_2006Moto3_ID:
                    get_motor_measure(&motor_chassis[2], rxData);
                    break;
                case CAN_2006Moto4_ID:
                    get_motor_measure(&motor_chassis[3], rxData);
                    break;
                default:
                    break;
            }
        }
    }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
