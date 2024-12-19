#include "stm32h7xx_hal.h"
#include "main.h"
#include "usart.h"
#include "ANDmessage.h"




static par_struct      send_parameter;       //<����֡�еĲ���;	
static par_struct      rec_parameter;        //<����֡�Ĳ���;
static ano_frameStruct send_frame_struct;    //<(����)ͨ��֡�ṹ��
__IO ano_frameStruct   rec_frame_struct;     //<(����)ͨ��֡�ṹ��,��ֹ�ڱ�.c�ļ�ʹ��,�ʲ���static����






/**
  * @brief   ��ʼ��ͨ��֡�ṹ��,ʹ��ǰ�������
  * @param   ���������
  * @retval  �޷���
  **/
void ano_frame_init(void)
{
	/*�����ṹ���ʼ��*/
	send_frame_struct.parameter = &send_parameter;
	rec_frame_struct.parameter = &rec_parameter;
	send_frame_struct.parameter->par_id = 0;
	send_frame_struct.parameter->par_val = 0;
	rec_frame_struct.parameter->par_id = 0;
	rec_frame_struct.parameter->par_val = 0;

	send_frame_struct.head = rec_frame_struct.head = FRAME_HEADER;//֡ͷ�̶���0XAA
	send_frame_struct.target_addr = rec_frame_struct.target_addr = HOST_ADDR;
	send_frame_struct.function_id = 0XFF;//<Э����û�ж���Ĺ���ID,������ʼ��Ŀ����Ϊ������˲�䲻���κζ���


	memset(send_frame_struct.data, 0, 40);//<����Ĭ��ȫ����0
	memset(rec_frame_struct.data, 0, 40);
}



/**
  * @brief   ��λͨ��֡�ṹ�壬ano_frame_init()����Ҫ���й�һ��
  * @param   ͨ��֡�ṹ�����
  * @retval  �޷���
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
  * @brief   ͨ��֡�в����ṹ���ڳ�Ա������
  * @param   ͨ��֡�ṹ����󣬲���ID�����ֵ
  * @retval
  **/
void ano_par_struct_config(ano_frameStruct* frame, uint16_t id, int32_t val)
{
	frame->parameter->par_id = id;
	frame->parameter->par_val = val;
}



/**
  * @brief   ͨ��֡У�����
  * @param   ͨ��֡�ṹ�����
  * @retval  �޷���ֵ
  **/
void ano_check_calculate(ano_frameStruct* frame)
{
	frame->sum_check = 0;
	frame->add_check = 0;

	//��ȥ��У�飬����У�鼰���ݲ��֣���4������4���ֽڣ����ȹ̶�
	for (uint32_t i = 0; i < 4; i++)
	{
		frame->sum_check += *(uint8_t*)(&frame->head + i);
		frame->add_check += frame->sum_check;
	}
	//��ȡ���ݳ��Ȳ�λ,�����ݲ���ȫ����
	for (uint32_t i = 0; i < frame->data_len; i++)
	{
		frame->sum_check += *((uint8_t*)(frame->data) + i);
		frame->add_check += frame->sum_check;
	}
}



/**
  * @brief   ͨ��֡У����(������λ��ͨ��֡ʱ��)
  * @param   ͨ��֡�ṹ�����
 * @retval   1��У��ɹ� 0:У��ʧ��
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
	//����������ȡ����ȣ�У��ɹ�
	if ((sum_check == frame->sum_check) && (add_check == frame->add_check))
		return 1;
	else
		return 0;
}



/**
  * @brief  �������ڷ���
  * @param  �ַ����������ֽڸ���
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
  * @brief   ͨ��֡�ṹ��ת��Ϊ��������
  * @param   Ҫת����ͨ��֡����������
  * @retval
  **/
void frame_turn_to_array(ano_frameStruct* frame, uint8_t* str)
{
	memcpy(str, (uint8_t*)frame, 4);
	memcpy(str + 4, (uint8_t*)frame->data, frame->data_len);
	memcpy(str + 4 + frame->data_len, (uint8_t*)(&frame->sum_check), 2);
}



/**
 * @brief  ����λ������ASCII�ַ���
 * @param  color:ϣ����λ����ʾ���ַ�����ɫ��str��Ҫ���͵��ַ���
 * @retval �޷���ֵ
 */
