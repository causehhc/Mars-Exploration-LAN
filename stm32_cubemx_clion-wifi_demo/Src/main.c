/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "User/motor.h"
#include "User/oled_motor.h"
#include "User/follow.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "User/oled.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFLEN 10
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t RxBuffer1;
uint8_t RxBuffer2;

motorInfoType leftInfo;
motorInfoType rightInfo;
PI_Type leftPIvel;
PI_Type rightPIvel;
PI_Type leftPIpos;
PI_Type rightPIpos;

uint8_t control[2] = {0};

char ir1=0, ir2=0, ir3=0, ir4=0, ir5=0;

uint16_t usartCnt = 0;
uint16_t beforeCnt = 0;
uint8_t checkTime = 0;

uint8_t speed = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void para_init(){
  leftInfo.ENC = 0;
  leftInfo.PWM = 0;
  leftInfo.ADD = 0;
  leftInfo.TGT = 0;

  rightInfo.ENC = 0;
  rightInfo.PWM = 0;
  rightInfo.ADD = 0;
  rightInfo.TGT = 0;

  leftPIvel.Kp = 800;
  leftPIvel.Ki = 100;
  leftPIvel.E = 0;
  leftPIvel.lastE = 0;

  rightPIvel.Kp = 800;
  rightPIvel.Ki = 100;
  rightPIvel.E = 0;
  rightPIvel.lastE = 0;

  leftPIpos.Kp = 1;
  leftPIpos.Ki = 1;
  leftPIpos.E = 0;
  leftPIpos.lastE = 0;

  rightPIpos.Kp = 1;
  rightPIpos.Ki = 1;
  rightPIpos.E = 0;
  rightPIpos.lastE = 0;
}

void check_timeout(){
  if(checkTime++ == 100){
    checkTime = 0;
    if(beforeCnt == usartCnt){
      leftInfo.TGT=0;
      rightInfo.TGT=0;
    } else{
      beforeCnt = usartCnt;
    }
  }
}

void control_motor(){
  if(control[0] == '1'){
    if(speed<50) speed++;
  }else if(control[0] == '2'){
    if(speed>0) speed--;
  }

  if(control[1] == '0'){
    leftInfo.TGT = 0;
    rightInfo.TGT = 0;
  }else if(control[1] == '1'){
    leftInfo.TGT = speed;
    rightInfo.TGT = -speed;
  }else if(control[1] == '2'){
    leftInfo.TGT = speed;
    rightInfo.TGT = -speed/2;
  }else if(control[1] == '3'){
    leftInfo.TGT = speed;
    rightInfo.TGT = speed;
  }else if(control[1] == '4'){
    leftInfo.TGT = -speed;
    rightInfo.TGT = speed/2;
  }else if(control[1] == '5'){
    leftInfo.TGT = -speed;
    rightInfo.TGT = speed;
  }else if(control[1] == '6'){
    leftInfo.TGT = -speed/2;
    rightInfo.TGT = speed;
  }else if(control[1] == '7'){
    leftInfo.TGT = -speed;
    rightInfo.TGT = -speed;
  }else if(control[1] == '8') {
    leftInfo.TGT = speed / 2;
    rightInfo.TGT = -speed;
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  para_init();

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM6_Init();
  MX_I2C2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM8_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  OLED_preFlash();

  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1|TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1|TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_4);

  HAL_UART_Receive_IT(&huart1, &RxBuffer1, 1);
  HAL_UART_Receive_IT(&huart2, &RxBuffer2, 1);
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if(!HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin)){
      while(!HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin));
      HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin); //翻转LED
      leftInfo.TGTADD = 90000;
//      rightInfo.TGTADD = -10;
    }
    printf("ENC:%d TGT:%d PWM:%d ADD:%d TGTADD%d\n", leftInfo.ENC, leftInfo.TGT, leftInfo.PWM, leftInfo.ADD, leftInfo.TGTADD);
    HAL_Delay(100);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  if (htim->Instance == htim6.Instance){ //10ms
//    //check_time
//    check_timeout();

//    //ir
//    flash_ir();

    //motorControl
    check_ENC(&leftInfo, &rightInfo);
    plus_ADD(&leftInfo, &rightInfo);
    incremental_PI_pos(&leftInfo, &leftPIpos);
    incremental_PI_vel(&leftInfo, &leftPIvel);
    incremental_PI_pos(&rightInfo, &rightPIpos);
    incremental_PI_vel(&rightInfo, &rightPIvel);
    range_PWM(&leftInfo, &rightInfo, 7000);
    set_PWM(&leftInfo, &rightInfo);
  }
  if (htim->Instance == htim7.Instance){ //300ms
    flash_OLED_info(leftInfo, rightInfo, speed);
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  if(huart == &huart1){
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin); //翻转LED
    HAL_UART_Transmit(&huart1, &RxBuffer1, sizeof(uint8_t), 100);
    HAL_UART_Receive_IT(&huart1, &RxBuffer1, 1);

  }
  if(huart == &huart2){
//    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin); //翻转LED
//    HAL_UART_Transmit(&huart2, &RxBuffer2, sizeof(uint8_t), 100);
//    HAL_UART_Receive_IT(&huart2, &RxBuffer2, 1);

    HAL_UART_Transmit(&huart2, &RxBuffer2, sizeof(uint8_t), 100);
    static uint8_t flag, i, receive[BUFLEN];
    if(RxBuffer2 == '[') flag = 1; // �????????始采集标记位
    if(RxBuffer2 == ']') flag = 2; //结束采集标记

    if(flag == 1){ //采集数据
      receive[i++] = RxBuffer2;
      if(receive[i] == '['){
        i--;
      }
      if(i == 10){
        memset(receive, 0, sizeof(uint8_t) * BUFLEN);
        flag = 0;
      }
    }
    if(flag==2){ //分析数据
      HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin); //翻转LED
      //数据更新
      control[0] = receive[1];
      control[1] = receive[2];
      if(usartCnt++ == 60000) usartCnt=0;

      control_motor();
      // 重置数据
      flag = 0;
      i = 0;
      memset(receive, 0, sizeof(uint8_t) * BUFLEN);
    }
    HAL_UART_Receive_IT(&huart2, &RxBuffer2, 1);
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
