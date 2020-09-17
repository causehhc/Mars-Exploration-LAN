//
// Created by Cause_Hhc on 2020/8/24.
//

#include "motor.h"

void check_ENC(motorInfoType *leftInfo, motorInfoType *rightInfo){
  leftInfo->ENC = (short)__HAL_TIM_GET_COUNTER(&htim3);
  __HAL_TIM_SET_COUNTER(&htim3, 0);
  rightInfo->ENC = (short)__HAL_TIM_GET_COUNTER(&htim4);
  __HAL_TIM_SET_COUNTER(&htim4, 0);
}

void plus_ADD(motorInfoType *leftInfo, motorInfoType *rightInfo){
  leftInfo->ADD += leftInfo->ENC;
  rightInfo->ADD += rightInfo->ENC;
}

void incremental_PI_vel(motorInfoType *motorInfo, PI_Type *PIvel){
  PIvel->E = motorInfo->TGT - motorInfo->ENC;
  motorInfo->PWM += (PIvel->Kp*(PIvel->E-PIvel->lastE)+PIvel->Ki*PIvel->E);
  PIvel->lastE = PIvel->E;
}

void range_PWM(motorInfoType *leftInfo, motorInfoType *rightInfo, int amplitude){
  if(leftInfo->PWM < -amplitude) leftInfo->PWM = -amplitude;
  if(leftInfo->PWM > amplitude) leftInfo->PWM = amplitude;
  if(rightInfo->PWM < -amplitude) rightInfo->PWM = -amplitude;
  if(rightInfo->PWM > amplitude) rightInfo->PWM = amplitude;
}

void set_PWM(motorInfoType *leftInfo, motorInfoType *rightInfo){
  if(leftInfo->PWM >= 0){
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, leftInfo->PWM);
  }else{
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 7200);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 7200 + leftInfo->PWM);
  }

  if(rightInfo->PWM >= 0){
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 0);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, rightInfo->PWM);
  }else{
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 7200);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, 7200 + rightInfo->PWM);
  }
}

void incremental_PI_pos(motorInfoType *motorInfo, PI_Type *PIpos){
  PIpos->E = motorInfo->TGTADD - motorInfo->ADD;
  motorInfo->TGT += (PIpos->Kp * (PIpos->E - PIpos->lastE) + PIpos->Ki * PIpos->E);
  PIpos->lastE = PIpos->E;
}
