/*
 * @Description: 
 * @Version: 2.0
 * @Autor: ��贺�
 * @Date: 2020-12-27 11:55:53
 * @LastEditors: Seven
 * @LastEditTime: 2020-12-27 12:33:18
 */
#ifndef _DELAY_H_
#define _DELAY_H_
#include "tim.h"
#include "main.h"
/* ��ʱ���õ�ͨ�ö�ʱ�� ��֤��Ƶ��Ϊ1Mhz*/
#define DELAY_TIME htim6
void Delay_ms(uint16_t ms);
/* ��� 65535us û�������� ��̫��ȷ �����𳬹������ */
void Delay_us(uint16_t us);


#endif
