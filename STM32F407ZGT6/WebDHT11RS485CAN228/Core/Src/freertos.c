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
#include "blue.h"
#include "RS485.h"
#include "dht11.h"
#include "hr04.h"


extern uint8_t RS485_receive_str[];
extern int key_value;
extern char rxBuff[];

char DHT11_buffer[128];
char blue_sendmsg[32];
char blue_sendmsg1[32];
char RS485_receivemsg[32];

double distance;
	
DHT11_Data_TypeDef DHT11_Data;


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
osThreadId VoiceTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDHT11Task(void const * argument);
void StartMainTask(void const * argument);
void StartVoiceTask(void const * argument);

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
  osThreadDef(DHT11Task, StartDHT11Task, osPriorityNormal, 0, 512);
  DHT11TaskHandle = osThreadCreate(osThread(DHT11Task), NULL);

  /* definition and creation of MainTask */
  osThreadDef(MainTask, StartMainTask, osPriorityIdle, 0, 512);
  MainTaskHandle = osThreadCreate(osThread(MainTask), NULL);

  /* definition and creation of VoiceTask */
  osThreadDef(VoiceTask, StartVoiceTask, osPriorityBelowNormal, 0, 512);
  VoiceTaskHandle = osThreadCreate(osThread(VoiceTask), NULL);

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
    osDelay(1000);
	if(DHT11_Read_TempAndHumidity(&DHT11_Data)==SUCCESS)
    {
      sprintf(DHT11_buffer,"%.0f,%.1f",DHT11_Data.humidity,DHT11_Data.temperature);
 	  //rs485_sendData(DHT11_buffer, strlen(DHT11_buffer));
	  memset(RS485_receivemsg, 0, 32);
    }else printf("读取失败");
	printf("%s",DHT11_buffer);
  }
  /* USER CODE END StartDHT11Task */
}

/* USER CODE BEGIN Header_StartMainTask */
/**
* @brief Function implementing the MainTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMainTask */
void StartMainTask(void const * argument)
{
  /* USER CODE BEGIN StartMainTask */
	osThreadSuspend(DHT11TaskHandle);
	osThreadSuspend(VoiceTaskHandle);
	init_rs485();
  /* Infinite loop */
  for(;;)
  {
	  osDelay(1000);
	  switch(key_value)
	  {
		  case 0: 
		  {
			  osThreadResume(DHT11TaskHandle);
		  }
		  case 1: 
		  {
			  osThreadSuspend(DHT11TaskHandle);
		  }
		  case 2: 
		  {
			  osThreadResume(VoiceTaskHandle);
		  }
		  case 3: 
		  {
			  osThreadSuspend(VoiceTaskHandle);
		  }
	  }
	  printf("blutooth接收到的数据为：%s\r\n",rxBuff);
	  if(strcmp("OLED",rxBuff)==0)
	  {
		  LED2_ON();
		  rs485_sendData("OL", strlen("OL"));
		  
	  }else if(strcmp("CLED",rxBuff)==0)
	  {
		  LED2_OFF();
		  rs485_sendData("CL", strlen("CL"));
	  }else if(strcmp("OBEEP",rxBuff)==0)
	  {
		  BEEP_ON();
		  rs485_sendData("OB", strlen("OB"));
	  }else if(strcmp("CBEEP",rxBuff)==0)
	  {
		  BEEP_OFF();
		  rs485_sendData("CB", strlen("CB"));
	  }
	  get_message((char*)RS485_receivemsg);
	  if(strlen(RS485_receivemsg)!=2)
	  {
		  sprintf(blue_sendmsg,"#,2,%.1f,%s,$",distance,RS485_receivemsg);
		  blue_send(blue_sendmsg);
		  printf("%s\r\n",blue_sendmsg);
	  }else {
	  if(strcmp("OL",RS485_receivemsg)==0)
	  {
		  LED2_ON();
		  
	  }else if(strcmp("CL",RS485_receivemsg)==0)
	  {
		  LED2_OFF();
	  }else if(strcmp("OB",RS485_receivemsg)==0)
	  {
		  BEEP_ON();
	  }else if(strcmp("CB",RS485_receivemsg)==0)
	  {
		  BEEP_OFF();
	  }
	  }
	  
  }
  /* USER CODE END StartMainTask */
}

/* USER CODE BEGIN Header_StartVoiceTask */
/**
* @brief Function implementing the VoiceTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartVoiceTask */
void StartVoiceTask(void const * argument)
{
  /* USER CODE BEGIN StartVoiceTask */
	osThreadSuspend(VoiceTaskHandle);
  /* Infinite loop */
  for(;;)
  {
    distance=Hcsr04_StateRead();
    printf("距离是：%lf\r\n",distance);
    osDelay(777);
  }
  /* USER CODE END StartVoiceTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
