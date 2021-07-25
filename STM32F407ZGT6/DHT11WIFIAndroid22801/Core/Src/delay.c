#include "delay.h"
#include "time.h"
#include "main.h"


void Delay_ms(uint16_t ms){
	while(ms--)
   Delay_us(1000);
}

uint8_t count=0;
void Delay_us(uint16_t time)
{
     count=time;   
     HAL_TIM_Base_Start_IT(&htim6);		//开启定时器
	   __HAL_TIM_SetCounter(&htim6,65535-time); //设置计数值	
    while(__HAL_TIM_GetCounter(&DELAY_TIME)<=65530)
		{
		}			//判断计数值是否耗尽
     HAL_TIM_Base_Stop_IT(&htim6);		//关闭定时器
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM6)
	{
    	__HAL_TIM_SetCounter(&htim6,65535-count); //设置计数值
	}
}
