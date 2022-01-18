#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//�ⲿ�ж� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/06  
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									 
////////////////////////////////////////////////////////////////////////////////// 	  


//�ⲿ�жϳ�ʼ������
//��ʼ��PA0,PC5,PA15Ϊ�ж�����.
void EXTI_Init(void)
{
	KEY_Init();
	Ex_NVIC_Config(GPIO_A,0,RTIR); 		//�����ش���
	Ex_NVIC_Config(GPIO_C,5,FTIR);		//�½��ش���//���ڲ�����
	Ex_NVIC_Config(GPIO_A,15,FTIR);		//�½��ش���//��ͣ

	MY_NVIC_Init(2,2,EXTI0_IRQn,2);    	//��ռ2�������ȼ�2����2
	MY_NVIC_Init(2,1,EXTI9_5_IRQn,2);  	//��ռ2�������ȼ�1����2
	MY_NVIC_Init(2,0,EXTI15_10_IRQn,2);	//��ռ2�������ȼ�0����2	   
}












