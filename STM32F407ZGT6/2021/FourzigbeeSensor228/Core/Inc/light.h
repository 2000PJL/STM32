/*
 * @Description: 
 * @Version: 2.0
 * @Autor: 葛璐豪
 * @Date: 2020-12-05 12:30:48
 * @LastEditors: Seven
 * @LastEditTime: 2020-12-27 12:33:28
 */
#ifndef _LIGHT_H_
#define _LIGHT_H_
#include "adc.h"
/* 所选引脚对应的ad通道 */
#define LIGHT_ADC hadc1  
/* 获取0-255之间的光照强度 */
uint16_t get_light(void);
#endif

