#ifndef _CC2530_H_
#define _CC2530_H_

#include "usart.h"
#include "string.h"
#include "stdio.h"

#define cc2530Usart huart6
#define cc2530Interrupt USART6

void cc2530_config(void);
void cc2530_sendMessage(char *msg);
void cc2530_getMessage(char *msg);
void get_cc2530Message(char *msg);
void cc2530_interrupt(void);

#endif
