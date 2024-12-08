#include "M2006.h"

void CAN_Open(FDCAN_HandleTypeDef *can)
{
  FDCAN_FilterTypeDef filter;                    //< 声明局部变量 can过滤器结构体
  filter.IdType = FDCAN_STANDARD_ID;             //< id设置为标准id
  filter.FilterIndex = 0;                        //< 设值筛选器的编号，标准id选择0-127
  filter.FilterType = FDCAN_FILTER_MASK;         //< 设置工作模式为掩码模式
  filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; //< 将经过过滤的数据存储到 fifo0
  filter.FilterID1 = 0x000;                      //< 筛选器的id
  filter.FilterID2 = 0x000;

  HAL_FDCAN_ConfigFilter(can, &filter); //< 配置过滤器
  HAL_FDCAN_Start(can);
  HAL_FDCAN_ActivateNotification(can, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0); // 使能fifo0接收到新信息中断
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

void sendFloat(UART_HandleTypeDef *huart, float value, int precision)
{
  char buffer[50]; // 字符串缓冲区

  // 处理负数
  char *ptr = buffer;
  if (value < 0)
  {
    *ptr++ = '-';
    value = -value; // 转为正数处理
  }

  // 取整数部分
  int intPart = (int)value;

  // 取小数部分，利用精度控制
  float fractionalPart = value - intPart;
  for (int i = 0; i < precision; i++)
  {
    fractionalPart *= 10;
  }
  int fractionalInt = (int)(fractionalPart + 0.5f); // 四舍五入

  // 将整数部分转换为字符串
  char temp[20]; // 临时存储整数部分
  int tempIndex = 0;
  if (intPart == 0)
  {
    temp[tempIndex++] = '0';
  }
  else
  {
    while (intPart > 0)
    {
      temp[tempIndex++] = (intPart % 10) + '0'; // 转为字符
      intPart /= 10;
    }
  }
  // 反转整数部分字符串
  for (int i = tempIndex - 1; i >= 0; i--)
  {
    *ptr++ = temp[i];
  }

  // 添加小数点
  if (precision > 0)
  {
    *ptr++ = '.';

    // 将小数部分转换为字符串
    for (int i = precision - 1; i >= 0; i--)
    {
      ptr[i] = (fractionalInt % 10) + '0'; // 转为字符
      fractionalInt /= 10;
    }
    ptr += precision;
  }

  // 添加字符串结束符
  *ptr++ = ' '; // 添加空格
  *ptr = '\0';  // 字符串结束符

  // 通过串口发送字符串
  HAL_UART_Transmit(huart, (uint8_t *)buffer, ptr - buffer, HAL_MAX_DELAY);
}
void PID_init(PID *pid, float kp, float ki, float kd,
              float max_output, float min_output,
              float max_integral, float min_integral,
              float alpha, float deadzone)
{
  // 初始化 PID 参数
  pid->kp = kp;
  pid->ki = ki;
  pid->kd = kd;

  // 初始化积分项和误差状态
  pid->integral = 0;
  pid->previous_error = 0;

  // 初始化输出限制
  pid->max_output = max_output;
  pid->min_output = min_output;

  // 初始化积分限制
  pid->max_integral = max_integral;
  pid->min_integral = min_integral;

  // 初始化微分滤波系数
  pid->filtered_derivative = 0; // 初始微分值
  pid->alpha = alpha;           // 微分滤波权重

  // 初始化死区范围
  pid->deadzone = deadzone;

  // 初始化调试辅助参数
  pid->last_output = 0;
  pid->error = 0;
  pid->derivative = 0;
}

float pid_compute(PID *pid, float setpoint, float current_value, float dt)
{
  // 计算误差
  float error = setpoint - current_value;

  pid->integral += error * dt;
  if (pid->integral > pid->max_integral)
    pid->integral = pid->max_integral;
  else if (pid->integral < pid->min_integral)
    pid->integral = pid->min_integral;
  // 计算微分项
  float derivative = (error - pid->previous_error) / dt;
  pid->filtered_derivative = pid->alpha * derivative + (1 - pid->alpha) * pid->filtered_derivative;
  // 计算输出
  float output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;

  // 限制输出范围
  if (output > pid->max_output)
    output = pid->max_output;
  else if (output < pid->min_output)
    output = pid->min_output;

  // 保存当前误差以备下次使用
  pid->previous_error = error;

  return output;
}
