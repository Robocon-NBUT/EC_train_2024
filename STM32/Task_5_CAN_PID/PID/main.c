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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "filter.h"
#include <math.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// ȫ�ֱ������ڴ洢��������Ϣ
int16_t motor2_current_position;  // ��������M2006����ĵ�ǰλ�ã�ʵ������ݾ����ȡ��ʽȷ��
int16_t motor2_last_position;
int16_t motor2_speed;

// ����ͨ��CAN������Ϣ�Ļ�����
uint8_t can_rx_buffer[8];
// ���ȣ�����һ��ȫ�ֱ������洢CAN�����жϵ�״̬
uint8_t can_rx_interrupt_enabled = 0;

// ����һ��������ʹ��CAN�����ж�
void Enable_CAN_Receive_Interrupt(FDCAN_HandleTypeDef* hfdcan) {
    // ����ж��Ƿ��Ѿ�ʹ�ܣ����û��ʹ�����������
    if (!can_rx_interrupt_enabled) {
        // ����CAN�����ж����ȼ�����������Ϊһ��ʾ��ֵ���ɸ������������
        HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 1, 0);

        // ʹ��CAN�����ж�
        HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);

        // �����ж�ʹ��״̬��־
        can_rx_interrupt_enabled = 1;
    }
}

void CAN_Receive(uint8_t* can_rx_buffer);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


// PID��ʼ������
void PID_Init(PID_t *pid) {
    pid->kp = 1.0;  // ����ϵ�����ɸ���ʵ���������
    pid->ki = 0.1;  // ����ϵ�����ɸ���ʵ���������
    pid->kd = 0.5;  // ΢��ϵ�����ɸ���ʵ���������
    pid->setpoint = 100;  // ����Ŀ��λ��Ϊ100���ɸ���ʵ�������޸�
    pid->integral = 0;
    pid->prev_error = 0;
}


// PID���º���
float PID_Update(PID_t *pid, float process_variable) {
    float error = pid->setpoint - process_variable;

    // ������
    float p_term = pid->kp * error;

    // ������
    pid->integral += error;
    float i_term = pid->ki * pid->integral;

    // ΢����
    float derivative = error - pid->prev_error;
    float d_term = pid->kd * derivative;

    // ����PID���
    float output = p_term + i_term + d_term;

    // �������ת�����Ƶ������
    if (output > MAX_RPM) {
        output = MAX_RPM;
    } else if (output < -MAX_RPM) {
        output = -MAX_RPM;
    }

    pid->prev_error = error;

    return output;
}

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */



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
	 // MPU����
    MPU_Config();

    // MCU����
    HAL_Init();

    // ����ϵͳʱ��
    SystemClock_Config();

    // ��ʼ���������õ�����
    MX_GPIO_Init();
    MX_FDCAN1_Init();
    MX_USART3_UART_Init();
    MX_TIM14_Init();

    // ��ʼ��PID����
    PID_t pid;
    PID_Init(&pid);

    // ��CANͨ�Ų�������ع�������
    CAN_Open(&hfdcan1);

    // ʹ��CAN�����ж�
    Enable_CAN_Receive_Interrupt(&hfdcan1);
    // ��ʼ�������һ��λ��
    motor2_last_position = 0;

    while (1) {
        // ������2���ٶȣ������ͨ��λ�ò��ʱ�������㣬ֻʵ�ʿ�����Ҫ����ȷ�ķ�����
        motor2_speed = motor2_current_position - motor2_last_position;

        // ���ٶ�ת��ΪRPM�����������ʱ������1�룬����ʵ�����������
        float motor2_rpm = (float)motor2_speed * 60;

        // Ӧ��PID�����㷨���������
        float control_output = PID_Update(&pid, motor2_current_position);

        // ��������ת��Ϊ�ʺϷ��͸�����ĸ�ʽ��������轫RPMת��Ϊ�ض�������λ��
        int16_t motor_control_value = RPM_TO_CONTROL_UNIT((int16_t)control_output);

        // ���Ϳ����������
        CAN_Send(&hfdcan1, 0, motor_control_value, 0, 0);

        // ������һ�εĵ��λ��
        motor2_last_position = motor2_current_position;

        HAL_Delay(1000);
    }

    return 0;

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
  MX_USART3_UART_Init();
  MX_TIM14_Init();
  /* USER CODE BEGIN 2 */
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		CAN_Send(&hfdcan1,800,800,800,800);
    HAL_Delay(1000);
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
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 120;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
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
