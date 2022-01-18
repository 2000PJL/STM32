#include "cc2530.h"


const int source_id = 0;				//��ǰ�ڵ��豸���	��۽ڵ�
const int central_node_id = 0;	//���Ľڵ��豸���

/* �жϽ������ñ��� */

#define cc2530_rx_rxBUFF_SIZE 500
uint8_t cc2530_rxBuffer = 0;
unsigned char cc2530_rxCount = 0;  	//������
char cc2530_rxBuff[cc2530_rx_rxBUFF_SIZE] = {0};   //������
int receiveCC2530_flag = 0;   //0����Ϣ 1 ����Ϣ


void cc2530_config(void)
{
	HAL_UART_Receive_IT(&cc2530Usart,&cc2530_rxBuffer,1);//ʹ��
}
/**
  * ��������: ��Ŀ��ڵ㷢����Ϣ
  * �������: 
			*msg:������Ϣ
			goal_id:Ŀ��ڵ�id
  * �� �� ֵ: ��
  * ˵    ������
  */
void cc2530_sendMessage(char *msg)
{
//	printf("%s\r\n", msg);
	char cc2530_message[50] = {0};
	sprintf(cc2530_message, "#%s$",msg);
	HAL_UART_Transmit(&cc2530Usart, (unsigned char*)cc2530_message, strlen(cc2530_message), 1000);
}

void cc2530_getMessage(char *msg)
{
	if(receiveCC2530_flag==1) {
		strcpy(msg,(char*)cc2530_rxBuff);
		memset(cc2530_rxBuff,0,cc2530_rx_rxBUFF_SIZE);
		receiveCC2530_flag = 0;
	}
}

void get_cc2530Message(char *msg)
{
	//������Ϣ
	if(receiveCC2530_flag==1) {
		strcpy(msg,(char*)cc2530_rxBuff);
		memset(cc2530_rxBuff,0,cc2530_rx_rxBUFF_SIZE);
		receiveCC2530_flag = 0;
	}
}

/**
  * ��������: ��Ŀ��ڵ㷢����Ϣ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void cc2530_interrupt(void)
{
	if(cc2530_rxBuffer=='#')		  	                         //�����#����ʾ��������Ϣ����ʼλ
	{
		cc2530_rxCount=0x01; 
	}
	else if(cc2530_rxBuffer=='$')		                         //�����$����ʾ��������Ϣ�Ľ���λ
	{
		//���봦����֮ǰ����Ϣ �Źܽ�������
		if(receiveCC2530_flag == 0) 
		{
			receiveCC2530_flag = 1;
			cc2530_rxCount=0;
		}
	}				  
	else if((cc2530_rxCount>0)&&(cc2530_rxCount<=128))
	{
		cc2530_rxBuff[cc2530_rxCount-1]=cc2530_rxBuffer;
		cc2530_rxCount++;
	}
	HAL_UART_Receive_IT(&cc2530Usart,&cc2530_rxBuffer,1);
}
