#include "main.h"
#include "fdcan.h"

typedef struct {
    float kp;           // 比例系数
    float ki;           // 积分系数
    float kd;           // 微分系数
    float integral;     // 积分项
    float previous_error;  // 上次的误差
    float max_output;   // 输出上限
    float min_output;   // 输出下限
	
	
	  float kp_pos;  // 位置环比例系数
    float ki_pos;  // 位置环积分系数
    float kd_pos;  // 位置环微分系数
    float max_output_pos;  // 位置环最大输出（例如可转化为电机期望速度等控制量）
    float min_output_pos;  // 位置环最小输出
    float integral_pos;  // 位置环积分项累计值
    float previous_error_pos;  // 位置环上一次误差
    float previous_position_error;
	
	  float max_integral;  
    float min_integral; 
	
	  float derivative;     
    float alpha;         
    float filtered_derivative;
	
} PID;

typedef struct {
    float current_angle;
    float target_angle;
    float control_speed;
} Motor;




void CAN_Open(FDCAN_HandleTypeDef *can);
void motor_speed(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);

void PID_init(PID *pid, float kp, float ki, float kd, float max_output, float min_output,float max_integral, float min_integral,float alpha);


