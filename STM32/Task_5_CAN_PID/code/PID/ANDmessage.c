#include "stm32h7xx_hal.h"
#include "main.h"
#include "usart.h"
#include "ANDmessage.h"




static par_struct      send_parameter;       //<发送帧中的参数;	
static par_struct      rec_parameter;        //<接收帧的参数;
static ano_frameStruct send_frame_struct;    //<(发送)通信帧结构体
__IO ano_frameStruct   rec_frame_struct;     //<(接收)通信帧结构体,因不止在本.c文件使用,故不用static修饰






/**
  * @brief   初始化通信帧结构体,使用前必须调用
  * @param   无输入参数
  * @retval  无返回
  **/
void ano_frame_init(void)
{
	/*参数结构体初始化*/
	send_frame_struct.parameter = &send_parameter;
	rec_frame_struct.parameter = &rec_parameter;
	send_frame_struct.parameter->par_id = 0;
	send_frame_struct.parameter->par_val = 0;
	rec_frame_struct.parameter->par_id = 0;
	rec_frame_struct.parameter->par_val = 0;

	send_frame_struct.head = rec_frame_struct.head = FRAME_HEADER;//帧头固定是0XAA
	send_frame_struct.target_addr = rec_frame_struct.target_addr = HOST_ADDR;
	send_frame_struct.function_id = 0XFF;//<协议中没有定义的功能ID,这样初始化目的是为了启动瞬间不做任何动作


	memset(send_frame_struct.data, 0, 40);//<缓存默认全部置0
	memset(rec_frame_struct.data, 0, 40);
}



/**
  * @brief   复位通信帧结构体，ano_frame_init()必须要运行过一次
  * @param   通信帧结构体对象
  * @retval  无返回
  **/
void ano_frame_reset(ano_frameStruct* frame)
{
	frame->function_id = 0XFF;
	frame->data_len = 0;
	memset(frame->data, 0, 40);
	frame->add_check = 0;
	frame->sum_check = 0;
}



/**
  * @brief   通信帧中参数结构体内成员的配置
  * @param   通信帧结构体对象，参数ID与参数值
  * @retval
  **/
void ano_par_struct_config(ano_frameStruct* frame, uint16_t id, int32_t val)
{
	frame->parameter->par_id = id;
	frame->parameter->par_val = val;
}



/**
  * @brief   通信帧校验计算
  * @param   通信帧结构体对象
  * @retval  无返回值
  **/
void ano_check_calculate(ano_frameStruct* frame)
{
	frame->sum_check = 0;
	frame->add_check = 0;

	//除去和校验，附加校验及数据部分，有4个部分4个字节，长度固定
	for (uint32_t i = 0; i < 4; i++)
	{
		frame->sum_check += *(uint8_t*)(&frame->head + i);
		frame->add_check += frame->sum_check;
	}
	//获取数据长度部位,把数据部分全加上
	for (uint32_t i = 0; i < frame->data_len; i++)
	{
		frame->sum_check += *((uint8_t*)(frame->data) + i);
		frame->add_check += frame->sum_check;
	}
}



/**
  * @brief   通信帧校验检查(接收上位机通信帧时用)
  * @param   通信帧结构体对象
 * @retval   1：校验成功 0:校验失败
  **/
extern uint8_t ano_check(ano_frameStruct* frame)
{
	uint8_t sum_check = 0;
	uint8_t add_check = 0;

	for (uint32_t i = 0; i < 4; i++)
	{
		sum_check += *(uint8_t*)(&frame->head + i);
		add_check += sum_check;
	}
	for (uint32_t i = 0; i < frame->data_len; i++)
	{
		sum_check += *((uint8_t*)(frame->data) + i);
		add_check += sum_check;
	}
	//如果计算与获取的相等，校验成功
	if ((sum_check == frame->sum_check) && (add_check == frame->add_check))
		return 1;
	else
		return 0;
}



/**
  * @brief  匿名串口发送
  * @param  字符串，数据字节个数
  * @retval
  */
static void ano_usart_send(uint8_t* str, uint16_t num)
{
	uint16_t cnt = 0;
	do
	{
		HAL_UART_Transmit(&huart5, ((uint8_t*)(str)) + cnt, 1, 1000);
		cnt++;
	} while (cnt <= num);
}



/**
  * @brief   通信帧结构体转化为线性数组
  * @param   要转换的通信帧，缓存数组
  * @retval
  **/
