/*
 * @Description: 
 * @Version: 2.0
 * @Autor: ��贺�
 * @Date: 2020-12-23 20:27:02
 * @LastEditors: Seven
 * @LastEditTime: 2020-12-26 17:06:28
 */
#include "blue.h"
#include "main.h"
/* ������������ */
unsigned char blueName[] = "PJL";
unsigned char bluePwd[] = "1234"; //�����������6λ������

/* config=1ʱΪATģʽ,�������������ָ���. config=2ʱ�����շ� */
int config = 2; 



/* ATָ������� */
char cmdBuf[100] = {0};

/* �жϽ������ñ��� */
#define rx_rxBUFF_SIZE 500
uint8_t rxBuffer = 0;
unsigned char rxCount = 0;  //������
char rxBuff[rx_rxBUFF_SIZE] = {0};   //������
/**
 * @description: ������������
 * @param {char} *cmd
 * @return {*}
 * @author: ��贺�
 */
void blue_send(char *cmd)
{
	HAL_UART_Transmit(&blueUsart,(uint8_t*)cmd,strlen((char*)cmd),1000);
}

/**
 * @description: ATģʽ�´��ڳ�ʼ������ ������38400 �̶�
 * @param {*}
 * @return {*}
 * @author: ��贺�
 */
void BlueConfig_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 38400;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
}
/**
 * @description: ATģʽ��������ʼ��
 * @param {*}
 * @return {*} 0:�ɹ� 1:ʧ��
 * @author: ��贺�
 */
uint8_t blue_config(void)
{
	BlueConfig_UART_Init();
	HAL_UART_Receive_IT(&blueUsart,&rxBuffer,1); //�����жϿ���
	printf("����5s���ɿ�����\r\n");
	HAL_Delay(5000);
	sprintf(cmdBuf,"AT+NAME=%s\r\n",blueName);
	if(blue_sendCmd(cmdBuf,50))
	{
		printf("���ָ���ʧ��,��������\r\n");
		return 1;
	}
	else
	{
		printf("���ָ��ĳɹ�!!\r\n");
	}
	memset(cmdBuf,0,sizeof(cmdBuf));
	sprintf(cmdBuf,"AT+PSWD=\"%s\"\r\n",bluePwd);
	if(blue_sendCmd(cmdBuf,50))
	{
		printf("�������ʧ��,��������\r\n");
		return 1;
	}
	else
	{
		printf("������ĳɹ�!!\r\n");
	}
	return 0;
}
/**
 * @description: ATָ���
 * @param {*} cmdΪatָ�� timeoutΪ���ȴ����� ÿ������100ms
 * @return {*} 0:�ɹ� 1:ʧ��
 * @author: ��贺�
 */
uint8_t blue_sendCmd(char *cmd,int timeout)
{
	unsigned char send_cmd[rx_rxBUFF_SIZE] = {0};
	rxCount = 0;
	memset(rxBuff,0,rx_rxBUFF_SIZE);
	sprintf((char*)send_cmd,"%s\r\n",cmd);
	printf("%s",send_cmd);
	printf("%d",strlen((char*)send_cmd));
	while(1) 
	{
		if(HAL_UART_Transmit(&blueUsart,send_cmd,strlen((char*)send_cmd),1000)==HAL_OK)
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

void bluetooth_interrupt()
{
	if(rxBuffer!=0) 
	{
		rxBuff[rxCount] = rxBuffer;
		rxCount++;
	}
	HAL_UART_Receive_IT(&blueUsart,&rxBuffer,1);
}


void blue_Init()
{
		printf("���������������������,��ֱ����config=2,����������Ϊ1234");
	printf("ģʽһ(ATģʽ)��config=1ʱ�����ģʽ,�ڰ����ϵ�֮ǰ��ס����(��Ҫ�ɿ�)��\
          �ڰ����ϵ���ɿ���������ʱ��2s��˸һ��,atָ��͸�������������֮����������\r\n");
	printf("ģʽ��(��������)��config=2ʱ�����ģʽ�����ʱ����˸�ܿ�,������Ϊ38400,ʲô�����ùܣ������շ�\r\n");
	printf("��ǰģʽΪconfig=%d,ע��\r\n",config);
	if(config==1) 
	{
		while(blue_config()==1);
		printf("����Ĵ���,��config=2��ˢ�����󣬶ϵ��������� ����λ������\r\n");
		while(1);
	}
}
