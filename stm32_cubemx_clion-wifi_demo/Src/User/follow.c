//
// Created by Cause_Hhc on 2020/9/15.
//

#include "follow.h"

extern char ir1, ir2, ir3, ir4, ir5;

void flash_ir(){
  if(HAL_GPIO_ReadPin(IR1_GPIO_Port, IR1_Pin)) ir1 = 1;
  else ir1 = 0;
  if(HAL_GPIO_ReadPin(IR2_GPIO_Port, IR2_Pin)) ir2 = 1;
  else ir2 = 0;
  if(HAL_GPIO_ReadPin(IR3_GPIO_Port, IR3_Pin)) ir3 = 1;
  else ir3 = 0;
  if(HAL_GPIO_ReadPin(IR4_GPIO_Port, IR4_Pin)) ir4 = 1;
  else ir4 = 0;
  if(HAL_GPIO_ReadPin(IR5_GPIO_Port, IR5_Pin)) ir5 = 1;
  else ir5 = 0;
}