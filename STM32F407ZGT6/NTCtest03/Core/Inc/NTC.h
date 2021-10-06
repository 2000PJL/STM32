#ifndef __NTC_H
#define __NTC_H

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t


#include "adc.h"
#include "math.h"
#include "stdlib.h"

u16 Get_ADC(ADC_HandleTypeDef *ADC_channel);
uint16_t Get_Res(ADC_HandleTypeDef *ADC_channel);
u8 Get_Temp(ADC_HandleTypeDef *ADC_channel);
double Get_Temp_Precise(ADC_HandleTypeDef *ADC_channel);//精确到小数点后1位
double Get_Temp_PJL(ADC_HandleTypeDef *ADC_channel);
#endif

