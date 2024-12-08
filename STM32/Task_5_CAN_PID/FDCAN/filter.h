#include "main.h"
#include "fdcan.h"
#include "memorymap.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
void CAN_Send(FDCAN_HandleTypeDef* can,int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4 );
void CAN_Open(FDCAN_HandleTypeDef* can);