void frame_turn_to_array(ano_frameStruct* frame, uint8_t* str)
{
	memcpy(str, (uint8_t*)frame, 4);
	memcpy(str + 4, (uint8_t*)frame->data, frame->data_len);
	memcpy(str + 4 + frame->data_len, (uint8_t*)(&frame->sum_check), 2);
}



/**
 * @brief  向上位机发送ASCII字符串
 * @param  color:希望上位机显示的字符串颜色，str：要发送的字符串
 * @retval 无返回值
 */
void ano_send_string(uint8_t color, char* str)
{
	uint8_t i = 0, cnt = 0;
	uint8_t buff[46];
	memset(send_frame_struct.data, 0, 40);
	send_frame_struct.function_id = 0XA0;           //<信息输出--字符串(功能码0XA0)
	send_frame_struct.data[cnt++] = color;          //<选择上位机打印的颜色
	/*字符串数据直接存入数据部分*/
	while (*(str + i) != '\0')
	{
		send_frame_struct.data[cnt++] = *(str + i++);
		if (cnt > 40)                                //<若字符串长度超过40，强制结束
			break;
	}
	send_frame_struct.data_len = cnt;               //<记录下数据部分长度

	ano_check_calculate(&send_frame_struct);      //<计算校验和
	frame_turn_to_array(&send_frame_struct, buff); //<通信帧转线性数组
	ano_usart_send(buff, 6 + send_frame_struct.data_len);
}



/**
  * @brief  向上位机发送ASCII字符串+数字组合
  * @param  value:32位的数值，str：要发送的字符串
  * @retval
  */
void ano_send_message(char* str, int32_t value)
{
	uint8_t i = 0, cnt = 0;
	uint8_t buff[46];
	memset(send_frame_struct.data, 0, 40);
	send_frame_struct.function_id = 0XA1;	          //信息输出--字符串+数字

	/*协议规定VAL在前，先对要求的32位数据进行截断*/
	send_frame_struct.data[cnt++] = BYTE0(value);
	send_frame_struct.data[cnt++] = BYTE1(value);
	send_frame_struct.data[cnt++] = BYTE2(value);
	send_frame_struct.data[cnt++] = BYTE3(value);
	/*再轮到字符串数据*/
	while (*(str + i) != '\0')
	{
		send_frame_struct.data[cnt++] = *(str + i++);
		if (cnt > 40)
			break;
	}

	send_frame_struct.data_len = cnt;				  //<记录下数据部分长度

	ano_check_calculate(&send_frame_struct);	  //<计算校验和
	frame_turn_to_array(&send_frame_struct, buff); //<通信帧转线性数组
	ano_usart_send(buff, 6 + send_frame_struct.data_len);
}



/**
  * @brief  发送灵活格式帧
  * @param  id:0xF1~0XFA，x_coordinate:x轴坐标值 ，y_coordinate：y轴坐标值
  *         !!!要传多少个参数完全可以自己进行计算，最高只支持40字节的数据，低位先输出
  *         一般10个以内够用，40个字节限制，一个32位数据占4个字节，可以发送10个
  * @retval
  */
void ano_send_flexible_frame(uint8_t id, int32_t x_coordinate, int32_t y_coordinate)
{
	uint8_t buff[46];

	memset(send_frame_struct.data, 0, 40);
	send_frame_struct.function_id = id;
	send_frame_struct.data_len = 8;			   //<根据自己的参数数填写							

	/*第一个x_coordinate数据从低位到高位截断*/
	send_frame_struct.data[0] = BYTE0(x_coordinate);
	send_frame_struct.data[1] = BYTE1(x_coordinate);
	send_frame_struct.data[2] = BYTE2(x_coordinate);
	send_frame_struct.data[3] = BYTE3(x_coordinate);
	/*第二个数据y_coordinate从低位到高位截断*/
	send_frame_struct.data[4] = BYTE0(y_coordinate);
	send_frame_struct.data[5] = BYTE1(y_coordinate);
	send_frame_struct.data[6] = BYTE2(y_coordinate);
	send_frame_struct.data[7] = BYTE3(y_coordinate);
	/*第N个数据xxx从低位到高位截断*/
	//......用户自行添加

	ano_check_calculate(&send_frame_struct);
	frame_turn_to_array(&send_frame_struct, buff);


	ano_usart_send(buff, 6 + send_frame_struct.data_len);
}
