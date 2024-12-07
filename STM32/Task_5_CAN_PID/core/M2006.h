#include "main.h"
#include "fdcan.h"
#include "math.h"
typedef struct {
    float kp;           // 比例系数
    float ki;           // 积分系数
    float kd;           // 微分系数
    float integral;     // 积分项
    float previous_error; // 上次的误差
    float max_output;     // 输出上限
    float min_output;     // 输出下限
    float max_integral;   // 积分项上限
    float min_integral;   // 积分项下限
    float deadzone;       // 死区范围（绝对值小于该值的误差将被视为 0）
    float last_output;    // 上一次的控制输出
    float error;          // 当前误差
    float derivative;     // 当前微分值（未滤波）
    float alpha;          // 微分滤波系数aa
    float filtered_derivative; // 滤波后的微分值
    float previous_output;//上一次输出
} PID;

void CAN_Open(FDCAN_HandleTypeDef *can);
void motor_speed(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void sendFloat(UART_HandleTypeDef *huart, float value, int precision);
void PID_init(PID *pid, float kp, float ki, float kd, float max_output, float min_output,float max_integral, float min_integral,float alpha, float deadzone,float previous_output);
float pid_compute(PID *pid, float setpoint, float current_value, float dt);