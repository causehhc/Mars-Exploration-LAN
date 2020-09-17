//
// Created by Cause_Hhc on 2020/8/24.
//

#ifndef MAZE_STM32_CUBEMX_CLION_MOTOR_H
#define MAZE_STM32_CUBEMX_CLION_MOTOR_H
#include "tim.h"

typedef struct {
    int ENC;  //encoder: 当前编码器示数
    int TGT;  //target: 电机转速目标值
    int PWM;  //pwm: 电机实际PWM赋值
    int ADD;  //add: 编码器累计示数
    int TGTADD;  //target: 电机编码目标值
}motorInfoType;

typedef struct {
    int Kp; //p参数
    int Ki; //i参数
    int E;  //当前差距
    int lastE;  //上一个差距
}PI_Type;

void check_ENC(motorInfoType *leftInfo, motorInfoType *rightInfo);
void plus_ADD(motorInfoType *leftInfo, motorInfoType *rightInfo);
void incremental_PI_vel(motorInfoType *motorInfo, PI_Type *PIvel);
void range_PWM(motorInfoType *leftInfo, motorInfoType *rightInfo, int amplitude);
void set_PWM(motorInfoType *leftInfo, motorInfoType *rightInfo);
void incremental_PI_pos(motorInfoType *motorInfo, PI_Type *PIpos);

#endif //MAZE_STM32_CUBEMX_CLION_MOTOR_H
