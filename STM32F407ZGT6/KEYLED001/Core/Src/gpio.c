/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */
//#define LED0_ON() HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,RESET)
//#define LED0_OFF() HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,SET)
//#define LED1_ON() HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,RESET)
//#define LED1_OFF() HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,SET)
//#define LED2_ON() HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,RESET)
//#define LED2_OFF() HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,SET)
//#define BEEP_ON() HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,SET)
//#define BEEP_OFF() HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,RESET)



/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LED0_Pin|LED1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);

   /*Configure GPIO pins : PEPin PEPin */
  GPIO_InitStruct.Pin = LED0_Pin|LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PFPin PFPin PFPin PFPin */
  GPIO_InitStruct.Pin = KEY3_Pin|KEY2_Pin|KEY1_Pin|KEY0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PGPin PGPin */
  GPIO_InitStruct.Pin = BEEP_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 2 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	switch(GPIO_Pin){
		case KEY0_Pin:
			LED0_ON();
		  LED1_OFF();
		  LED2_OFF();
		  BEEP_OFF();
		printf("µ„¡¡LED0\r\n");
			break;
		case KEY1_Pin:
			LED0_OFF();
		  LED1_ON();
		  LED2_OFF();
		  BEEP_OFF();
		printf("µ„¡¡LED1\r\n");
			break;
		case KEY2_Pin:
			LED0_OFF();
		  LED1_OFF();
		  LED2_ON();
		  BEEP_OFF();
		printf("µ„¡¡LED2\r\n");
			break;
		case KEY3_Pin:
			LED0_OFF();
		  LED1_OFF();
		  LED2_OFF();
		  BEEP_ON();
		printf("∑‰√˘∆˜œÏ\r\n");
			break;}
		}


/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
