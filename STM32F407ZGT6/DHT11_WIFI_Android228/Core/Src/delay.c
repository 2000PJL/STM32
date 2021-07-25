#include "delay.h"
#include "time.h"

void Delay_us(uint16_t us)
{	
	uint16_t counter=0xffff-us-5;

	HAL_TIM_Base_Start(&DELAY_TIME);
	__HAL_TIM_SetCounter(&DELAY_TIME, counter);

	while(counter<0xffff-5)
	{
			counter=__HAL_TIM_GetCounter(&DELAY_TIME);
	}

	HAL_TIM_Base_Stop(&DELAY_TIME);
}

void Delay_ms(uint16_t ms)
{
	while(ms--)
    Delay_us(1000);
}

