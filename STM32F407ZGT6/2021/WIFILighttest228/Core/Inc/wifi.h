#ifndef _WIFI_H_
#define _WIFI_H_

#include "main.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"

#define wifiUsart huart4
#define Wifi_BufSize 200

void ESP8266_Station_Config(void);
void Wifi_SendData(char *buf);
void Wifi_Interrupt(void);
uint8_t Wifi_SendCmd(char *command, uint16_t timeout);
uint8_t Wifi_Reset(uint16_t timeout);

#endif
