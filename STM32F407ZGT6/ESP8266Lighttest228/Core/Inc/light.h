/*
 * @Description: 
 * @Version: 2.0
 * @Autor: ��贺�
 * @Date: 2020-12-05 12:30:48
 * @LastEditors: Seven
 * @LastEditTime: 2020-12-27 12:33:28
 */
#ifndef _LIGHT_H_
#define _LIGHT_H_
#include "adc.h"
/* ��ѡ���Ŷ�Ӧ��adͨ�� */
#define LIGHT_ADC hadc1  
/* ��ȡ0-255֮��Ĺ���ǿ�� */
uint8_t get_light(void);
#endif

