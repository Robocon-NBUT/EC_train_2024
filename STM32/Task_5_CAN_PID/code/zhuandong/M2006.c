#include "M2006.h"



void CAN_Open(FDCAN_HandleTypeDef *can)
{
  FDCAN_FilterTypeDef filter;                    
  filter.IdType = FDCAN_STANDARD_ID;             
  filter.FilterIndex = 0;                        
  filter.FilterType = FDCAN_FILTER_MASK;         
  filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; 
  filter.FilterID1 = 0x000;                      
  filter.FilterID2 = 0x000;

  HAL_FDCAN_ConfigFilter(can, &filter); 
  HAL_FDCAN_Start(can);
  HAL_FDCAN_ActivateNotification(can, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0); 
}


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



void PID_init(PID *pid, float kp, float ki, float kd, float max_output, float min_output,float max_integral, float min_integral,float alpha)
{
  pid->kp = kp;
  pid->ki = ki;
  pid->kd = kd;
  pid->integral = 0;
  pid->previous_error = 0;
  pid->max_output = max_output;
  pid->min_output = min_output;
	

	pid->max_integral = max_integral;
  pid->min_integral = min_integral;
	
	pid->derivative = 0;
	pid->filtered_derivative = 0; // 初始微分值
  pid->alpha = alpha;  
	
	  // 初始化输出限制
 

	
}







