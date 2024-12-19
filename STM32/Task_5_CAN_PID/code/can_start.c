#include "main.h"
#include "fdcan.h"
#include "memorymap.h"
#include "gpio.h"
#include "can_start.h"


/**
  * @brief CAN发送函数
  */
uint8_t chassis_can_send_data[8];
void CAN_Send(FDCAN_HandleTypeDef* can,int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4 )
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

	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &txHeader, chassis_can_send_data);
}

void CAN_Open(FDCAN_HandleTypeDef* can) 
{ 
    FDCAN_FilterTypeDef filter;                   	//< 声明局部变量 can过滤器结构体
	filter.IdType       = FDCAN_STANDARD_ID;       	//< id设置为标准id
	filter.FilterIndex  = 0;                      	//< 设值筛选器的编号，标准id选择0-127
	filter.FilterType   = FDCAN_FILTER_MASK;       	//< 设置工作模式为掩码模式
	filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; 	//< 将经过过滤的数据存储到 fifo0
	filter.FilterID1    = 0x000;                   	//< 筛选器的id
	filter.FilterID2    = 0x000;
	
	HAL_FDCAN_ConfigFilter(can, &filter);   //< 配置过滤器	
  HAL_FDCAN_Start(can);  
	HAL_FDCAN_ActivateNotification(can, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);  // 使能fifo0接收到新信息中断

}


