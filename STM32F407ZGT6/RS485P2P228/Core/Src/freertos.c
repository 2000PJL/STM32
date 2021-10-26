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
#include "stdio.h"
#include "string.h"
#include "RS485.h"
#include "GPIO.h"
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
osThreadId RS485ReadTaskHandle;
osThreadId RS485WriteTaskHandle;
osMessageQId KeyQueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartRS485ReadTask(void const * argument);
void StartRS485WriteTask(void const * argument);

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

  /* Create the queue(s) */
  /* definition and creation of KeyQueue */
  osMessageQDef(KeyQueue, 2, uint8_t);
  KeyQueueHandle = osMessageCreate(osMessageQ(KeyQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of RS485ReadTask */
  osThreadDef(RS485ReadTask, StartRS485ReadTask, osPriorityNormal, 0, 128);
  RS485ReadTaskHandle = osThreadCreate(osThread(RS485ReadTask), NULL);

  /* definition and creation of RS485WriteTask */
  osThreadDef(RS485WriteTask, StartRS485WriteTask, osPriorityLow, 0, 128);
  RS485WriteTaskHandle = osThreadCreate(osThread(RS485WriteTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartRS485ReadTask */
/**
  * @brief  Function implementing the RS485ReadTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartRS485ReadTask */
void StartRS485ReadTask(void const * argument)
{
  /* USER CODE BEGIN StartRS485ReadTask */
    osEvent event;
	init_rs485();
	rs485_sendData("init", 10);
  /* Infinite loop */
  for(;;)
  {
		event = osMessageGet( KeyQueueHandle, 100);
		
		if( event.status == osEventMessage )
		{
			printf( "message = %d\r\n", event.value.v);
			
			if(event.value.v == 0)
			{
				rs485_sendData("open_led0", 10);
				printf("点亮了LED0");
			}
			else if(event.value.v == 1)
			{
				rs485_sendData("open_led1", 10);
				printf("点亮了LED1");
			}
			else if(event.value.v == 2)
			{
				rs485_sendData("open_beep", 10);
				printf("打开了蜂鸣器");
			}
			else if(event.value.v == 3)
			{
				rs485_sendData("close_led0", 10);
				printf("关闭了LED0");
			}
			else if(event.value.v == 4)
			{
				rs485_sendData("close_led1", 10);
				printf("关闭了LED1");
			}
			else if(event.value.v == 5)
			{
				rs485_sendData("close_beep", 10);
				printf("关闭了蜂鸣器");
			}
			else if(event.value.v == 6)
			{
				rs485_sendData("open_led2", 10);
				printf("点亮了LED2");
			}
			else if(event.value.v == 7)
			{
				rs485_sendData("close_led2", 10);
				printf("关闭了LED2");
			}
		}
  }
  /* USER CODE END StartRS485ReadTask */
}

/* USER CODE BEGIN Header_StartRS485WriteTask */
/**
* @brief Function implementing the RS485WriteTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRS485WriteTask */
void StartRS485WriteTask(void const * argument)
{
  /* USER CODE BEGIN StartRS485WriteTask */
  uint8_t receive[64];
  /* Infinite loop */
  for(;;)
  {
    if(get_message((char*)receive)==1)
		{
			printf("消息是%s\r\n",receive);
			if(strcmp((char *)receive, "init") == 0)
				printf("我要初始化了");
				//MX_GPIO_Init();
			else if(strcmp((char *)receive, "open_led0") == 0)
				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
			else if(strcmp((char *)receive, "close_led0") == 0)
				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
			else if(strcmp((char *)receive, "open_led1") == 0)
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			else if(strcmp((char *)receive, "close_led1") == 0)
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			else if(strcmp((char *)receive, "open_led2") == 0)
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			else if(strcmp((char *)receive, "close_led2") == 0)
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			else if(strcmp((char *)receive, "open_beep") == 0)
				HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
			else if(strcmp((char *)receive, "close_beep") == 0)
				HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
			memset(receive, 0, 64);
		}
  }
  /* USER CODE END StartRS485WriteTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
