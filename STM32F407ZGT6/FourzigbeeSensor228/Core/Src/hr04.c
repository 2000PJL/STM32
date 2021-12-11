/* 包含头文件 ----------------------------------------------------------------*/
#include "hr04.h"
#include "gpio.h"
#include "delay.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 读取引脚的状态
  * 输入参数：无
  * 返 回 值: 距离
  * 说    明：无。
  */
double Hcsr04_StateRead(void)
{
	uint32_t distance = 0;
	
	HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_SET);
	
	delay_us(10);
	
	HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_RESET);
	
	while(HAL_GPIO_ReadPin(Echo_GPIO_Port,Echo_Pin)==GPIO_PIN_RESET);
	
	while(HAL_GPIO_ReadPin(Echo_GPIO_Port,Echo_Pin)==GPIO_PIN_SET)
	{
		delay_us(10);
		distance++;
	}
	
	return (distance/2)*0.34;
}

