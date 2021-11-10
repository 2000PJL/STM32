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
#include "dht11.h"
#include "stdio.h"
#include "esp8266.h"
#include "string.h"

extern short wifi_count;
extern uint8_t wifi_rxbuf[Wifi_BufSize];

DHT11_Data_TypeDef dht11_data;
char tx_buf[64];
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
osThreadId WIFITaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDHT11Task(void const * argument);
void StartWIFITask(void const * argument);

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

  /* definition and creation of WIFITask */
  osThreadDef(WIFITask, StartWIFITask, osPriorityIdle, 0, 128);
  WIFITaskHandle = osThreadCreate(osThread(WIFITask), NULL);

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
	//osThreadSuspend(DHT11TaskHandle);
  for(;;)
  { 
		
    if(DHT11_Read_TempAndHumidity(&dht11_data)==SUCCESS)
		{
			sprintf(tx_buf, "%.2f %.2f\n", dht11_data.temperature, dht11_data.humidity);
            Wifi_SendData("AT+CIPSEND=0,12\r\n");
            printf("%s\r\n", tx_buf);
            Wifi_SendData(tx_buf);
            memset(tx_buf, 0, sizeof(tx_buf));
			
		}
		else 
		{
			printf("读取失败\r\n");
			
		}
		osDelay(2000);
  }
  /* USER CODE END StartDHT11Task */
}

/* USER CODE BEGIN Header_StartWIFITask */
/**
* @brief Function implementing the WIFITask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartWIFITask */
void StartWIFITask(void const * argument)
{
  /* USER CODE BEGIN StartWIFITask */
  /* Infinite loop */
  for(;;)
  {
      	if(wifi_count == -1)
		{
			wifi_count = 0;
			printf("%s\r\n", wifi_rxbuf);
      if(strcmp((char *)wifi_rxbuf, "open_led1") == 0)
      {
         LED0_ON();
      }
      else if(strcmp((char *)wifi_rxbuf, "close_led1") == 0)
      {
         LED0_OFF();
      }
      else if(strcmp((char *)wifi_rxbuf, "open_led2") == 0)
      {
         LED1_ON();
      }
      else if(strcmp((char *)wifi_rxbuf, "close_led2") == 0)
      {
         LED1_OFF();
      }
      else if(strcmp((char *)wifi_rxbuf, "open_led3") == 0)
      {
         LED2_ON();
      }
            else if(strcmp((char *)wifi_rxbuf, "close_led3") == 0)
            {
                LED2_OFF();
            }
            else if(strcmp((char *)wifi_rxbuf, "open_beep") == 0)
            {
                BEEP_ON();
            }
            else if(strcmp((char *)wifi_rxbuf, "close_beep") == 0)
            {
                BEEP_OFF();
            }
            else if(strcmp((char *)wifi_rxbuf, "open_dht11") == 0)
            {
                // 恢复温湿度读取的线程
                osThreadResume(DHT11TaskHandle);
            }
            else if(strcmp((char *)wifi_rxbuf, "close_dht11") == 0)
            {
                // 挂起温湿度读取的线程
                osThreadSuspend(DHT11TaskHandle);
            }
            else if(strcmp((char *)wifi_rxbuf, "reset_all") == 0)
            {
                LED0_OFF();
							  LED1_OFF();
							  LED2_OFF();
						  	BEEP_OFF();
                osThreadSuspend(DHT11TaskHandle);
            }
            memset(wifi_rxbuf, 0, sizeof(wifi_rxbuf));//清空wifi_rxbuf中的数据
		}
  }
  /* USER CODE END StartWIFITask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
