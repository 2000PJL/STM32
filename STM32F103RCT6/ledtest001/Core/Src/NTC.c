#include "NTC.h"

int NTC_temp(ADC_HandleTypeDef *ADC_channel)
{
	int temp,ADC_temp;
	uint32_t NTC_Value;
	
	  HAL_ADC_Start(ADC_channel);//����ADCת��
		HAL_ADC_PollForConversion(&hadc1, 50);//��ʾ�ȴ�ת����ɣ��ڶ���������ʾ��ʱʱ�䣬��λms
		if(HAL_IS_BIT_SET(HAL_ADC_GetState(ADC_channel), HAL_ADC_STATE_REG_EOC))//��ȡADC״̬���ж�ת����ɱ�־λ�Ƿ�����
		{
			ADC_temp=HAL_ADC_GetValue(ADC_channel);//��ȡADCת������
			NTC_Value = ADC_temp*330/4096;
		}
		HAL_Delay(1000);
	temp=(int)100*NTC_Value;
	
	return temp;
}
