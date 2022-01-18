#include "key.h"

extern int cansend_flag;
int key_value = -1;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == KEY0_Pin)
	{
		LED0_OFF();
		cansend_flag=0;
	}
	else if(GPIO_Pin == KEY1_Pin) 
	{
		LED0_ON();
		cansend_flag=1;
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
