#include "wifi.h"

char name[] = "dbtest";
char password[] = "liuyang7303";
char address[] = "192.168.137.1";
int port = 3050;

uint8_t wifi_count = 0;
uint8_t esp8266_step = 0;//配置步骤编号
uint8_t wifi_rxbuf[Wifi_BufSize];
uint8_t rx_temp;

void ESP8266_Station_Config(void)
{	
	char client[100];
	char server[100];
	HAL_UART_Receive_IT(&wifiUsart, &rx_temp, 1);
	
	sprintf(client, "AT+CWJAP=\"%s\",\"%s\"\r\n", name, password);
	sprintf(server, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", address, port);
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
			printf("step1-> config as station mode!\r\n");
			if(Wifi_SendCmd("AT+CWMODE=1\r\n", 4000))
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
			printf("step3-> connect router!\r\n");
			if(Wifi_SendCmd(client, 8000))
			{
				esp8266_step++;
				printf("connection successful!\r\n");
			}
		}
		else if(esp8266_step == 4)
		{
			printf("step4-> start single route mode!\r\n");
			if(Wifi_SendCmd("AT+CIPMUX=0\r\n", 2000))
			{
				esp8266_step++;
				printf("start successful!\r\n");
			}
		}
		else if(esp8266_step == 5)
		{
			printf("step5-> connect server!\r\n");
			if(Wifi_SendCmd(server, 6000))
			{
				esp8266_step++;
				printf("connection successful!\r\n");
			}
		}
		else if(esp8266_step == 6)
		{
			printf("step6-> config as transparent transmission!\r\n");
			if(Wifi_SendCmd("AT+CIPMODE=1\r\n", 4000))
			{
				esp8266_step++;
				printf("configuration successful!\r\n");
			}
		}
		else if(esp8266_step == 7)
		{
			printf("step7-> start transparent transmission...\r\n");
			if(Wifi_SendCmd("AT+CIPSEND\r\n", 4000))
			{
				esp8266_step++;
				printf("successful!\r\n");
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
	HAL_GPIO_WritePin(WIFI_RST_GPIO_Port, WIFI_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(WIFI_RST_GPIO_Port, WIFI_RST_Pin, GPIO_PIN_SET);
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
	if(rx_temp != '\0')
	{
		wifi_rxbuf[wifi_count++] = rx_temp;
	}

	HAL_UART_Receive_IT(&wifiUsart, &rx_temp, 1);
}

