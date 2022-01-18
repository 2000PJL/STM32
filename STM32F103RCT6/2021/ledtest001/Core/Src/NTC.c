#include "NTC.h"

int NTC_temp(ADC_HandleTypeDef *ADC_channel)
{
	int temp,ADC_temp;
	uint32_t NTC_Value;
	
	  HAL_ADC_Start(ADC_channel);//启动ADC转换
		HAL_ADC_PollForConversion(&hadc1, 50);//表示等待转换完成，第二个参数表示超时时间，单位ms
		if(HAL_IS_BIT_SET(HAL_ADC_GetState(ADC_channel), HAL_ADC_STATE_REG_EOC))//获取ADC状态。判断转换完成标志位是否设置
		{
			ADC_temp=HAL_ADC_GetValue(ADC_channel);//读取ADC转换数据
			NTC_Value = ADC_temp*330/4096;
		}
		HAL_Delay(1000);
	temp=(int)100*NTC_Value;
	
	return temp;
}
