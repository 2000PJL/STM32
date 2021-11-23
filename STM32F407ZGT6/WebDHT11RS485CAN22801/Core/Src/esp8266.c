/*
 * @Description: 
 * @Version: 2.0
 * @Autor: ��贺�
 * @Date: 2020-12-05 12:30:48
 * @LastEditors: Seven
 * @LastEditTime: 2020-12-27 12:32:22
 */
#include "esp8266.h"
unsigned char WIFI_NAME[] = "Elsa";
unsigned char WIFI_MI[] = "41282620";

unsigned char IP[] = "192.168.137.1";
unsigned char PORT[] = "5000";
/* �жϽ������ñ��� */
uint8_t wifi_state = 0;
#define rx_rxBUFF_SIZE 500
uint8_t rxBuffer = 0;
unsigned char rxCount = 0;  //������
char rxBuff[rx_rxBUFF_SIZE] = {0};   //������
int receiveWifi_flag = 0;  //0����Ϣ 1 ����Ϣ
char serve[150] = {0};
char client[50] = {0};
int get_wifiMessage(char *message)
{
	//������Ϣ
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
		printf("wifi ��λʧ��\r\n");
		return 1;	
	}
	else
	{
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
* cmd:ָ��  timeout: 100ms�ı���
* 0:��ȷ    1:����
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
void wifi_interrupt(void)
{
	if(wifi_state==0) {
		if(rxBuffer!=0) 
		{
			rxBuff[rxCount] = rxBuffer;
			rxCount++;
		}
	} else {
		if(rxBuffer=='#')		  	                         //�����S����ʾ��������Ϣ����ʼλ
		{
			rxCount=0x01; 
		}
		else if(rxBuffer=='$')		                         //���E����ʾ��������Ϣ���͵Ľ���λ
		{
			//���봦����֮ǰ����Ϣ �Źܽ�������
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

