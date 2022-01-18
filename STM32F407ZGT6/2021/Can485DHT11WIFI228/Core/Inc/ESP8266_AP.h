#ifndef _ESP8266_AP_H_
#define _ESP8266_AP_H_

#include "main.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "cmsis_os.h"

#define wifiUsart huart4
#define Wifi_BufSize 256

void ESP8266_Station_Config(void);
void Wifi_SendData(char *buf);
void Wifi_Interrupt(void);
uint8_t Wifi_SendCmd(char *command, uint16_t timeout);
uint8_t Wifi_Reset(uint16_t timeout);

#endif
