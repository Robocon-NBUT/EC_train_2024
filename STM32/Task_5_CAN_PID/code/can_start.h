#include "main.h"
#include "fdcan.h"
#include "memorymap.h"
#include "gpio.h"
void CAN_Send(FDCAN_HandleTypeDef* can,int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4 );
void CAN_Open(FDCAN_HandleTypeDef* can);
float PID_Compute(PID_Controller* pid, float measured_value, float dt);
