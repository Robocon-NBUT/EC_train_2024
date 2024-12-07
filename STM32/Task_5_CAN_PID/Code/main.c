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

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Control mode enumeration
typedef enum {
    CONTROL_MODE_SPEED,   // 速度环控制模式
    CONTROL_MODE_POSITION // 位置环控制模式
} ControlMode_t;

#define MOTOR_COUNT 4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
// 全局变量
motor_measure_t motor_chassis[MOTOR_COUNT] = {0}; // 电机状态
PID_Controller position_pid_controllers[MOTOR_COUNT]; // 位置环PID
PID_Controller speed_pid_controllers[MOTOR_COUNT];    // 速度环PID
ControlMode_t control_mode = CONTROL_MODE_SPEED;      // 默认控制模式为速度环

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
        // 速度环参数
        speed_pid_controllers[i].Kp = 2.0f;
        speed_pid_controllers[i].Ki = 4.0f;
        speed_pid_controllers[i].Kd = 0.0f;
        speed_pid_controllers[i].setpoint = 1000.0f;
        speed_pid_controllers[i].integral = 0.0f;
        speed_pid_controllers[i].last_error = 0.0f;

        // 位置环参数
        position_pid_controllers[i].Kp = 1.0f;    // 提高Kp加快响应
        position_pid_controllers[i].Ki = 0.0f;    // 适当增加Ki消除稳态误差
        position_pid_controllers[i].Kd = 0.1f;    // 增加Kd提高阻尼
        position_pid_controllers[i].setpoint = 180.0f; // 默认目标位置（单位：角度）
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


// 模式切换检测函数

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FDCAN1_Init();
  MX_FDCAN2_Init();
  MX_TIM14_Init();
  MX_UART5_Init();

  /* USER CODE BEGIN 2 */
  PID_Init();    // 初始化PID参数
  CAN_Open(&hfdcan1); // 打开CAN总线

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      // 控制循环
      float dt = 0.02f; // 20ms间隔
      for (int i = 0; i < MOTOR_COUNT; i++) {
          if (control_mode == CONTROL_MODE_SPEED) {
              // 速度环控制
              float current_speed = (float)motor_chassis[i].speed_rpm;
              float pid_output = PID_Compute(&speed_pid_controllers[i], current_speed, dt);

              // 限幅
              if (pid_output > 10000.0f) pid_output = 10000.0f;
              if (pid_output < -10000.0f) pid_output = -10000.0f;

              motor_chassis[i].given_current = (int16_t)pid_output;
          } else if (control_mode == CONTROL_MODE_POSITION) {
              // 位置环控制
              float current_angle = (motor_chassis[i].ecd / 8192.0f) * 360.0f;
              float desired_speed = PID_Compute(&position_pid_controllers[i], current_angle, dt);

              float current_speed = (float)motor_chassis[i].speed_rpm;
              speed_pid_controllers[i].setpoint = desired_speed;
              float pid_output = PID_Compute(&speed_pid_controllers[i], current_speed, dt);

              // 限幅
              if (pid_output > 10000.0f) pid_output = 10000.0f;
              if (pid_output < -10000.0f) pid_output = -10000.0f;

              motor_chassis[i].given_current = (int16_t)pid_output;
          }
      }

      // 发送电流控制值到电机
      CAN_Send(&hfdcan1,
               motor_chassis[0].given_current,
               motor_chassis[1].given_current,
               motor_chassis[2].given_current,
               motor_chassis[3].given_current);

      HAL_Delay(20); // 20ms延迟
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
  __disable_irq();
  while (1)
  {
  }
}
