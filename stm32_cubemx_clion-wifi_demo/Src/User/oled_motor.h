//
// Created by Cause_Hhc on 2020/9/15.
//

#ifndef MAZE_STM32_CUBEMX_CLION_OLED_MOTOR_H
#define MAZE_STM32_CUBEMX_CLION_OLED_MOTOR_H
#include "motor.h"

void OLED_preFlash();
void flash_OLED_info(motorInfoType leftInfo, motorInfoType rightInfo, uint8_t speed);
#endif //MAZE_STM32_CUBEMX_CLION_OLED_MOTOR_H
