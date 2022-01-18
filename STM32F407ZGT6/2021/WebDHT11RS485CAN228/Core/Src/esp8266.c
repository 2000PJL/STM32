/*
 * @Description: 
 * @Version: 2.0
 * @Autor: 葛璐豪
 * @Date: 2020-12-05 12:30:48
 * @LastEditors: Seven
 * @LastEditTime: 2020-12-27 12:32:22
 */
#include "esp8266.h"
unsigned char WIFI_NAME[] = "Elsa";
unsigned char WIFI_MI[] = "41282620";

unsigned char IP[] = "192.168.137.1";
unsigned char PORT[] = "5000";
/* 中断接收所用变量 */
uint8_t wifi_state = 0;
#define rx_rxBUFF_SIZE 500
uint8_t rxBuffer = 0;
unsigned char rxCount = 0;  //计数器
char rxBuff[rx_rxBUFF_SIZE] = {0};   //缓冲区
int receiveWifi_flag = 0;  //0无消息 1 有消息
char serve[150] = {0};
char client[50] = {0};
int get_wifiMessage(char *message)
{
	//若有消息
	if(receiveWifi_flag==1) {
		strcpy(message,(char*)rxBuff);
		memset(rxBuff,0,rx_rxBUFF_SIZE);
		receiveWifi_flag = 0;
		return 1;
	} else {
		return 0;
	}
}
uint8_t wifi_config(void)
{
	wifi_state = 0;
	memset(serve,0,150);
	memset(client,0,50);
	HAL_UART_Receive_IT(&wifiUsart,&rxBuffer,1);

	if(wifi_reset(50))
	{
		printf("wifi 复位失败\r\n");
		return 1;	
	}
	else
	{
		printf("wifi 复位成功!!\r\n");
	}

	printf("(1) 模式配置中\r\n");
	if(wifi_sendCmd("AT+CWMODE=1",50))
	{
		printf("模式配置失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("模式配置成功!!\r\n");
	}
	
	printf("(2)复位配置中\r\n");
	if(wifi_sendCmd("AT+RST",50))
	{
		printf("复位配置失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("复位配置成功!!\r\n");
	}
	
	HAL_Delay(3000);
	printf("(3) 连接wifi中\r\n");
	printf("正在连接的wifi名=%s\r\n",WIFI_NAME);
	printf("正在连接的wifi密码= %s\r\n",WIFI_MI);
	
	sprintf(client,"AT+CWJAP=\"%s\",\"%s\"",WIFI_NAME,WIFI_MI);
	if(wifi_sendCmd(client,500))
	{
		printf("wifi连接失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("wifi连接成功!!\r\n");
	}
	
	printf("(4) 正在s_connection配置\r\n");
	if(wifi_sendCmd("AT+CIPMUX=0",50))
	{
		printf("s_connection配置失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("s_connection配置成功!!\r\n");
	}
	
	
	printf("(5) 正在OPEN_TI_MODE 配置\r\n");
	if(wifi_sendCmd("AT+CIPMODE=1",50))
	{
		printf("OPEN_TI_MODE 配置失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("OPEN_TI_MODE 配置成功!!\r\n");
	}
	
	printf("(6) TCP建立中\r\n");
	sprintf(serve,"AT+CIPSTART=\"TCP\",\"%s\",%s",IP,PORT);
	printf("send=%s\r\n",serve);
	if(wifi_sendCmd(serve,50))
	{
		printf("TCP建立失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("TCP建立成功!!\r\n");
	}
	
	printf("(7) 建立数据传输中\r\n");
	if(wifi_sendCmd("AT+CIPSEND",50))
	{
		printf("建立数据传输失败 重启\r\n");
		return 1;
	}
	else
	{
		printf("建立数据传输成功 可以发送数据了!!\r\n");
	}
	rxBuffer = 0;
	rxCount = 0;
	memset(rxBuff,0,rx_rxBUFF_SIZE);
	wifi_state = 1;
	return 0;
}
uint8_t wifi_reset(int timeout)
{
	HAL_GPIO_WritePin(WIFI_RESET_GPIO_Port,WIFI_RESET_Pin,GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(WIFI_RESET_GPIO_Port,WIFI_RESET_Pin,GPIO_PIN_SET);
	memset(rxBuff,0,rx_rxBUFF_SIZE);
	while(timeout--)
	{
		HAL_Delay(100);
		if(strstr(rxBuff,"ready"))
			break;
		printf("%d ",timeout);
	}
	printf("\r\n");
	if(timeout<=0) return 1;
	else return 0;
}
void wifi_sendMessage(char *cmd)
{
	printf("%s\r\n",cmd);
	//printf("%d\r\n",strlen(cmd));
	HAL_UART_Transmit(&wifiUsart,(unsigned char*)cmd,strlen(cmd),1000);
}
/*
* cmd:指令  timeout: 100ms的倍数
* 0:正确    1:错误
*/
uint8_t wifi_sendCmd(char *cmd,int timeout)
{
	unsigned char send_cmd[rx_rxBUFF_SIZE] = {0};
	rxCount = 0;
	memset(send_cmd,0,rx_rxBUFF_SIZE);
	memset(rxBuff,0,rx_rxBUFF_SIZE);
	sprintf((char*)send_cmd,"%s\r\n",cmd);
	while(1) 
	{
		if(HAL_UART_Transmit(&wifiUsart,send_cmd,strlen((char*)send_cmd),1000)==HAL_OK)
		{
			while(timeout--)
			{
				HAL_Delay(100);
				if(strstr(rxBuff,"OK"))  //在字符串 haystack 中查找第一次出现字符串 needle 的位置
					break;
				printf("%d ",timeout);
			}
			printf("\r\n");
			if(timeout<=0) return 1;
			else return 0;
		}
	}
}
/**
 * @description: 串口接收中断函数
 * @param {UART_HandleTypeDef} *huart
 * @return {*}
 * @author: 葛璐豪
 */
void wifi_interrupt(void)
{
	if(wifi_state==0) {
		if(rxBuffer!=0) 
		{
			rxBuff[rxCount] = rxBuffer;
			rxCount++;
		}
	} else {
		if(rxBuffer=='#')		  	                         //如果是S，表示是命令信息的起始位
		{
			rxCount=0x01; 
		}
		else if(rxBuffer=='$')		                         //如果E，表示是命令信息传送的结束位
		{
			//必须处理完之前的消息 才管接下来的
			if(receiveWifi_flag == 0) {
				receiveWifi_flag = 1;
			}
			printf("get message:%s\r\n", rxBuff);
		}				  
		else if((rxCount>0)&&(rxCount<=128))
		{
			rxBuff[rxCount-1]=rxBuffer;
			rxCount++;
		}
	}
	HAL_UART_Receive_IT(&wifiUsart,&rxBuffer,1);
}

