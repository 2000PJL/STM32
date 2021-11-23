#include "key.h"


int key_value = -1;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == KEY0_Pin)
	{
		LED0_ON();
		key_value=0;
	}
	else if(GPIO_Pin == KEY1_Pin) 
	{
		LED0_OFF();
		key_value=1;
	}else if(GPIO_Pin == KEY2_Pin) 
	{
		LED1_ON();
		key_value=2;
	}else if(GPIO_Pin == KEY3_Pin) 
	{
		LED1_OFF();
		key_value=3;
	}
}

int get_keyValue(void)
{
	return key_value;
}

void keyValue_Reset(void)
{
	key_value = -1;
}
