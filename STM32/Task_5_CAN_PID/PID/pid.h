typedef struct
{
   	float kp, ki, kd; //三个系数
    float error, lastError; //误差、上次误差
    float integral, maxIntegral; //积分、积分限幅
    float output, maxOutput; //输出、输出限幅
}PID;

void PID_Init(PID *pid, float p, float i, float d, float maxI, float maxOut);

void PID_Calc(PID *pid, float reference, float feedback);


