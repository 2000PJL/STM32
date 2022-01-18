/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
//#define LED1_ON() HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET)
//#define LED1_OFF() HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET)
//#define LED2_ON() HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET)
//#define LED2_OFF() HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET)
//#define LED3_ON() HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET)
//#define LED3_OFF() HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET)
//#define BEEP_ON() HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET)
//#define BEEP_OFF() HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET)

//uint8_t str_sd1[]="hello,my baby,这是我的第一次串口控制程序.\r\n";
//uint8_t str_led1[]="LED0,1 已经打开.\r\n";
//uint8_t str_led2[]="LED0,1 已经关闭.\r\n";
//uint8_t str_[]="LED0,1 OPEN.\r\n";
//uint8_t str_beep1[]="蜂鸣器已经打开\r\n";
//uint8_t str_beep2[]="蜂鸣器已经关闭\r\n";
//uint8_t str_ledc1=161;  //0xA1
//uint8_t str_ledc2=162;  //0XA2
//uint8_t str_beepc1=163;  //0XA3
//uint8_t str_beepc2=164;  //0XA4
//uint8_t str_rd=0;
//char buf[]="0";

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
