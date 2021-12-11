/*
 * @Description: 
 * @Version: 2.0
 * @Autor: 葛璐豪
 * @Date: 2020-12-05 12:30:48
 * @LastEditors: Seven
 * @LastEditTime: 2020-12-27 12:32:41
 */
#include "light.h"
uint16_t get_light(void)
{
		int AD_Light = 0,mide_temp = 0;
	
		HAL_ADC_Start(&LIGHT_ADC);//启动ADC转换
		HAL_ADC_PollForConversion(&LIGHT_ADC, 50);//表示等待转换完成，第二个参数表示超时时间，单位ms
		if(HAL_IS_BIT_SET(HAL_ADC_GetState(&LIGHT_ADC), HAL_ADC_STATE_REG_EOC))//获取ADC状态。判断转换完成标志位是否设置
		{
			AD_Light=HAL_ADC_GetValue(&LIGHT_ADC);//读取ADC转换数据
			mide_temp = AD_Light*3300/4096;
			AD_Light = 3300-mide_temp;
		}
		return AD_Light;
}
