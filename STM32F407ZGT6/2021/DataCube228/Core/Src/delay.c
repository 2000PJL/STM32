/*
 * delay.c
 *
 *  Created on: Sep 27, 2021
 *      Author: TanXY
 */

#include "delay.h"
#include "time.h"

void delay_us(uint16_t us)
{
	uint16_t counter = 0xffff - us - 5;

	HAL_TIM_Base_Start(&DELAY_TIME);
	__HAL_TIM_SetCounter(&DELAY_TIME, counter);

	while (counter < 0xffff - 5)
	{
		counter = __HAL_TIM_GetCounter(&DELAY_TIME);
	}

	HAL_TIM_Base_Stop(&DELAY_TIME);
}

void delay_ms(uint16_t ms)
{
	while (ms--)
		delay_us(1000);
}
