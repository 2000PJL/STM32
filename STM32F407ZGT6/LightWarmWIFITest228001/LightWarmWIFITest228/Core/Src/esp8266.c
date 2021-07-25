/*
 * @Description: 
 * @Version: 2.0
 * @Autor: ��贺�
 * @Date: 2020-12-05 12:30:48
 * @LastEditors: Seven
 * @LastEditTime: 2020-12-27 12:32:22
 */
#include "esp8266.h"
#include "usart.h"
unsigned char WIFI_NAME[] = "HONOR 9X";
unsigned char WIFI_MI[] = "1985343015";

unsigned char IP[] = "192.168.137.1";
unsigned char PORT[] = "19853";
/* �жϽ������ñ��� */
#define rx_rxBUFF_SIZE 500
uint8_t rxBuffer = 0;
unsigned char rxCount = 0;  //������
char rxBuff[rx_rxBUFF_SIZE] = {0};   //������

char serve[150] = {0};
char client[50] = {0};
uint8_t wifi_config(void)
{
	memset(serve,0,150);
	memset(client,0,50);
	HAL_UART_Receive_IT(&wifiUsart,&rxBuffer,1);

	if(wifi_reset(50))
	{
		printf("wifi ��λʧ��\r\n");
		return 1;	
	}
	else
	{
		//HAL_UART_Transmit(&wifiUsart,(unsigned char *)"AT+RST",strlen("AT+RST"),1000);
		printf("wifi ��λ�ɹ�!!\r\n");
	}

	printf("(1) ģʽ������\r\n");
	if(wifi_sendCmd("AT+CWMODE=1",50))
	{
		printf("ģʽ����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("ģʽ���óɹ�!!\r\n");
	}
	
	printf("(2)��λ������\r\n");
	if(wifi_sendCmd("AT+RST",50))
	{
		printf("��λ����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("��λ���óɹ�!!\r\n");
	}
	
	HAL_Delay(3000);
	printf("(3) ����wifi��\r\n");
	printf("�������ӵ�wifi��=%s\r\n",WIFI_NAME);
	printf("�������ӵ�wifi����= %s\r\n",WIFI_MI);
	
	sprintf(client,"AT+CWJAP=\"%s\",\"%s\"",WIFI_NAME,WIFI_MI);
	if(wifi_sendCmd(client,500))
	{
		printf("wifi����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("wifi���ӳɹ�!!\r\n");
	}
	
	printf("(4) ����s_connection����\r\n");
	if(wifi_sendCmd("AT+CIPMUX=0",50))
	{
		printf("s_connection����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("s_connection���óɹ�!!\r\n");
	}
	
	
	printf("(5) ����OPEN_TI_MODE ����\r\n");
	if(wifi_sendCmd("AT+CIPMODE=1",50))
	{
		printf("OPEN_TI_MODE ����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("OPEN_TI_MODE ���óɹ�!!\r\n");
	}
	
	printf("(6) TCP������\r\n");
	sprintf(serve,"AT+CIPSTART=\"TCP\",\"%s\",%s",IP,PORT);
	printf("send=%s\r\n",serve);
	if(wifi_sendCmd(serve,50))
	{
		printf("TCP����ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("TCP�����ɹ�!!\r\n");
	}
	
	printf("(7) �������ݴ�����\r\n");
	if(wifi_sendCmd("AT+CIPSEND",50))
	{
		printf("�������ݴ���ʧ�� ����\r\n");
		return 1;
	}
	else
	{
		printf("�������ݴ���ɹ� ���Է���������!!\r\n");
	}
	return 0;
}
uint8_t wifi_reset(int timeout)
{
	HAL_GPIO_WritePin(WIFI_RST_GPIO_Port,WIFI_RST_Pin,GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(WIFI_RST_GPIO_Port,WIFI_RST_Pin,GPIO_PIN_SET);
	while(timeout--)
	{
		HAL_Delay(100);
		if(strstr(rxBuff,"ready"))
			break;
		printf("%d \r\n",timeout);
	}
	printf("\r\n");
	if(timeout<=0) return 1;
	else return 0;
}
void wifi_sendMessage(char *cmd)
{
	HAL_UART_Transmit(&wifiUsart,(unsigned char*)cmd,strlen(cmd),1000);
}
/*
* cmd:ָ��  timeout: 100ms�ı���
* 0:��ȷ    1:����
*/
uint8_t wifi_sendCmd(char *cmd,int timeout)
{
	unsigned char send_cmd[rx_rxBUFF_SIZE] = {0};
	rxCount = 0;
	memset(send_cmd,0,rx_rxBUFF_SIZE);
	sprintf((char*)send_cmd,"%s\r\n",cmd);
	while(1) 
	{
		if(HAL_UART_Transmit(&wifiUsart,send_cmd,strlen((char*)send_cmd),1000)==HAL_OK)
		{
			while(timeout--)
			{
				HAL_Delay(100);
				if(strstr(rxBuff,"OK"))  //���ַ��� haystack �в��ҵ�һ�γ����ַ��� needle ��λ��
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
 * @description: ���ڽ����жϺ���
 * @param {UART_HandleTypeDef} *huart
 * @return {*}
 * @author: ��贺�
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
	if(huart->Instance==wifiInterrupt)  //���������ж�
	{

		if(rxBuffer!=0) 
		{
			rxBuff[rxCount] = rxBuffer;
			rxCount++;
		}

		HAL_UART_Receive_IT(&wifiUsart,&rxBuffer,1);
	}
}
