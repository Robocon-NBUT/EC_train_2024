/****************************************************************************
 *  Copyright (C) 2018 RoboMaster.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/
 
#ifndef _PID_H
#define _PID_H

#include "main.h"

//typedef struct _pid_struct_t
//{
//  float kp;
//  float ki;
//  float kd;
//  float i_max;
//  float out_max;
//  
//  float ref;      // target value
//  float fdb;      // feedback value
//  float err[2];   // error and last error

//  float p_out;
//  float i_out;
//  float d_out;
//  float output;
//}pid_struct_t;

typedef struct {
    float kp;          // 比例系数
    float ki;          // 积分系数
    float kd;          // 微分系数
    float i_max;       // 积分限幅最大值
    float out_max;     // 输出限幅最大值
    float err[3];      // 存储当前、上一、上上时刻的误差
    float last_output; // 上一时刻的输出值
} IncrementalPID_struct_t;

//void pid_init(pid_struct_t *pid,
//              float kp,
//              float ki,
//              float kd,
//              float i_max,
//              float out_max);
//              
//float  pid_calc(pid_struct_t *pid, float ref, float fdb);
//float LIMIT_MIN_MAX(float i_out,float i_max,float i_min);
							
void IncrementalPID_init(IncrementalPID_struct_t *pid,
                         float kp,
                         float ki,
                         float kd,
                         float i_max,
                         float out_max);
float IncrementalPID_calc(IncrementalPID_struct_t *pid, float ref, float fdb);
							
#endif
