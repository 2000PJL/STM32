#include "NTC.h"

//参考链接;https://www.cnblogs.com/ct20150811/p/11276595.html

/**
  ******************************************************************************
  * 文件名称: NTC.c 
  * 作    者: 彭俊龙
  * 版    本: V1.0
  * 编写日期: 2021/10/06
  * 功    能: 获取热敏电阻的温度
  ******************************************************************************
  * 说明：
  *       通过分压法，将热敏电阻与一个10K的电阻串联，然后接上3.3V的电源，
	* 通过ADC去获取热敏电阻两端的电压，然后计算出热敏电阻的阻值，通过阻值-温度表
	* 去查询温度。
	* 注意：
	        该驱动适用于热敏系数为3950的负温度系数10K热敏电阻
  ******************************************************************************
  */
uint32_t RT_Table[131]={
0xE9D,0xE89,0xE73,0xE5D,0xE46,0xE2E,0xE15,0xDFB,0xDE0,0xDC5,//-20--11
0xDA8,0xD8A,0xD6B,0xD4C,0xD2B,0xD0A,0xCE7,0xCC4,0xCA0,0xC7B,//-10--1
0xC55,0xC2E,0xC07,0xBDE,0xBB6,0xB8C,0xB62,0xB37,0xB0B,0xADF,//0-9
0xAB3,0xA86,0xA58,0xA2B,0x9FD,0x9CF,0x9A0,0x972,0x943,0x915,//10-19
0x8E6,0x8B8,0x88A,0x85C,0x82E,0x800,0x7D3,0x7A6,0x779,0x74D,//20-29
0x721,0x6F6,0x6CB,0x6A1,0x677,0x64E,0x626,0x5FE,0x5D7,0x5B1,//30-39
0x58B,0x566,0x542,0x51E,0x4FB,0x4D9,0x4B8,0x497,0x477,0x458,
0x43A,0x41C,0x3FF,0x3E3,0x3C7,0x3AC,0x392,0x378,0x35F,0x347,
0x330,0x319,0x302,0x2ED,0x2D8,0x2C3,0x2AF,0x29C,0x289,0x277,
0x265,0x254,0x243,0x233,0x223,0x214,0x205,0x1F6,0x1E8,0x1DB,
0x1CE,0x1C1,0x1B4,0x1A8,0x19D,0x191,0x186,0x17C,0x171,0x167,
0x15E,0x154,0x14B,0x142,0x13A,0x131,0x129,0x121,0x11A,0x112,
0x10B,0x104,0xFD,0xF7,0xF0,0xEA,0xE4,0xDE,0xD9,0xD3,0xCE,
};

u16 Get_Res(ADC_HandleTypeDef *ADC_channel)//获取NTC的电阻值
{
	u16 adc_value=Get_ADC(ADC_channel);
	u16 temp;
	u32 R_Value,V_Value;
	V_Value=(3300*adc_value)/4095;
	R_Value=(10000*V_Value)/(3300-V_Value);
	temp=R_Value;
	return temp;
}

u16 Get_ADC(ADC_HandleTypeDef *ADC_channel)//获取ADC通道的ADC值
{
	  int ADC_temp=0;
	  HAL_ADC_Start(ADC_channel);//启动ADC转换
		HAL_ADC_PollForConversion(&hadc1, 50);//表示等待转换完成，第二个参数表示超时时间，单位ms
		if(HAL_IS_BIT_SET(HAL_ADC_GetState(ADC_channel), HAL_ADC_STATE_REG_EOC))//获取ADC状态。判断转换完成标志位是否设置
		{
			ADC_temp=HAL_ADC_GetValue(ADC_channel);//读取ADC转换数据
		}
		return ADC_temp;
}

u8 Get_Temp(ADC_HandleTypeDef *ADC_channel)//获取温度，但是是整数的
{
	u16 adc_value=Get_ADC(ADC_channel);
	u8 low=0,high=131,mid;//value,

	while(low<=high){
		mid=(low+high)/2;
		if(RT_Table[mid]==adc_value) break;			
				
		if(adc_value<RT_Table[mid]) low=mid+1;//值小于中值,在中位右边,low 移动					
		
		if(adc_value>RT_Table[mid]) high=mid-1;	//值大于中值,在中位左边,High 移动	
				
	}
	if(adc_value<RT_Table[mid]) mid=mid+1;//

	return mid-20;
}

double Get_Temp_Precise(ADC_HandleTypeDef *ADC_channel)//获取温度，通过查电阻-温度表获得
{
  	u16 adc_value=Get_ADC(ADC_channel);
    float temp_decimal;
    u32 TpValue;
	  u8 low=0,high=131,mid;//value,

	while(low<=high){
		mid=(low+high)/2;
		//下降排序
		if(RT_Table[mid]==adc_value) break;					
		if(adc_value<RT_Table[mid]) low=mid+1;//值小于中值,在中位右边,low 移动					
		if(adc_value>RT_Table[mid]) high=mid-1;	//值大于中值,在中位左边,High 移动	
	}
	if(adc_value<RT_Table[mid]){ 
        temp_decimal=mid+((adc_value-RT_Table[mid+1])/(RT_Table[mid]-RT_Table[mid+1]));
    }else{
        temp_decimal=mid-1+((adc_value-RT_Table[mid])/(RT_Table[mid-1]-RT_Table[mid]));
    }
    
    TpValue=((temp_decimal-20)*10);
    return (TpValue/10.0);
}

/**********************************************
 *     获取高精度温度，需要用到大量计算，但是
 * 不需要用到上方的温度-阻值表，可能会有一定的
 * 精度丢失。
 * 
***********************************************/
double Get_Temp_PJL(ADC_HandleTypeDef *ADC_channel)
{
	u16 adc_value=0,adc_value1=0,train=0;
	for(;train<10;train++){
			 adc_value1=Get_ADC(ADC_channel);
		   adc_value=(adc_value+adc_value1);
		  // HAL_Delay(5);
	 }
	 adc_value=adc_value/10;
	double R_Value,T_Value;
	double B_Value=3950.0,//NTC系数
		     R0_Value=10000.0;//热敏电阻25摄氏度下的阻值
	R_Value=(adc_value*R0_Value/(4096.0-adc_value));
	T_Value=1.0/((1.0/298.15)+((log(R_Value/R0_Value)/log(2.71828))/B_Value));
	return T_Value-273.15;
}


