#ifndef _RS485_H_
#define _RS485_H_

#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "FreeRTOS.h"

#define rs485usart huart2

void reset_rxbuff(void);
void reset_toReceiveState(void);
int get_receiveState(void);
int get_toReceiveState(void);
int rs485_sendData(char *message,int timeout);
int get_message(char *message);
void rs485_receive_interrupt(void);
void init_rs485(void);
void reset_receiveState(void);


#endif

