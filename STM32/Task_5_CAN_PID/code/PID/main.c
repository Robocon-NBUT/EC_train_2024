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
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <ANDmessage.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

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
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef struct {
    float SetPoint;      
    float Proportion;    
    float Integral;      
    float Derivative;    
    float LastError;     
    float PrevError;    
    float SumError;    
} PID;

float TargetPosition = 4000.0f;  
float TargetSpeed = 800.0f;      
float CurrentPosition = 0.0f;  
float CurrentSpeed = 0.0f;     

FDCAN_TxHeaderTypeDef TxHeader; 
FDCAN_RxHeaderTypeDef RxHeader;  
uint8_t TxData[8];  
uint8_t RxData[8];  

typedef uint16_t u16;   

float PIDCalc(PID *pp, float NextPoint) {
    float dError, Error;
    Error = pp->SetPoint - NextPoint;  
    pp->SumError += Error;  
    dError = pp->LastError - pp->PrevError;  
    pp->PrevError = pp->LastError;
    pp->LastError = Error;

    if (pp->SumError > 900) pp->SumError = 4000;
    else if (pp->SumError < -900) pp->SumError = -4000;

    return (pp->Proportion * Error  
            + pp->Integral * pp->SumError  
            + pp->Derivative * dError); 
}

float CascadePIDControl(int vSetPosition, int vSetSpeed, float encoderPosition, float encoderSpeed) {
    static PID PositionPID = {2.5, 0.1, 0.001, 0, 0, 0, 0}; 
    static PID SpeedPID = {0, 0.1, 0.001, 0.01, 0, 0, 0};  

    PositionPID.SetPoint = vSetPosition; 
    TargetSpeed = PIDCalc(&PositionPID, encoderPosition); 

    SpeedPID.SetPoint = vSetSpeed;  
    return PIDCalc(&SpeedPID, encoderSpeed);  
}

void FDCAN1_SendData(uint8_t* data) {
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, data) != HAL_OK) {
        Error_Handler();  
    }
}

void FDCAN1_ReceiveData(void) {
    if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {
        CurrentPosition = (float)(RxData[0] | (RxData[1] << 8));  
        CurrentSpeed = (float)(RxData[2] | (RxData[3] << 8));     
    }
}

void UART5_SendData(float position, float speed) {
    char buffer[100];
    sprintf(buffer, "Position: %.2f, Speed: %.2f\r\n", position, speed);
    HAL_UART_Transmit(&huart5, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);  
}

void CAN_Open(FDCAN_HandleTypeDef *can)
{
  FDCAN_FilterTypeDef filter;                   
  filter.IdType = FDCAN_STANDARD_ID;             
  filter.FilterIndex = 0;                        
  filter.FilterType = FDCAN_FILTER_MASK;         
  filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; 
  filter.FilterID1 = 0x200;                      
  filter.FilterID2 = 0x200; 

  HAL_FDCAN_ConfigFilter(can, &filter);                                  
  HAL_FDCAN_Start(can);                                          
  HAL_FDCAN_ActivateNotification(can, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0); 
}
void motor_speed(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{

  FDCAN_TxHeaderTypeDef txHeader;
  uint8_t txData[8];
  txHeader.Identifier =(uint32_t)0x200;
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

  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &txHeader, txData);

}
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
  MX_TIM14_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */
  ano_frame_init();
  CAN_Open(&hfdcan1);
	int d=0;
	/* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//		motor_speed(1000,0,0,0);
    FDCAN1_ReceiveData();
    float controlSignal = CascadePIDControl(TargetPosition, TargetSpeed, CurrentPosition, CurrentSpeed);
    TxData[0] = (uint8_t)(controlSignal);  
    FDCAN1_SendData(TxData);  
//    UART5_SendData(CurrentPosition, CurrentSpeed);
    ano_send_flexible_frame(0xF1,CurrentPosition, CurrentSpeed);    
    HAL_Delay(10);  
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
