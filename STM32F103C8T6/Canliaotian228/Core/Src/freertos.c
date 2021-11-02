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
#include "can.h"
#include "string.h"
#include "key.h"
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
osThreadId CanReceiveTaskHandle;
osThreadId CanSendTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartCanReceiveTask(void const * argument);
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
  /* definition and creation of CanReceiveTask */
  osThreadDef(CanReceiveTask, StartCanReceiveTask, osPriorityBelowNormal, 0, 128);
  CanReceiveTaskHandle = osThreadCreate(osThread(CanReceiveTask), NULL);

  /* definition and creation of CanSendTask */
  osThreadDef(CanSendTask, StartCanSendTask, osPriorityNormal, 0, 128);
  CanSendTaskHandle = osThreadCreate(osThread(CanSendTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartCanReceiveTask */
/**
  * @brief  Function implementing the CanReceiveTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartCanReceiveTask */
void StartCanReceiveTask(void const * argument)
{
  /* USER CODE BEGIN StartCanReceiveTask */
	uint8_t message[8] = {0};
  /* Infinite loop */
  for(;;)
  {
		if(get_canMessage(message) == 1)
		{
			printf("收到的控制指令为%s,长度为%d\r\n",(char*)message,strlen((char*)message));
	        if(strcmp((char *)message,"cled2")==0) 
			{
				HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
				printf("改变LED2的状态\r\n");
			}
			else if(strcmp((char *)message,"cbeep")==0) 
			{
				HAL_GPIO_TogglePin(BEEP_GPIO_Port,BEEP_Pin);
				printf("改变蜂鸣器的状态\r\n");
			}
			else if(strcmp((char *)message,"复位")==0)
			{
						HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET);
				        HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET);		
			}
		}
		osDelay(10);
  }
  /* USER CODE END StartCanReceiveTask */
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
	can1_config();
	uint8_t data[8] = {0};
	sprintf((char *)data,"call");
	CAN_TRANSMIT1(data);
	printf("data=%s",data);
  /* Infinite loop */
  for(;;)
  {
		if(get_keyValue()==0)
		{
			sprintf((char *)data,"cled0");
			printf("发送的消息为：%s\r\n",data);
			CAN_TRANSMIT1(data);
			keyValue_Reset();
		}else if(get_keyValue()==1)
		{
			sprintf((char *)data,"cled1");
			printf("发送的消息为：%s\r\n",data);
			CAN_TRANSMIT1(data);
			keyValue_Reset();
		}else if(get_keyValue()==2)
		{
			sprintf((char *)data,"cled2");
			printf("发送的消息为：%s\r\n",data);
			CAN_TRANSMIT1(data);
			keyValue_Reset();
		}else if(get_keyValue()==3)
		{
			sprintf((char *)data,"cbeep");
			printf("发送的消息为：%s\r\n",data);
			CAN_TRANSMIT1(data);
			keyValue_Reset();
		}else if(get_keyValue()==4)
		{
			sprintf((char *)data,"复位");
			printf("发送的消息为：%s\r\n",data);
			CAN_TRANSMIT1(data);
			keyValue_Reset();
		}
		osDelay(10);
  }
  /* USER CODE END StartCanSendTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
