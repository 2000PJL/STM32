#include "NTC.h"

//�ο�����;https://www.cnblogs.com/ct20150811/p/11276595.html

/**
  ******************************************************************************
  * �ļ�����: NTC.c 
  * ��    ��: ����
  * ��    ��: V1.0
  * ��д����: 2021/10/06
  * ��    ��: ��ȡ����������¶�
  ******************************************************************************
  * ˵����
  *       ͨ����ѹ����������������һ��10K�ĵ��贮����Ȼ�����3.3V�ĵ�Դ��
	* ͨ��ADCȥ��ȡ�����������˵ĵ�ѹ��Ȼ�����������������ֵ��ͨ����ֵ-�¶ȱ�
	* ȥ��ѯ�¶ȡ�
	* ע�⣺
	        ����������������ϵ��Ϊ3950�ĸ��¶�ϵ��10K��������
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

u16 Get_Res(ADC_HandleTypeDef *ADC_channel)//��ȡNTC�ĵ���ֵ
{
	u16 adc_value=Get_ADC(ADC_channel);
	u16 temp;
	u32 R_Value,V_Value;
	V_Value=(3300*adc_value)/4095;
	R_Value=(10000*V_Value)/(3300-V_Value);
	temp=R_Value;
	return temp;
}

u16 Get_ADC(ADC_HandleTypeDef *ADC_channel)//��ȡADCͨ����ADCֵ
{
	  int ADC_temp=0;
	  HAL_ADC_Start(ADC_channel);//����ADCת��
		HAL_ADC_PollForConversion(&hadc1, 50);//��ʾ�ȴ�ת����ɣ��ڶ���������ʾ��ʱʱ�䣬��λms
		if(HAL_IS_BIT_SET(HAL_ADC_GetState(ADC_channel), HAL_ADC_STATE_REG_EOC))//��ȡADC״̬���ж�ת����ɱ�־λ�Ƿ�����
		{
			ADC_temp=HAL_ADC_GetValue(ADC_channel);//��ȡADCת������
		}
		return ADC_temp;
}

u8 Get_Temp(ADC_HandleTypeDef *ADC_channel)//��ȡ�¶ȣ�������������
{
	u16 adc_value=Get_ADC(ADC_channel);
	u8 low=0,high=131,mid;//value,

	while(low<=high){
		mid=(low+high)/2;
		if(RT_Table[mid]==adc_value) break;			
				
		if(adc_value<RT_Table[mid]) low=mid+1;//ֵС����ֵ,����λ�ұ�,low �ƶ�					
		
		if(adc_value>RT_Table[mid]) high=mid-1;	//ֵ������ֵ,����λ���,High �ƶ�	
				
	}
	if(adc_value<RT_Table[mid]) mid=mid+1;//

	return mid-20;
}

double Get_Temp_Precise(ADC_HandleTypeDef *ADC_channel)//��ȡ�¶ȣ�ͨ�������-�¶ȱ���
{
  	u16 adc_value=Get_ADC(ADC_channel);
    float temp_decimal;
    u32 TpValue;
	  u8 low=0,high=131,mid;//value,

	while(low<=high){
		mid=(low+high)/2;
		//�½�����
		if(RT_Table[mid]==adc_value) break;					
		if(adc_value<RT_Table[mid]) low=mid+1;//ֵС����ֵ,����λ�ұ�,low �ƶ�					
		if(adc_value>RT_Table[mid]) high=mid-1;	//ֵ������ֵ,����λ���,High �ƶ�	
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
 *     ��ȡ�߾����¶ȣ���Ҫ�õ��������㣬����
 * ����Ҫ�õ��Ϸ����¶�-��ֵ�����ܻ���һ����
 * ���ȶ�ʧ��
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
	double B_Value=3950.0,//NTCϵ��
		     R0_Value=10000.0;//��������25���϶��µ���ֵ
	R_Value=(adc_value*R0_Value/(4096.0-adc_value));
	T_Value=1.0/((1.0/298.15)+((log(R_Value/R0_Value)/log(2.71828))/B_Value));
	return T_Value-273.15;
}


