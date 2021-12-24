/*
 * @Description: 
 * @Version: 2.0
 * @Autor: 葛璐豪
 * @Date: 2020-12-23 20:27:02
 * @LastEditors: Seven
 * @LastEditTime: 2020-12-26 17:06:28
 */
#include "blue.h"
#include "main.h"
/* 蓝牙名与密码 */
unsigned char blueName[] = "PJL";
unsigned char bluePwd[] = "1234"; //蓝牙密码最多6位且数字

/* config=1时为AT模式,进行密码与名字更改. config=2时正常收发 */
int config = 2; 



/* AT指令缓存数组 */
char cmdBuf[100] = {0};

/* 中断接收所用变量 */
#define rx_rxBUFF_SIZE 500
uint8_t rxBuffer = 0;
unsigned char rxCount = 0;  //计数器
char rxBuff[rx_rxBUFF_SIZE] = {0};   //缓冲区
/**
 * @description: 蓝牙发送数据
 * @param {char} *cmd
 * @return {*}
 * @author: 葛璐豪
 */
void blue_send(char *cmd)
{
	HAL_UART_Transmit(&blueUsart,(uint8_t*)cmd,strlen((char*)cmd),1000);
}

/**
 * @description: AT模式下串口初始化配置 波特率38400 固定
 * @param {*}
 * @return {*}
 * @author: 葛璐豪
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
 * @description: AT模式下蓝牙初始化
 * @param {*}
 * @return {*} 0:成功 1:失败
 * @author: 葛璐豪
 */
uint8_t blue_config(void)
{
	BlueConfig_UART_Init();
	HAL_UART_Receive_IT(&blueUsart,&rxBuffer,1); //接收中断开启
	printf("请在5s内松开按键\r\n");
	HAL_Delay(5000);
	sprintf(cmdBuf,"AT+NAME=%s\r\n",blueName);
	if(blue_sendCmd(cmdBuf,50))
	{
		printf("名字更改失败,即将重启\r\n");
		return 1;
	}
	else
	{
		printf("名字更改成功!!\r\n");
	}
	memset(cmdBuf,0,sizeof(cmdBuf));
	sprintf(cmdBuf,"AT+PSWD=\"%s\"\r\n",bluePwd);
	if(blue_sendCmd(cmdBuf,50))
	{
		printf("密码更改失败,即将重启\r\n");
		return 1;
	}
	else
	{
		printf("密码更改成功!!\r\n");
	}
	return 0;
}
/**
 * @description: AT指令函数
 * @param {*} cmd为at指令 timeout为最大等待周期 每个周期100ms
 * @return {*} 0:成功 1:失败
 * @author: 葛璐豪
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
		printf("若不想更改蓝牙名和密码,请直接让config=2,该蓝牙密码为1234");
	printf("模式一(AT模式)：config=1时进入该模式,在板子上电之前按住按键(不要松开)，\
          在板子上电后松开按键，此时灯2s闪烁一次,at指令发送更改完名字密码之后，重启板子\r\n");
	printf("模式二(正常发送)：config=2时进入该模式，配对时灯闪烁很快,波特率为38400,什么都不用管，正常收发\r\n");
	printf("当前模式为config=%d,注意\r\n",config);
	if(config==1) 
	{
		while(blue_config()==1);
		printf("请更改代码,让config=2，刷入代码后，断电重启！！ 按复位键不行\r\n");
		while(1);
	}
}
