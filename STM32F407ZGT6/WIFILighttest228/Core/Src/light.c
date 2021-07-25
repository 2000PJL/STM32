/*
 * @Description: 
 * @Version: 2.0
 * @Autor: ��贺�
 * @Date: 2020-12-05 12:30:48
 * @LastEditors: Seven
 * @LastEditTime: 2020-12-27 12:32:41
 */
#include "light.h"
uint8_t get_light(void)
{
		int AD_Light = 0,mide_temp = 0;
	
		HAL_ADC_Start(&LIGHT_ADC);//����ADCת��
		HAL_ADC_PollForConversion(&LIGHT_ADC, 50);//��ʾ�ȴ�ת����ɣ��ڶ���������ʾ��ʱʱ�䣬��λms
		if(HAL_IS_BIT_SET(HAL_ADC_GetState(&LIGHT_ADC), HAL_ADC_STATE_REG_EOC))//��ȡADC״̬���ж�ת����ɱ�־λ�Ƿ�����
		{
			AD_Light=HAL_ADC_GetValue(&LIGHT_ADC);//��ȡADCת������
			mide_temp = AD_Light*255/4096;
			AD_Light = 255-mide_temp;
		}
		return AD_Light;
}
