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
#include "usart.h"
#include "gpio.h"
#include <math.h>
#include <stdio.h>


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#define MOTOR_MAX 10
#include "M2006.h"


#define PID_MAX_OUTPUT 100.0
#define PID_MIN_OUTPUT -100.0


PID pid;
float speed=0.0;

float setspeed=9000;

float angle = 0.0;


float speed1=0.0;
float dt=0.1;
typedef struct 
{
  uint16_t angle;
  uint16_t rpm;
  uint16_t elc;
}motor;
motor data;
/* USER CODE END Includes */
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


typedef struct
{
	float POS_GAOL;//????
	float POS_ABS;//????0
	float POS_OFFSET;
	float eer;
	float eer_eer;
}ANGLE_TypeDef;


typedef struct
{
	ANGLE_TypeDef ANGLE[8];
	PID   PID_SPEED[8];
	PID   PID_ANGLE[8];
	
}MOTOR_TypeDef;





typedef struct {
    float kp;    // ????
    float ki;    // ????
    float kd;    // ????
} PID_Params;




typedef struct {
    PID_Params params;
    float integral;  // ???
    float prev_error; // ??????,???????
} PID_Controller;




void PID_Init(PID_Controller *pid, float kp, float ki, float kd) {
    pid->params.kp = kp;
    pid->params.ki = ki;
    pid->params.kd = kd;
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
	
}


float pid_compute(PID *pid, float setpoint, float current_value, float dt)
{
  
  float error = setpoint - current_value;
	


  pid->integral += error * dt;
	
	
  if (pid->integral > pid->max_integral)
    pid->integral = pid->max_integral;
	
	
	
  else if (pid->integral < pid->min_integral)
    pid->integral = pid->min_integral;
	
  
  float derivative = (error - pid->previous_error) / dt;
  pid->filtered_derivative = pid->alpha * derivative + (1 - pid->alpha) * pid->filtered_derivative;
  
	
  float output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;

  
  if (output > pid->max_output)
    output = pid->max_output;
  else if (output < pid->min_output)
    output = pid->min_output;

 
  pid->previous_error = error;

  return output;
}




void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  FDCAN_RxHeaderTypeDef rxHeader;
  uint8_t rxData[8];
	
	Motor motor;
	
		motor.target_angle = 3600.0;
	  float dt = 0.001;
	  float error = motor.target_angle -  data.angle;
	
		
	
  if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxHeader, rxData) == HAL_OK)
  {
    
    data.angle = (uint16_t)((rxData[0] << 8) | rxData[1]);
		
		angle = data.angle;
    angle = (angle / 8191) * 360;
		
		
    data.rpm = (uint16_t)((rxData[2]<<8)|rxData[3]);
		
		
		
    speed=pid_compute(&pid,setspeed,data.rpm,dt);
		
		
		
		//speed1=PID_Update((PID_Controller *)&pid,setspeed,angle);
		
		//speed = pid_compute(&pid, setspeed, angle, dt);
		
		
		
		 
		motor_speed(speed,0,0,0);
		
  }
}  




































/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_DMA_Init();
  MX_UART4_Init();
  MX_FDCAN1_Init();
  /* USER CODE BEGIN 2 */
  CAN_Open(&hfdcan1);
	
  //PID_init(&pid, 5.0, 0.5, 0.3, 1384, 0);
	
	//PID_Init((PID_Controller *)&pid, 28.0, 0.13 ,0.7);
  PID_Init((PID_Controller *)&pid, 28.0, 0.13 ,0.7);

	Motor motor;
	
	
	
	
	

	
	
	
	
	
	
	
	
	
	
	
	
	
	 
	 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		
		
		
		

		
		
		
			
		
		
		
		
		
		
		
		

		
		
		HAL_Delay(10);
		
		//motor_speed(control_signal,200,200,200);
		
		
		
		
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
  RCC_OscInitStruct.PLL.PLLQ = 2;
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