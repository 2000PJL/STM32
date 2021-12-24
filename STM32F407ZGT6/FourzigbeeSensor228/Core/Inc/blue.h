/*
 * @Description: 
 * @Version: 2.0
 * @Autor: 葛璐豪
 * @Date: 2020-12-05 12:30:53
 * @LastEditors: Seven
 * @LastEditTime: 2020-12-26 16:55:09
 */
#ifndef _WIFI_H_
#define _WIFI_H_
#include "usart.h"
#include "string.h"
#include "stdio.h"
/* 蓝牙 对应的串口 */
#define blueUsart huart3
#define blueInterrupt USART3

void blue_send(char *cmd);
void BlueConfig_UART_Init(void);
void bluetooth_interrupt(void);
void bluetooth_interrupt2(void);
void blue_Init(void);

uint8_t blue_config(void);
uint8_t blue_sendCmd(char *cmd,int timeout);

#endif

