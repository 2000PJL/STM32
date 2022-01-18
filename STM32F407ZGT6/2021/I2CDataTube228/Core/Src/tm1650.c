/*
 * TM1650.c
 *
 *  Created on: Sep 26, 2021
 *      Author: TanXY
 */

#include "tm1650.h"

//�������ʾ0~9��Ӧ��ֵ
const uint8_t NUM[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
0x77,0x7f,0x39,0x3f,0x79,0x71};



void TM1650_Wr_RAM(uint8_t Address, uint8_t Data)
{
    IIC_Start();
    IIC_Send_Byte(Address);
    IIC_Wait_Ack();
    IIC_Send_Byte(Data);
    IIC_Wait_Ack();
    IIC_Stop();
}

// ���������ʾ
void open_display()
{
	// IIC GPIO��ʼ��
	IIC_Init();
	TM1650_Wr_RAM(0x48, 0x71);
}

// д������
void set_number(char A1,char A2,char A3,char A4)
{
//	TM1650_Wr_RAM(0x68, NUM[num / 1000]);
//	// ����С����
//	TM1650_Wr_RAM(0x6A, NUM[num % 1000 / 100] | 0X80);
//	TM1650_Wr_RAM(0x6C, NUM[num % 100 / 10]);
//	TM1650_Wr_RAM(0x6E, NUM[num % 10]);
	TM1650_Wr_RAM(0x68, NUM[A1]);
	TM1650_Wr_RAM(0x6A, NUM[A2]|0X80);
	TM1650_Wr_RAM(0x6C, NUM[A3]);
	TM1650_Wr_RAM(0x6E, NUM[A4]);
}
