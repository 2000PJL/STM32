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
#include "adc.h"
#include "stdlib.h"

uint16_t ADC_Value1=0,ADC_LIGHT=0;
uint8_t str_buff[64];//��ʾ
DHT11_Data_TypeDef DHT11_Data;

			
void UR1_Send_Info1(){
  sprintf((char *)str_buff,"�����������ɼ��������-->����ֵ:%d,����ֵ:%d.%d%dV\r\n",ADC_Value1,ADC_LIGHT/100,(ADC_LIGHT%100)/10,ADC_LIGHT%10);	
	HAL_UART_Transmit(&huart1,str_buff,sizeof(str_buff),1000);
}

char txbuf[250];

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
osThreadId WIFITaskHandle;
osThreadId LightTaskHandle;
osThreadId DHT11TaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartWIFITask(void const * argument);
void StartLightTask(void const * argument);
void StartDHT11Task(void const * argument);

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
  /* definition and creation of WIFITask */
  osThreadDef(WIFITask, StartWIFITask, osPriorityAboveNormal, 0, 128);
  WIFITaskHandle = osThreadCreate(osThread(WIFITask), NULL);

  /* definition and creation of LightTask */
  osThreadDef(LightTask, StartLightTask, osPriorityIdle, 0, 128);
  LightTaskHandle = osThreadCreate(osThread(LightTask), NULL);

  /* definition and creation of DHT11Task */
  osThreadDef(DHT11Task, StartDHT11Task, osPriorityNormal, 0, 256);
  DHT11TaskHandle = osThreadCreate(osThread(DHT11Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartWIFITask */
/**
  * @brief  Function implementing the WIFITask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartWIFITask */
void StartWIFITask(void const * argument)
{
  /* USER CODE BEGIN StartWIFITask */
  /* Infinite loop */
	printf("�Ѿ�����WIFI�߳�\r\n");
  for(;;)
  { 
		LED2_ON();
    wifi_sendMessage(txbuf);
		osDelay(1000);
		LED2_OFF();
		osDelay(1000);
  }
  /* USER CODE END StartWIFITask */
}

/* USER CODE BEGIN Header_StartLightTask */
/**
* @brief Function implementing the LightTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLightTask */
void StartLightTask(void const * argument)
{
  /* USER CODE BEGIN StartLightTask */
  /* Infinite loop */
	printf("�Ѿ�����Light�߳�\r\n");
  for(;;)
  {
    HAL_ADC_Start(&hadc1);
		LED0_ON();
		if(HAL_ADC_PollForConversion(&hadc1,10)==HAL_OK)
		{
			ADC_Value1=HAL_ADC_GetValue(&hadc1);
			ADC_LIGHT=ADC_Value1*330/4096;
		}
		UR1_Send_Info1();
    osDelay(1000);
		LED0_OFF();
		osDelay(1000);
		HAL_ADC_Stop(&hadc1);

  }
  /* USER CODE END StartLightTask */
}

/* USER CODE BEGIN Header_StartDHT11Task */
/**
* @brief Function implementing the DHT11Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDHT11Task */
void StartDHT11Task(void const * argument)
{
  /* USER CODE BEGIN StartDHT11Task */
  /* Infinite loop */
	printf("�Ѿ�����DHT11�߳�\r\n");
  for(;;)
  {
		if(DHT11_Read_TempAndHumidity(&DHT11_Data)==SUCCESS)
    {
			
			LED1_ON();
			//sprintf(txbuf,"people=%d,dist=%.1f,light=%.lf,hum=%.1f,temp=%.lf \r\n",rand()%2,((double)(rand()%100))/10,((double)ADC_Value1),DHT11_Data.temperature,DHT11_Data.humidity);
      sprintf(txbuf,"%sDHT11���ݲɼ����-->ʪ��Ϊ%.1f %% ���¶�Ϊ %.1f���϶� \r\n",str_buff,DHT11_Data.humidity,DHT11_Data.temperature);			
			printf((char *)txbuf);
			osDelay(1000);
      LED1_OFF();
			osDelay(1000);
    }
    else
    {
      printf("��ȡDHT11��Ϣʧ��\r\n");      
    }
    osDelay(2000);
  }
  /* USER CODE END StartDHT11Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
