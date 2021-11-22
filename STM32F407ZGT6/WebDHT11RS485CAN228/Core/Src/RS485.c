#include "rs485.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

//���ջ����� 	
uint8_t RS485_buffer;
uint8_t RS485_receive_str[256];   //���ջ���,���128���ֽ�.
uint8_t uart_byte_count=0;        //���յ������ݳ���

void set_rs485_state(int state) //0Ϊ���� 1Ϊ����
{
	if(state==0) HAL_GPIO_WritePin(RS485_STATE_GPIO_Port, RS485_STATE_Pin,GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(RS485_STATE_GPIO_Port,RS485_STATE_Pin,GPIO_PIN_SET);
}
int get_rs485_state(void) 
{
	return HAL_GPIO_ReadPin(RS485_STATE_GPIO_Port,RS485_STATE_Pin);
}
void init_rs485(void)
{
	set_rs485_state(0);
	HAL_UART_Receive_IT(&rs485usart,&RS485_buffer,1);
}
/*
* ���ͻ�
* message:Ҫ���͵���Ϣ timeout:���ȴ����ٸ�100ms
* return: 0:�Է���δ�յ���Ϣ 1:�Է��ɹ��յ���Ϣ
*/

int rs485_sendData(char *message,int timeout)
{
	char final[128];
	int flag = 0;
	set_rs485_state(1);
	memset(final,0,128);
	sprintf(final,"#%s$",message);
	HAL_UART_Transmit_IT(&rs485usart,(uint8_t*)final,strlen(final));
	while(timeout--)
	{
		osDelay(100);
		if(get_toReceiveState())  //������ջ��յ���Ϣ
		{
			flag = 1;
			printf("\r\n�Է��ɹ��յ���Ϣ\r\n");
			break;
		}
		printf("%d ",timeout);
	}
	reset_toReceiveState(); //�ָ���Ϣ���ձ�־
	set_rs485_state(0);
	return flag;
}
/*
* ���ջ�
* return 1����Ϣ 0����Ϣ
* param  ������Ϣ message����ֵ  ������ϢlengthΪ��Ϣ����
*/
int get_message(char *message)
{
	int flag = 0;
	//����յ���Ϣ
	if(get_rs485_state() == 0)  //����ͬʱ�շ�
	{
		if(get_receiveState()==1) 
		{
			//Ӧ���źŲ�����
			if(strstr((char*)RS485_receive_str,"clicent_receive")){}
			//��ͨ��Ϣ���еĴ��� 
			else {
				//printf("�յ�����ϢΪ:%s\r\n",RS485_receive_str);
				strcpy(message,(char*)RS485_receive_str);
				flag = 1;
			}
			reset_rxbuff();
			reset_receiveState();
			//����Ӧ����Ϣ ����ȴ�
			rs485_sendData("clicent_receive",0);
		}
	}
	return flag;
}
/*
* �����жϴ���
*/
/* ���ͻ���Ҫ��*/
int clicent_receive = 0; //0 ����Է�δ���յ� 1����Է����յ�
int get_toReceiveState(void)
{
	return clicent_receive;
}
void reset_toReceiveState(void)
{
	clicent_receive=0;
}
/*���ջ���Ҫ��*/
int receive_flag = 0; //0 ������ջ�δ�յ� 1������ջ��յ�
int get_receiveState(void)
{
	return receive_flag;
}
void reset_receiveState(void)
{
	receive_flag = 0;
}
void reset_rxbuff(void)
{
	for(uart_byte_count=0;uart_byte_count<32;uart_byte_count++) RS485_receive_str[uart_byte_count]=0x00;
			uart_byte_count=0;  
}
void rs485_receive_interrupt(void)
{
		if(RS485_buffer=='#')		  	                         //�����S����ʾ��������Ϣ����ʼλ
		{
			uart_byte_count=0x01; 
		}
		else if(RS485_buffer=='$')		                         //���E����ʾ��������Ϣ���͵Ľ���λ
		{
			//printf("�յ�����Ϣ\r\n");
			//���ͻ�����ȷ�϶Է��յ���Ϣ�Ĵ���
			if(strcmp("clicent_receive",(char *)RS485_receive_str)==0)  {
				clicent_receive=1;	
				reset_rxbuff();
			}
			//���ջ����ڽ��յ�����Ϣ����
			else if(get_rs485_state()==0){
				receive_flag = 1;
			} 
			else 
			{
				reset_rxbuff();
			}
		}
		else if((uart_byte_count>0)&&(uart_byte_count<=128))
		{
			RS485_receive_str[uart_byte_count-1]=RS485_buffer;
			uart_byte_count++;
		}
		HAL_UART_Receive_IT(&rs485usart,&RS485_buffer,1);
}
