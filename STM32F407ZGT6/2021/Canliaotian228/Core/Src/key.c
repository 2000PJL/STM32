#include "key.h"

int key_value = -1;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == KEY0_Pin)
	{
		//key_value = 0;
		HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
	}
	else if(GPIO_Pin == KEY1_Pin) 
	{
		//key_value = 1;
		HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
	}
	else if(GPIO_Pin == KEY2_Pin) 
	{
		key_value = 2;
	}
	else if(GPIO_Pin == KEY3_Pin) 
	{
		key_value = 3;
	}else if(GPIO_Pin == GPIO_PIN_15)
	{
		key_value = 4;
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
