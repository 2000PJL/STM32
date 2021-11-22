#include "rs485.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

//接收缓存区 	
uint8_t RS485_buffer;
uint8_t RS485_receive_str[256];   //接收缓冲,最大128个字节.
uint8_t uart_byte_count=0;        //接收到的数据长度

void set_rs485_state(int state) //0为接收 1为发送
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
* 发送机
* message:要发送的消息 timeout:最大等待多少个100ms
* return: 0:对方并未收到消息 1:对方成功收到消息
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
		if(get_toReceiveState())  //如果接收机收到信息
		{
			flag = 1;
			printf("\r\n对方成功收到消息\r\n");
			break;
		}
		printf("%d ",timeout);
	}
	reset_toReceiveState(); //恢复消息接收标志
	set_rs485_state(0);
	return flag;
}
/*
* 接收机
* return 1有消息 0无消息
* param  若有消息 message会有值  若有消息length为消息长度
*/
int get_message(char *message)
{
	int flag = 0;
	//如果收到消息
	if(get_rs485_state() == 0)  //不能同时收发
	{
		if(get_receiveState()==1) 
		{
			//应答信号不处理
			if(strstr((char*)RS485_receive_str,"clicent_receive")){}
			//普通消息进行的处理 
			else {
				//printf("收到的消息为:%s\r\n",RS485_receive_str);
				strcpy(message,(char*)RS485_receive_str);
				flag = 1;
			}
			reset_rxbuff();
			reset_receiveState();
			//发送应答消息 不设等待
			rs485_sendData("clicent_receive",0);
		}
	}
	return flag;
}
/*
* 接收中断代码
*/
/* 发送机需要用*/
int clicent_receive = 0; //0 代表对方未接收到 1代表对方接收到
int get_toReceiveState(void)
{
	return clicent_receive;
}
void reset_toReceiveState(void)
{
	clicent_receive=0;
}
/*接收机需要用*/
int receive_flag = 0; //0 代表接收机未收到 1代表接收机收到
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
		if(RS485_buffer=='#')		  	                         //如果是S，表示是命令信息的起始位
		{
			uart_byte_count=0x01; 
		}
		else if(RS485_buffer=='$')		                         //如果E，表示是命令信息传送的结束位
		{
			//printf("收到了消息\r\n");
			//发送机用于确认对方收到消息的代码
			if(strcmp("clicent_receive",(char *)RS485_receive_str)==0)  {
				clicent_receive=1;	
				reset_rxbuff();
			}
			//接收机用于将收到的消息缓存
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
