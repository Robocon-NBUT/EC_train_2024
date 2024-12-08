#include "main.h"
#include "fdcan.h"
typedef struct {
    float kp;           
    float ki;           
    float kd;           
    float integral;    
    float previous_error; 
    float max_output;    
    float min_output;    
    float max_integral;   
    float min_integral;   
    float deadzone;      
    float last_output;    
    float error;         
    float derivative;     
    float alpha;          
    float filtered_derivative; 
} PID;
void CAN_Open(FDCAN_HandleTypeDef *can);
void motor_speed(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);

void PID_init(PID *pid, float kp, float ki, float kd,float max_output, float min_output,float max_integral, float min_integral,float alpha, float deadzone);
float pid_compute(PID *pid, float setpoint, float current_value,float dt);

