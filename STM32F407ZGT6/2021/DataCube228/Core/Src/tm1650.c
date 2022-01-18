/*
 * TM1650.c
 *
 *  Created on: Sep 26, 2021
 *      Author: TanXY
 */

#include "tm1650.h"
#include "usart.h"
//数码管显示0~9对应的值
const uint8_t NUM[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};


void TM1650_Wr_RAM(uint8_t Address, uint8_t Data)
{
    IIC_Start();
    IIC_Send_Byte(Address);
    IIC_Wait_Ack();
    IIC_Send_Byte(Data);
    IIC_Wait_Ack();
    IIC_Stop();
	
}

// 打开数码管显示
void open_display()
{
	// IIC GPIO初始化
	IIC_Init();
	TM1650_Wr_RAM(0x48, 0x71);
}

// 写入数字
void set_number(int num)
{
	TM1650_Wr_RAM(0x68, NUM[num / 1000]);
	// 加上小数点
	TM1650_Wr_RAM(0x6A, NUM[num % 1000 / 100] | 0X80);
	TM1650_Wr_RAM(0x6C, NUM[num % 100 / 10]);
	TM1650_Wr_RAM(0x6E, NUM[num % 10]);
}
