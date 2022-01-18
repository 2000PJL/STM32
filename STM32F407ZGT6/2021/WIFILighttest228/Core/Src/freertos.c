/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "esp8266.h"
#include "light.h"
#include "adc.h"
#include "stdio.h"
#include "string.h"
#include "stm32f4xx_it.h"

extern __IO uint8_t ucTcpClosedFlag;
uint16_t ADC_Value1=0,ADC_LIGHT=0;
uint8_t str_buff[64];
uint8_t ucStatus;

char cStr [128];

void UR1_Send_Info(){
  sprintf((char *)str_buff,"RS232串口显示：采样值:%d,光敏值:%d.%d%dB\r\n",ADC_Value1,ADC_LIGHT/100,(ADC_LIGHT%100)/10,ADC_LIGHT%10);	
	sprintf((char *)cStr,"ESP8266显示：采样值:%d,光敏值:%d.%d%dB\r\n",ADC_Value1,ADC_LIGHT/100,(ADC_LIGHT%100)/10,ADC_LIGHT%10);
	HAL_UART_Transmit(&huart1,str_buff,sizeof(str_buff),1000);
}


/* 私有变量 ------------------------------------------------------------------*/
//char cStr [128];
//char txbuf[128];
/* 扩展变量 ------------------------------------------------------------------*/
//extern __IO uint8_t ucTcpClosedFlag;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId LightTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartLightTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of LightTask */
  osThreadDef(LightTask, StartLightTask, osPriorityNormal, 0, 128);
  LightTaskHandle = osThreadCreate(osThread(LightTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartLightTask */
/**
  * @brief  Function implementing the LightTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartLightTask */
void StartLightTask(void const * argument)
{
  /* USER CODE BEGIN StartLightTask */
  /* Infinite loop */
  
	//ESP8266wo();
  for(;;)
  {
		HAL_ADC_Start(&hadc1);
	  LED0_ON();
	if (HAL_ADC_PollForConversion(&hadc1,10)==HAL_OK){
		ADC_Value1=HAL_ADC_GetValue(&hadc1);
		ADC_LIGHT=ADC_Value1*330/4096;
    osDelay(100);		
	  UR1_Send_Info();
		ESP8266_SendString(ENABLE,cStr,strlen(cStr),Single_ID_0);
		//sprintf((char *)cStr,"采样值:%d,光敏值:%d.%d%dB\r\n",ADC_Value1,ADC_LIGHT/100,(ADC_LIGHT%100)/10,ADC_LIGHT%10);
  	osDelay(100);
  	LED0_OFF();
  	osDelay(100);
  	HAL_ADC_Stop(&hadc1);
	}

     if(ucTcpClosedFlag)                                             //检测是否失去连接
		{
			ESP8266_ExitUnvarnishSend();                      		
			do ucStatus = ESP8266_Get_LinkStatus();                         //获取连接状态
			while(!ucStatus);			
			if(ucStatus==4)                                             //确认失去连接后重连
			{
				printf("正在重连热点和服务器 ......\n");				
				while(!ESP8266_JoinAP(User_ESP8266_ApSsid,User_ESP8266_ApPwd));				
				while(!ESP8266_Link_Server(enumTCP,User_ESP8266_TcpServer_IP,User_ESP8266_TcpServer_Port,Single_ID_0));				
				printf("重连热点和服务器成功!!!\n");
			}			
			while(!ESP8266_UnvarnishSend());					
		}
  }
  /* USER CODE END StartLightTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
