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
#include "stdio.h"
//#define LED0_ON() HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,RESET)
//#define LED0_OFF() HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,SET)
//#define LED1_ON() HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,RESET)
//#define LED1_OFF() HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,SET)
//#define LED2_ON() HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,RESET)
//#define LED2_OFF() HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,SET)
//#define BEEP_ON() HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,SET)
//#define BEEP_OFF() HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,RESET)
#define LED0_ON() HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin,GPIO_PIN_RESET)
#define LED0_OFF() HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin,GPIO_PIN_SET)
#define LED1_ON() HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,GPIO_PIN_RESET)
#define LED1_OFF() HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,GPIO_PIN_SET)
#define LED2_ON() HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,GPIO_PIN_RESET)
#define LED2_OFF() HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,GPIO_PIN_SET)
#define BEEP_ON() HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin,GPIO_PIN_SET)
#define BEEP_OFF() HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin,GPIO_PIN_RESET)


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
