#include "main.h"
#include "dma.h"
#include "fdcan.h"
#include "memorymap.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

typedef struct 
{ 
    int16_t Rotor_Angle;  // 转子角度 0~8192 0~360°
    int16_t Speed_RPM;    // 每分钟电机转速(RPM)
    int16_t Real_Current; // 实际电机电流值 -16384~+16384
    uint16_t Temp;        // 电机温度
    int16_t Last_Angle;   // 上次读取到的角度
    int16_t Offset_Angle; // 初始位置偏移角度
    int32_t Round_Cnt;    // 转子旋转圈数
    int32_t Total_Angle;  // 总角度
    uint32_t Msg_Cnt;     // 消息计数
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


void get_moto_measure(motor_measure_t *ptr);
void get_moto_offset(motor_measure_t *ptr);
void CAN_Send(FDCAN_HandleTypeDef* can,int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4 );
void CAN_Open(FDCAN_HandleTypeDef* can);
uint8_t fdcanx_receive(FDCAN_HandleTypeDef *hfdcan, uint16_t *rec_id, uint8_t *buf);
