#include "cc2530.h"


const int source_id = 0;				//当前节点设备编号	汇聚节点
const int central_node_id = 0;	//中心节点设备编号

/* 中断接收所用变量 */

#define cc2530_rx_rxBUFF_SIZE 500
uint8_t cc2530_rxBuffer = 0;
unsigned char cc2530_rxCount = 0;  	//计数器
char cc2530_rxBuff[cc2530_rx_rxBUFF_SIZE] = {0};   //缓冲区
int receiveCC2530_flag = 0;   //0无消息 1 有消息


void cc2530_config(void)
{
	HAL_UART_Receive_IT(&cc2530Usart,&cc2530_rxBuffer,1);//使能
}
/**
  * 函数功能: 向目标节点发送消息
  * 输入参数: 
			*msg:发送信息
			goal_id:目标节点id
  * 返 回 值: 无
  * 说    明：无
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
	//若有消息
	if(receiveCC2530_flag==1) {
		strcpy(msg,(char*)cc2530_rxBuff);
		memset(cc2530_rxBuff,0,cc2530_rx_rxBUFF_SIZE);
		receiveCC2530_flag = 0;
	}
}

/**
  * 函数功能: 向目标节点发送消息
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void cc2530_interrupt(void)
{
	if(cc2530_rxBuffer=='#')		  	                         //如果是#，表示是命令信息的起始位
	{
		cc2530_rxCount=0x01; 
	}
	else if(cc2530_rxBuffer=='$')		                         //如果是$，表示是命令信息的结束位
	{
		//必须处理完之前的消息 才管接下来的
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
