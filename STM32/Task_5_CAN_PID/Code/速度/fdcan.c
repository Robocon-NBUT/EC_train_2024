/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    fdcan.c
  * @brief   This file provides code for the configuration
  *          of the FDCAN instances.
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
#include "fdcan.h"

/* USER CODE BEGIN 0 */
#include "pid.h"
#include "oled.h"
#include "font.h"
#include "stdio.h"
#include "tim.h" 
/* USER CODE END 0 */

FDCAN_HandleTypeDef hfdcan1;
pid_t pid_motor[4];  // 假设朿多控刿4个电机的PID结构体数绿
float set_speed[4];// 对应4个电机的速度设定值数绿
extern volatile float speed1;
/* FDCAN1 init function */
void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = DISABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 1;
  hfdcan1.Init.NominalSyncJumpWidth = 10;
  hfdcan1.Init.NominalTimeSeg1 = 34;
  hfdcan1.Init.NominalTimeSeg2 = 15;
  hfdcan1.Init.DataPrescaler = 3;
  hfdcan1.Init.DataSyncJumpWidth = 10;
  hfdcan1.Init.DataTimeSeg1 = 29;
  hfdcan1.Init.DataTimeSeg2 = 10;
  hfdcan1.Init.MessageRAMOffset = 0;
  hfdcan1.Init.StdFiltersNbr = 1;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.RxFifo0ElmtsNbr = 4;
  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxFifo1ElmtsNbr = 0;
  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxBuffersNbr = 0;
  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.TxEventsNbr = 0;
  hfdcan1.Init.TxBuffersNbr = 0;
  hfdcan1.Init.TxFifoQueueElmtsNbr = 4;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */
	
	 if (HAL_FDCAN_Init(&hfdcan1)!= HAL_OK)
    {
        
    }
    else
    {
        
        FDCAN_FilterTypeDef filter;
        filter.IdType = FDCAN_STANDARD_ID;
        filter.FilterIndex = 0;
        filter.FilterType = FDCAN_FILTER_MASK;
        filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
        filter.FilterID1 = 0x000;  
        filter.FilterID2 = 0x000;
        HAL_FDCAN_ConfigFilter(&hfdcan1, &filter);
			
    }
		
		
	    
  /* USER CODE END FDCAN1_Init 2 */

}

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspInit 0 */

  /* USER CODE END FDCAN1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* FDCAN1 clock enable */
    __HAL_RCC_FDCAN_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**FDCAN1 GPIO Configuration
    PA11     ------> FDCAN1_RX
    PA12     ------> FDCAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* FDCAN1 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
    HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
  /* USER CODE BEGIN FDCAN1_MspInit 1 */

  /* USER CODE END FDCAN1_MspInit 1 */
  }
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspDeInit 0 */

  /* USER CODE END FDCAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_FDCAN_CLK_DISABLE();

    /**FDCAN1 GPIO Configuration
    PA11     ------> FDCAN1_RX
    PA12     ------> FDCAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* FDCAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
    HAL_NVIC_DisableIRQ(FDCAN1_IT1_IRQn);
  /* USER CODE BEGIN FDCAN1_MspDeInit 1 */

  /* USER CODE END FDCAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void CAN_Open(FDCAN_HandleTypeDef* can) 
{ 
    FDCAN_FilterTypeDef filter;                   	//< 声明屿部变釿 can过滤器结构体
	filter.IdType       = FDCAN_STANDARD_ID;       	//< id设置为标准id
	filter.FilterIndex  = 0;                      	//< 设忼筛�?�器的编号，标准id选择0-127
	filter.FilterType   = FDCAN_FILTER_MASK;       	//< 设置工作模式为掩码模�?
	filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; 	//< 将经过过滤的数据存储�? fifo0
	filter.FilterID1    = 0x000;                   	//< 筛鿉器的id
	filter.FilterID2    = 0x000;
	
	HAL_FDCAN_ConfigFilter(can, &filter);   //< 配置过滤�?	
  HAL_FDCAN_Start(can);                   //< 使能can
    //该check来测试can控制器是否使能，可以把该赋忼去掿	
	// 初始化PID结构�?
    for (int i = 0; i < 4; i++) {
        PID_struct_init(&pid_motor[i], POSITION_PID, 10000, 16384,2.9f, 0.1f, 0.1f);
    }
	HAL_FDCAN_ActivateNotification(can, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);  // 使能fifo0接收到新信息中断

}






uint8_t chassis_can_send_data[8];
uint8_t CAN_Send(FDCAN_HandleTypeDef* can,int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4 )
{
	FDCAN_TxHeaderTypeDef txHeader;
	txHeader.Identifier = (uint32_t)0x200;
	txHeader.IdType = FDCAN_STANDARD_ID;
	txHeader.TxFrameType = FDCAN_DATA_FRAME;
	txHeader.DataLength = FDCAN_DLC_BYTES_8;
	txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    txHeader.BitRateSwitch = FDCAN_BRS_OFF;
	txHeader.FDFormat = FDCAN_CLASSIC_CAN;                    
	txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	txHeader.MessageMarker = 0x00;
	
	chassis_can_send_data[0] = motor1 >> 8; 
    chassis_can_send_data[1] = motor1; 
    chassis_can_send_data[2] = motor2 >> 8; 
    chassis_can_send_data[3] = motor2; 
    chassis_can_send_data[4] = motor3 >> 8; 
    chassis_can_send_data[5] = motor3; 
    chassis_can_send_data[6] = motor4 >> 8; 
	chassis_can_send_data[7] = motor4; 

if(HAL_FDCAN_AddMessageToTxFifoQ(can, &txHeader,chassis_can_send_data) != HAL_OK)
	{
			return 0;
	}
	else
	{
		return 1;
	}		
}

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
	CAN_3508Moto1_ID = 0x201,
	CAN_3508Moto2_ID = 0x202,
	CAN_3508Moto3_ID = 0x203,
	CAN_3508Moto4_ID = 0x204,
	
}CAN_Message_ID;

 typedef struct 
{
  uint16_t angle;
  uint16_t rpm;
  uint16_t elc;
}motor;
motor data;

motor_measure_t motor_chassis[4] = {0};//4 chassis motor

#define get_motor_measure(ptr, data)\
 { \
 (ptr)->last_ecd = (ptr)->ecd; \
 (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]); \
 (ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]); \
 (ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]); \
 (ptr)->temperate = (data)[6]; \
 } 

 
 void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hcan,uint32_t RxFifo0ITs) //回调
{ 
	if(hcan==&hfdcan1)
	{
 FDCAN_RxHeaderTypeDef rx_header; 
		
 uint8_t rx_data[8]; 
 
 HAL_FDCAN_GetRxMessage(hcan, FDCAN_RX_FIFO0, &rx_header, rx_data); 
 
 switch (rx_header.Identifier) 
 { 
 case CAN_3508Moto1_ID: 
 case CAN_3508Moto2_ID: 
 case CAN_3508Moto3_ID: 
 case CAN_3508Moto4_ID: 

 { 
 static uint8_t i = 0; 
 //get motor id 
 i = rx_header.Identifier - CAN_3508Moto1_ID; 
 get_motor_measure(&motor_chassis[i], rx_data); 
	 // 进行PID计算并更新鿟度设定�??
	 //CAN_Send( &hfdcan1,4000, set_speed[1],set_speed[2], set_speed[3]);
	//data.angle = (uint16_t)((rx_Data[0] << 8) | rx_Data[1]);
	//float k=(data.angle/8194)*360;
	//speed1= motor_chassis[i].speed_rpm;
  set_speed[i] = pid_calc(&pid_motor[i], motor_chassis[i].speed_rpm, 5000 );
  CAN_Send( &hfdcan1,set_speed[0], set_speed[1],set_speed[2], set_speed[3]);
	speed1= motor_chassis[i].speed_rpm; 

 break; 
 } 
 default: 
 { 
 break; 
 } 
 } 
	}
} 


//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//	{ 
//  if(htim == &htim1)  //判断中断是否来自于定时器1
//   {
//	 //volatile float speed1;
//   OLED_NewFrame();		
//   sprintf(speed, "%f",speed1 );
//	 OLED_PrintString(0,0,  "实时速度", &font16x16, OLED_COLOR_NORMAL);
//   OLED_PrintString(60,0,  speed, &font16x16, OLED_COLOR_NORMAL);
//   OLED_ShowFrame();
//   }
//}
/* USER CODE END 1 */