void ano_send_string(uint8_t color, char* str)
{
	uint8_t i = 0, cnt = 0;
	uint8_t buff[46];
	memset(send_frame_struct.data, 0, 40);
	send_frame_struct.function_id = 0XA0;           //<��Ϣ���--�ַ���(������0XA0)
	send_frame_struct.data[cnt++] = color;          //<ѡ����λ����ӡ����ɫ
	/*�ַ�������ֱ�Ӵ������ݲ���*/
	while (*(str + i) != '\0')
	{
		send_frame_struct.data[cnt++] = *(str + i++);
		if (cnt > 40)                                //<���ַ������ȳ���40��ǿ�ƽ���
			break;
	}
	send_frame_struct.data_len = cnt;               //<��¼�����ݲ��ֳ���

	ano_check_calculate(&send_frame_struct);      //<����У���
	frame_turn_to_array(&send_frame_struct, buff); //<ͨ��֡ת��������
	ano_usart_send(buff, 6 + send_frame_struct.data_len);
}



/**
  * @brief  ����λ������ASCII�ַ���+�������
  * @param  value:32λ����ֵ��str��Ҫ���͵��ַ���
  * @retval
  */
void ano_send_message(char* str, int32_t value)
{
	uint8_t i = 0, cnt = 0;
	uint8_t buff[46];
	memset(send_frame_struct.data, 0, 40);
	send_frame_struct.function_id = 0XA1;	          //��Ϣ���--�ַ���+����

	/*Э��涨VAL��ǰ���ȶ�Ҫ���32λ���ݽ��нض�*/
	send_frame_struct.data[cnt++] = BYTE0(value);
	send_frame_struct.data[cnt++] = BYTE1(value);
	send_frame_struct.data[cnt++] = BYTE2(value);
	send_frame_struct.data[cnt++] = BYTE3(value);
	/*���ֵ��ַ�������*/
	while (*(str + i) != '\0')
	{
		send_frame_struct.data[cnt++] = *(str + i++);
		if (cnt > 40)
			break;
	}

	send_frame_struct.data_len = cnt;				  //<��¼�����ݲ��ֳ���

	ano_check_calculate(&send_frame_struct);	  //<����У���
	frame_turn_to_array(&send_frame_struct, buff); //<ͨ��֡ת��������
	ano_usart_send(buff, 6 + send_frame_struct.data_len);
}



/**
  * @brief  ��������ʽ֡
  * @param  id:0xF1~0XFA��x_coordinate:x������ֵ ��y_coordinate��y������ֵ
  *         !!!Ҫ�����ٸ�������ȫ�����Լ����м��㣬���ֻ֧��40�ֽڵ����ݣ���λ�����
  *         һ��10�����ڹ��ã�40���ֽ����ƣ�һ��32λ����ռ4���ֽڣ����Է���10��
  * @retval
  */
void ano_send_flexible_frame(uint8_t id, int32_t x_coordinate, int32_t y_coordinate)
{
	uint8_t buff[46];

	memset(send_frame_struct.data, 0, 40);
	send_frame_struct.function_id = id;
	send_frame_struct.data_len = 8;			   //<�����Լ��Ĳ�������д							

	/*��һ��x_coordinate���ݴӵ�λ����λ�ض�*/
	send_frame_struct.data[0] = BYTE0(x_coordinate);
	send_frame_struct.data[1] = BYTE1(x_coordinate);
	send_frame_struct.data[2] = BYTE2(x_coordinate);
	send_frame_struct.data[3] = BYTE3(x_coordinate);
	/*�ڶ�������y_coordinate�ӵ�λ����λ�ض�*/
	send_frame_struct.data[4] = BYTE0(y_coordinate);
	send_frame_struct.data[5] = BYTE1(y_coordinate);
	send_frame_struct.data[6] = BYTE2(y_coordinate);
	send_frame_struct.data[7] = BYTE3(y_coordinate);
	/*��N������xxx�ӵ�λ����λ�ض�*/
	//......�û��������

	ano_check_calculate(&send_frame_struct);
	frame_turn_to_array(&send_frame_struct, buff);


	ano_usart_send(buff, 6 + send_frame_struct.data_len);
}
