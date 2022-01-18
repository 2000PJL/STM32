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
#include "usart.h"
extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN 0 */
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//	
//  	 printf("接收信息为%i.\r\n",str_rd);
//	
//	if (huart->Instance==USART1){
//		HAL_UART_Receive_IT(&huart1,&str_rd,sizeof(str_rd));		
//		if (str_rd==str_ledc1){
//			LED1_ON();
//			LED2_ON();
//			BEEP_OFF();
//			HAL_UART_Transmit(&huart1,str_led1,sizeof(str_led1),1000);		
//		}else if (str_rd==str_ledc2){
//			LED1_OFF();
//			LED2_OFF();
//			BEEP_OFF();
//			HAL_UART_Transmit(&huart1,str_led2,sizeof(str_led2),1000);
//		}else if (str_rd==str_beepc1){
//			LED1_OFF();
//			LED2_OFF();
//			BEEP_ON();
//			HAL_UART_Transmit(&huart1,str_beep1,sizeof(str_beep1),1000);
//		}else if (str_rd==str_beepc2){
//			LED1_OFF();
//			LED2_OFF();
//			BEEP_OFF();
//			HAL_UART_Transmit(&huart1,str_beep2,sizeof(str_beep2),1000);
//		}
//	}
//}




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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LED0_Pin|LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, BEEP_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PEPin PEPin */
  GPIO_InitStruct.Pin = LED0_Pin|LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PGPin PGPin */
  GPIO_InitStruct.Pin = BEEP_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}




/* USER CODE BEGIN 2 */



/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
