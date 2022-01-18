#include "ESP8266.h"

short wifi_count = 0;
uint8_t esp8266_step = 0;//配置步骤编号
uint8_t wifi_rxbuf[Wifi_BufSize];
uint8_t rx_temp;
extern osMessageQId WifiRecvQueueHandle;

unsigned char AP_NAME[] = "HONOR 9X Pro";
unsigned char AP_MI[] = "1985343015";
unsigned char server[100]={0};

void ESP8266_Station_Config(void)
{	
	HAL_UART_Receive_IT(&wifiUsart, &rx_temp, 1);

	while(1)
	{
		if(esp8266_step == 0)
		{
			printf("wifi reset...\r\n");
			if(Wifi_Reset(5000))
			{
				printf("wifi reset successful!\r\n");
				esp8266_step++;
			}
		}
		else if(esp8266_step == 1)
		{
			printf("step1-> config as softAP!\r\n");
			if(Wifi_SendCmd("AT+CWMODE=2\r\n", 4000))
			{
				esp8266_step++;
				printf("configuration successful!\r\n");
			}
		}
		else if(esp8266_step == 2)
		{
			printf("step2-> restart!\r\n");
			if(Wifi_SendCmd("AT+RST\r\n", 5000))
			{
				esp8266_step++;
				printf("configuration successful!\r\n");
				HAL_Delay(1000);
			}
		}
		else if(esp8266_step == 3)
		{
			printf("step3-> connect mutiple connect!\r\n");
			if(Wifi_SendCmd("AT+CIPMUX=1\r\n", 4000))
			{
				esp8266_step++;
				printf("connection successful!\r\n");
				HAL_Delay(1000);
			}
		}
		else if(esp8266_step == 4)
		{
			printf("step4-> set softAP\r\n");
			if(Wifi_SendCmd("AT+CIPAP_DEF=\"192.168.8.1\",\"192.168.8.1\",\"255.255.255.0\"\r\n", 4000))
			{
				esp8266_step++;
				printf("start successful!\r\n");
					HAL_Delay(1000);
			}
		}
		else if(esp8266_step == 5)
		{
			printf("step5-> set softAP!\r\n");
			sprintf((char *)server,"AT+CWSAP=\"%s\",\"%s\",5,3\r\n",AP_NAME,AP_MI);
			if(Wifi_SendCmd((char *)server, 4000))
			{
				esp8266_step++;
				printf("connection successful!\r\n");
					HAL_Delay(1000);
			}
		}
		else if(esp8266_step == 6)
		{
			printf("step6-> set as server!\r\n");
			if(Wifi_SendCmd("AT+CIPSERVER=1,5000\r\n", 5000))
			{
				esp8266_step++;
				printf("set successful\r\n");
					HAL_Delay(1000);
				break;
			}
		}
	}
}

/**
 * @author: TanXY
 * @description: 发送wifi命令
 * @param: command：命令字符串 timeout：超时时间
 * @return: 0：命令接收失败  1：命令接收成功
 */
uint8_t Wifi_SendCmd(char *command, uint16_t timeout)
{
	wifi_count = 0;
	memset(wifi_rxbuf, 0, Wifi_BufSize);
	if(HAL_UART_Transmit(&wifiUsart, (uint8_t *)command, strlen(command), 1000) == HAL_OK)
	{
		while(timeout)
		{
			HAL_Delay(50);
			if(strstr((char *)wifi_rxbuf, "OK"))
				return 1;
			if(timeout % 1000 == 0)
				printf("timeout->%d\r\n", timeout/1000);
			timeout -= 50;
		}
		printf("timeout, try again!\r\n");
		return 0;
	}
	return 0;
}

/**
 * @author: TanXY
 * @description: wifi复位，使某些配置生效，不需要掉电重启
 * @param: timeout 等待超时时间
 * @return: 0-复位失败 1-复位成功
 */
uint8_t Wifi_Reset(uint16_t timeout)
{
	HAL_GPIO_WritePin(Wifi_Reset_GPIO_Port, Wifi_Reset_Pin, GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(Wifi_Reset_GPIO_Port, Wifi_Reset_Pin, GPIO_PIN_SET);
	while(timeout)
	{
		HAL_Delay(50);
		if(strstr((char *)wifi_rxbuf,"ready"))
			return 1;
		timeout -= 50;
		if(timeout % 1000 == 0)
			printf("timeout->%d\r\n",timeout / 1000);
	}
	
	printf("wait timeout, try again!\r\n");
	return 0;
}

void Wifi_SendData(char *buf)
{
	HAL_UART_Transmit(&wifiUsart, (unsigned char *)buf, strlen(buf), 1000);
}

void Wifi_Interrupt(void)
{	
	 if (esp8266_step <= 6)
	{
		if (rx_temp != '\0')
		{
			wifi_rxbuf[wifi_count++] = rx_temp;
		}
		else
		{
			wifi_count = 0;
			memset(wifi_rxbuf, 0, sizeof(wifi_rxbuf));
		}
	}
	else
	{
		if (rx_temp == '#')
		{
			wifi_count = 1;
			memset(wifi_rxbuf, 0, sizeof(wifi_rxbuf));
		}
		else if (rx_temp == '$')
		{
//			printf("%s\r\n", wifi_rxbuf);
			wifi_count = -1;
        
		}
		else if ((wifi_count > 0) && (wifi_count < Wifi_BufSize))
		{
			wifi_rxbuf[wifi_count - 1] = rx_temp;
			wifi_count++;
		}
	}
	HAL_UART_Receive_IT(&wifiUsart, &rx_temp, 1);
}

