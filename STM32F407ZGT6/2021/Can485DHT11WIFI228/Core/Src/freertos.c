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
#include "ESP8266_AP.h"
#include "delay.h"
#include "key.h"
#include "RS485.h"
#include "dht11.h"
#include "can.h"

char DHT11_buffer[128];
int cansend_flag=0;

	
DHT11_Data_TypeDef DHT11_Data;

extern char can_flag;
extern char wifi_rxbuf[];
extern uint8_t RS485_receive_str[];
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
osThreadId DHT11TaskHandle;
osThreadId MainTaskHandle;
osThreadId CanSendTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDHT11Task(void const * argument);
void StartMainTask(void const * argument);
void StartCanSendTask(void const * argument);

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
  /* definition and creation of DHT11Task */
  osThreadDef(DHT11Task, StartDHT11Task, osPriorityNormal, 0, 256);
  DHT11TaskHandle = osThreadCreate(osThread(DHT11Task), NULL);

  /* definition and creation of MainTask */
  osThreadDef(MainTask, StartMainTask, osPriorityIdle, 0, 128);
  MainTaskHandle = osThreadCreate(osThread(MainTask), NULL);

  /* definition and creation of CanSendTask */
  osThreadDef(CanSendTask, StartCanSendTask, osPriorityIdle, 0, 128);
  CanSendTaskHandle = osThreadCreate(osThread(CanSendTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDHT11Task */
/**
  * @brief  Function implementing the DHT11Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDHT11Task */
void StartDHT11Task(void const * argument)
{
  /* USER CODE BEGIN StartDHT11Task */
  /* Infinite loop */
  for(;;)
  {
   if(DHT11_Read_TempAndHumidity(&DHT11_Data)==SUCCESS)
    {
      sprintf(DHT11_buffer,"%.0f,%.1f",DHT11_Data.humidity,DHT11_Data.temperature);
    }
	osDelay(1000);
  }
  /* USER CODE END StartDHT11Task */
}

/* USER CODE BEGIN Header_StartMainTask */
/**
* @brief Function implementing the myMainTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMainTask */
void StartMainTask(void const * argument)
{
  /* USER CODE BEGIN StartMainTask */
	//osThreadSuspend(DHT11TaskHandle);
	osThreadSuspend(CanSendTaskHandle);
	printf("初始化完成，部分线程已被挂起");
	uint8_t  temp1[8],temp2[8];
	uint8_t message[32],wifi_sendmessage[32];
  /* Infinite loop */
  for(;;)
  {
	  if(get_canMessage(message) == 1)
	  {
		  memcpy(temp1,message,2);
		  temp2[0]=message[3],temp2[1]=message[4],temp2[2]=message[5],temp2[3]=message[6];
		  sprintf((char *)wifi_sendmessage,"采集到的湿度为%s%%,温度为%s℃",temp1,temp2);
		  Wifi_SendData("AT+CIPSEND=0,30\r\n");
		  printf("can发送的数据为：%s",message);
		  osDelay(100);
		  Wifi_SendData((char *)wifi_sendmessage);
	  }
	  if(cansend_flag==1)
	  {
		  osThreadResume(CanSendTaskHandle);
	  }else if(cansend_flag==0)
	  {
		  osThreadSuspend(CanSendTaskHandle);
	  }
	  osDelay(1000);
  }
  /* USER CODE END StartMainTask */
}

/* USER CODE BEGIN Header_StartCanSendTask */
/**
* @brief Function implementing the CanSendTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCanSendTask */
void StartCanSendTask(void const * argument)
{
  /* USER CODE BEGIN StartCanSendTask */
  /* Infinite loop */
  for(;;)
  {
	CAN_TRANSMIT1((uint8_t *)DHT11_buffer);
    osDelay(500);
	  printf("can发送的数据为：%s",DHT11_buffer);
  }
  /* USER CODE END StartCanSendTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
