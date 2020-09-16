//
// Created by Cause_Hhc on 2020/9/15.
//

#include "oled_motor.h"
#include "oled.h"

void OLED_preFlash(){
  OLED_Init();
  OLED_Clear();
  uint8_t char_size = 12;
  OLED_ShowString(30,0,"left",char_size);
  OLED_ShowString(80,0,"right",char_size);
  OLED_ShowString(1,2,"tgt",char_size);
  OLED_ShowString(1,3,"enc",char_size);
  OLED_ShowString(1,4,"pwm",char_size);
  OLED_ShowString(1,5,"add",char_size);
}

void change(int x, char *str, char len){
  for(int i=0; i<len; i++){
    str[i] = '0';
  }
  if(x >= 0){
    str[0] = '+';
  }else{
    str[0] = '-';
    x = -x;
  }
  str[--len] = '\0';
  while(x){
    str[--len] = x%10 + '0';
    x /= 10;
  }
}

void flash_OLED_info(motorInfoType leftInfo, motorInfoType rightInfo, uint8_t speed){
  static char x1=30,x2=80;
  static char y1=2,y2=2;
  static uint8_t char_size = 1;
  static char str_tgtA[6];
  static char str_tgtB[6];
  static char str_pwmA[6];
  static char str_pwmB[6];
  static char str_encA[6];
  static char str_encB[6];

  static char str_addA[6];
  static char str_addB[6];

  static char str_speed[6];

  change(leftInfo.TGT, str_tgtA, 6);
  change(rightInfo.TGT, str_tgtB, 6);
  change(leftInfo.PWM, str_pwmA, 6);
  change(rightInfo.PWM, str_pwmB, 6);
  change(leftInfo.ENC, str_encA, 6);
  change(rightInfo.ENC, str_encB, 6);
  change(leftInfo.ADD, str_addA, 6);
  change(rightInfo.ADD, str_addB, 6);

  change(speed, str_speed, 6);

  OLED_ShowString(x1,y1,str_tgtA,char_size);
  OLED_ShowString(x1,y1+1,str_encA,char_size);
  OLED_ShowString(x1,y1+2,str_pwmA,char_size);

  OLED_ShowString(x2,y2,str_tgtB,char_size);
  OLED_ShowString(x2,y2+1,str_encB,char_size);
  OLED_ShowString(x2,y2+2,str_pwmB,char_size);

  OLED_ShowString(x1,y1+3,str_addA,char_size);
  OLED_ShowString(x2,y2+3,str_addB,char_size);

  OLED_ShowString(x1,y2+4,str_speed,char_size);
}