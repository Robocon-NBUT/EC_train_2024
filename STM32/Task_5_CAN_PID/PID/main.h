/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
#ifdef __cplusplus
extern "C" {

#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

typedef struct {
    float kp;
    float ki;
    float kd;
    float setpoint;
    float process_variable;
    float integral;
    float prev_error;
} PID_t;
extern uint8_t can_rx_buffer[8];
// 最大转速限制值（假设为每分钟转数，可根据实际电机参数调整）
#define MAX_RPM 500

// 定义一个宏用于将RPM转换为适合电机控制的单位（这里假设是某种特定的计数单位，需根据实际情况调整）
#define RPM_TO_CONTROL_UNIT(rpm) ((int16_t)(rpm * 10))

// CAN消息ID用于接收电机位置信息（假设值，需根据实际情况调整）
#define MOTOR_POSITION_CAN_ID 0x300

// 函数声明
void PID_Init(PID_t *pid);
float PID_Update(PID_t *pid, float process_variable);
void CAN_Open(FDCAN_HandleTypeDef* can);
void HAL_FDCAN_RxCpltCallback(FDCAN_HandleTypeDef* hfdcan);
HAL_StatusTypeDef HAL_FDCAN_Receive_IT(FDCAN_HandleTypeDef*);
void Enable_CAN_Receive_Interrupt(FDCAN_HandleTypeDef* hfdcan);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
