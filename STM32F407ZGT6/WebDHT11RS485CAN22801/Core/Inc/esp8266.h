/*
 * @Description: 
 * @Version: 2.0
 * @Autor: 葛璐豪
 * @Date: 2020-12-05 12:30:53
 * @LastEditors: Seven
 * @LastEditTime: 2020-12-05 12:56:48
 */
#ifndef _ESP8266_H_
#define _ESP8266_H_
#include "usart.h"
#include "string.h"
#include "stdio.h"

#define wifiUsart huart4
#define wifiInterrupt UART4
int get_wifiMessage(char *message);
void wifi_sendMessage(char *cmd);
void wifi_interrupt(void);
uint8_t wifi_config(void);
uint8_t wifi_sendCmd(char *cmd,int timeout);
uint8_t wifi_reset(int timeout);
#endif
