/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "esp8266.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include <stdarg.h>

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
UART_HandleTypeDef husartx_esp8266;

STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };
uint8_t esp8266_rxdata;
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
static char * itoa( int value, char * string, int radix );

/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ��ʼ��ESP8266�õ���GPIO����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void ESP8266_GPIO_Config ( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  ESP8266_RST_GPIO_ClK_ENABLE();
  ESP8266_USARTx_GPIO_ClK_ENABLE();
  
  /* �������蹦��GPIO���� */
  GPIO_InitStruct.Pin = ESP8266_USARTx_Tx_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(ESP8266_USARTx_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ESP8266_USARTx_Rx_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ESP8266_USARTx_Port, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(ESP8266_RST_PORT,ESP8266_RST_PIN,GPIO_PIN_RESET);   
  GPIO_InitStruct.Pin = ESP8266_RST_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(ESP8266_RST_PORT, &GPIO_InitStruct);
}


/**
  * ��������: ��ʼ��ESP8266�õ��� USART
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void ESP8266_USART_Config ( void )
{	
  /* ��������ʱ��ʹ�� */
  ESP8266_USART_RCC_CLK_ENABLE();

  ESP8266_GPIO_Config();
  
	husartx_esp8266.Instance = ESP8266_USARTx;
  husartx_esp8266.Init.BaudRate = ESP8266_USARTx_BAUDRATE;
  husartx_esp8266.Init.WordLength = UART_WORDLENGTH_8B;
  husartx_esp8266.Init.StopBits = UART_STOPBITS_1;
  husartx_esp8266.Init.Parity = UART_PARITY_NONE;
  husartx_esp8266.Init.Mode = UART_MODE_TX_RX;
  husartx_esp8266.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husartx_esp8266.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&husartx_esp8266);
  
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(ESP8266_USARTx_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(ESP8266_USARTx_IRQn);
  
  /* ʹ�ܽ��գ������жϻص����� */
  HAL_UART_Receive_IT(&husartx_esp8266,&esp8266_rxdata,1);
}

/**
  * ��������: ��ʽ�������������C���е�printf��������û���õ�C��
  * �������: USARTx ����ͨ��������ֻ�õ��˴���2����USART2
  *		        Data   Ҫ���͵����ڵ����ݵ�ָ��
  *			      ...    ��������
  * �� �� ֵ: ��
  * ˵    ��������Ӧ�� USART2_printf( USART2, "\r\n this is a demo \r\n" );
  *            		     USART2_printf( USART2, "\r\n %d \r\n", i );
  *            		     USART2_printf( USART2, "\r\n %s \r\n", j );
  */
void USART_printf(USART_TypeDef * USARTx, char * Data, ... )
{
	const char *s;
	int d;   
	char buf[16];
	uint8_t txdata;
  
	va_list ap;
	va_start(ap, Data);
	while ( * Data != 0 )     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( * Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //�س���
          txdata=0x0d;
					HAL_UART_Transmit(&husartx_esp8266,&txdata,1,0xFF);
					Data ++;
				break;
				case 'n':							          //���з�
          txdata=0x0a;
					HAL_UART_Transmit(&husartx_esp8266,&txdata,1,0xFF);
					Data ++;
				break;
				default:
					Data ++;
				break;
			}			 
		}
		else if ( * Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //�ַ���
					s = va_arg(ap, const char *);
					for ( ; *s; s++) 
					{
				  	HAL_UART_Transmit(&husartx_esp8266,(uint8_t *)s,1,0xFF);
            while(__HAL_UART_GET_FLAG(&husartx_esp8266,UART_FLAG_TXE)==0);
					}				
					Data++;				
				break;
				case 'd':			
					//ʮ����
					d = va_arg(ap, int);					
					itoa(d, buf, 10);					
					for (s = buf; *s; s++) 
					{
						HAL_UART_Transmit(&husartx_esp8266,(uint8_t *)s,1,0xFF);
						while(__HAL_UART_GET_FLAG(&husartx_esp8266,UART_FLAG_TXE)==0);
					}					
					Data++;				
				break;				
				default:
					Data++;				
				break;				
			}		 
		}		
		else
    {
      HAL_UART_Transmit(&husartx_esp8266,(uint8_t *)Data,1,0xFF);
      Data++;
		  while(__HAL_UART_GET_FLAG(&husartx_esp8266,UART_FLAG_TXE)==0);
    }
	}
}

/**
  * ��������: ����������ת�����ַ���
  * �������: radix =10 ��ʾ10���ƣ��������Ϊ0
  *           value Ҫת����������
  *           buf ת������ַ���
  *           radix = 10
  * �� �� ֵ: ��
  * ˵    ������USART_printf()����
  */
static char * itoa( int value, char *string, int radix )
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;
	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}
	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}
	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';
		/* Make the value positive. */
		value *= -1;
	}
	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;
		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}
	/* Null terminate the string. */
	*ptr = 0;
	return string;
} /* NCL_Itoa */

/**
  * ��������: ESP8266��ʼ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void ESP8266_Init ( void )
{
	ESP8266_GPIO_Config ();	
	ESP8266_USART_Config ();	
}

/**
  * ��������: ֹͣʹ��ESP8266
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void  ESP8266_stop( void )
{
	ESP8266_RST_LOW();
	
	HAL_NVIC_DisableIRQ(ESP8266_USARTx_IRQn);
  
	ESP8266_USART_RCC_CLK_DISABLE();
}

/**
  * ��������: ����ESP8266ģ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������ESP8266_AT_Test����
  */
void ESP8266_Rst ( void )
{
#if 0
	 ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 );   	
#else
	 ESP8266_RST_LOW();
	 HAL_Delay( 500 ); 
	 ESP8266_RST_HIGH(); 
#endif
}

/**
  * ��������: ��ESP8266ģ�鷢��ATָ��
  * �������: cmd�������͵�ָ��
  *           reply1��reply2���ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
  *           waittime���ȴ���Ӧ��ʱ��
  * �� �� ֵ: 1��ָ��ͳɹ�
  *           0��ָ���ʧ��
  * ˵    ������
  */
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, uint32_t waittime )
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //���¿�ʼ�����µ����ݰ�

	ESP8266_Usart ( "%s\r\n", cmd );

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //����Ҫ��������
		return true;
	
	HAL_Delay( waittime );                 //��ʱ
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';

	PC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
//  printf("%s->%s\n",cmd,strEsp8266_Fram_Record .Data_RX_BUF);
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
	
}

/**
  * ��������: ��ESP8266ģ�����AT��������
  * �������: ��
  * �� �� ֵ: 1��ѡ��ɹ�
  *           0��ѡ��ʧ��
  * ˵    ������
  */
bool ESP8266_AT_Test ( void )
{

	
	ESP8266_RST_HIGH();	
	HAL_Delay(1000 );

	if(ESP8266_Cmd("AT","OK",NULL,1000)) return 1;
	HAL_Delay(7000);	

	return 1;
}

/**
  * ��������: ѡ��ESP8266ģ��Ĺ���ģʽ
  * �������: enumMode������ģʽ
  * �� �� ֵ: 1��ѡ��ɹ�
  *           0��ѡ��ʧ��
  * ˵    ������
  */
bool ESP8266_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode )
{
	   
   	bool result=0;
		switch ( enumMode )
		{
			case STA:
				result=ESP8266_Cmd( "AT+CWMODE=1", "OK", "no change", 2500 ); 
		  	printf("esp8266��WiFi����ģʽ%d\r\n",result);
			break;
			case AP:
				result=ESP8266_Cmd( "AT+CWMODE=2", "OK", "no change", 2500 );
        printf("esp8266��APģʽ%d\r\n",result);			
			break;
			case STA_AP:
				result=ESP8266_Cmd( "AT+CWMODE=3", "OK", "no change", 2500 );
	      printf("esp8266�򿪻��ģʽ%d\r\n",result);			
			break;
			default:
				result=false;
			break;
		}
		
		if(result) return result;

	return result;
}

/**
  * ��������: ESP8266ģ�������ⲿWiFi
  * �������: pSSID��WiFi�����ַ���
  *           pPassWord��WiFi�����ַ���
  * �� �� ֵ: 1�����ӳɹ�
  *           0������ʧ��
  * ˵    ������
  */
bool ESP8266_JoinAP ( char * pSSID, char * pPassWord )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );

	if(ESP8266_Cmd(cCmd,"OK",NULL,5000))return 1;

	return 1;	
}

/**
  * ��������: ESP8266ģ�鴴��WiFi�ȵ�
  * �������: pSSID��WiFi�����ַ���
  *           pPassWord��WiFi�����ַ���
  *           enunPsdMode��WiFi���ܷ�ʽ�����ַ���
  * �� �� ֵ: 1�������ɹ�
  *           0������ʧ��
  * ˵    ������
  */
bool ESP8266_BuildAP ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode )
{
	char cCmd [120];
	char count=0;
	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode );
	while(count<10)
	{
		if(ESP8266_Cmd(cCmd,"OK",0,1000))return 1;
		++count;
	}
	return 0;	
}

/**
  * ��������: ESP8266ģ������������
  * �������: enumEnUnvarnishTx�������Ƿ������
  * �� �� ֵ: 1�����óɹ�
  *           0������ʧ��
  * ˵    ������
  */
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];

	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );

	if(ESP8266_Cmd(cStr,"OK",0,500))return 1;

	
	return 1;		
}

/**
  * ��������: ESP8266ģ�������ⲿ������
  * �������: enumE������Э��
  *           ip��������IP�ַ���
  *           ComNum���������˿��ַ���
  *           id��ģ�����ӷ�������ID
  * �� �� ֵ: 1�����ӳɹ�
  *           0������ʧ��
  * ˵    ������
  */
bool ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];

  switch (  enumE )
  {
		case enumTCP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
		  break;
		
		case enumUDP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
		  break;
		
		default:
			break;
  }

  if ( id < 5 )
    sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);
 
  else
	  sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

	return ESP8266_Cmd ( cCmd, "OK", "ALREAY CONNECT", 4000 );
	
}

/**
  * ��������: ESP8266ģ�鿪����رշ�����ģʽ
  * �������: enumMode������/�ر�
  *           pPortNum���������˿ں��ַ���
  *           pTimeOver����������ʱʱ���ַ�������λ����
  * �� �� ֵ: 1�������ɹ�
  *           0������ʧ��
  * ˵    ������
  */
bool ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver )
{
	char cCmd1 [120], cCmd2 [120];

	if ( enumMode )
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
		
		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );

		return ( ESP8266_Cmd ( cCmd1, "OK", 0, 500 ) &&
						 ESP8266_Cmd ( cCmd2, "OK", 0, 500 ) );
	}
	
	else
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );

		return ESP8266_Cmd ( cCmd1, "OK", 0, 500 );
	}
}

/**
  * ��������: ��ȡESP8266 ������״̬�����ʺϵ��˿�ʱʹ��
  * �������: ��
  * �� �� ֵ: 2�����ip
  *           3����������
  *           4��ʧȥ����
  *           0����ȡ״̬ʧ��
  * ˵    ������
  */
uint8_t ESP8266_Get_LinkStatus ( void )
{
	if ( ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:2\r\n" ) )
			return 2;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:3\r\n" ) )
			return 3;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:4\r\n" ) )
			return 4;		
	}
	return 0;
}

/**
  * ��������: ��ȡESP8266 �Ķ˿ڣ�Id������״̬�����ʺ϶�˿�ʱʹ��
  * �������: ��
  * �� �� ֵ: �˿ڣ�Id��������״̬����5λΪ��Чλ���ֱ��ӦId5~0��ĳλ����1���Id���������ӣ�������0���Idδ��������
  * ˵    ������
  */
uint8_t ESP8266_Get_IdLinkStatus ( void )
{
	uint8_t ucIdLinkStatus = 0x00;
	
	
	if ( ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:0," ) )
			ucIdLinkStatus |= 0x01;
		else 
			ucIdLinkStatus &= ~ 0x01;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:1," ) )
			ucIdLinkStatus |= 0x02;
		else 
			ucIdLinkStatus &= ~ 0x02;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:2," ) )
			ucIdLinkStatus |= 0x04;
		else 
			ucIdLinkStatus &= ~ 0x04;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:3," ) )
			ucIdLinkStatus |= 0x08;
		else 
			ucIdLinkStatus &= ~ 0x08;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:4," ) )
			ucIdLinkStatus |= 0x10;
		else 
			ucIdLinkStatus &= ~ 0x10;	
	}
	return ucIdLinkStatus;
}

/**
  * ��������: ��ȡESP8266 �� AP IP
  * �������: pApIp����� AP IP ��������׵�ַ
  *           ucArrayLength����� AP IP ������ĳ���
  * �� �� ֵ: 1����ȡ�ɹ�
  *           0����ȡʧ��
  * ˵    ������
  */
uint8_t ESP8266_Inquire_ApIp ( char * pApIp, uint8_t ucArrayLength )
{
	char uc;
	
	char * pCh;
	
	
  ESP8266_Cmd ( "AT+CIFSR", "OK", 0, 500 );
	
	pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "APIP,\"" );
	
	if ( pCh )
		pCh += 6;
	
	else
		return 0;
	
	for ( uc = 0; uc < ucArrayLength; uc ++ )
	{
		pApIp [ uc ] = * ( pCh + uc);
		
		if ( pApIp [ uc ] == '\"' )
		{
			pApIp [ uc ] = '\0';
			break;
		}
	}
	return 1;
}

/**
  * ��������: ����ESP8266ģ�����͸������
  * �������: ��
  * �� �� ֵ: 1�����óɹ�
  *           0������ʧ��
  * ˵    ������
  */
bool ESP8266_UnvarnishSend ( void )
{
	
	if ( ! ESP8266_Cmd ( "AT+CIPMODE=1", "OK", 0, 1000 ) ){}
	HAL_Delay(10000);
	

	return  ESP8266_Cmd ( "AT+CIPSEND", "OK", ">", 1000 );
	
}

/**
  * ��������: ����ESP8266ģ���˳�͸��ģʽ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void ESP8266_ExitUnvarnishSend ( void )
{
	HAL_Delay(1000);	
	ESP8266_Usart("+++");	
	HAL_Delay(500); 
	
}

/**
  * ��������: ESP8266ģ�鷢���ַ���
  * �������: enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
  *           pStr��Ҫ���͵��ַ���
  *           ucId���ĸ�ID���͵��ַ���
  *           ulStrLength��Ҫ���͵��ַ������ֽ���
  * �� �� ֵ: 1�����ͳɹ�
  *           0������ʧ��
  * ˵    ������
  */
bool ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	bool bRet = false;
	
		
	if ( enumEnUnvarnishTx )
	{
		ESP8266_Usart ( "%s\n", pStr );
		
		bRet = true;
		
	}

	else
	{
		if ( ucId < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );
		
		ESP8266_Cmd ( cStr, "> ", 0, 1000 );

		bRet = ESP8266_Cmd ( pStr, "SEND OK", 0, 1000 );
  }
	
	return bRet;

}

/**
  * ��������: ESP8266ģ������ַ���
  * �������: enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
  * �� �� ֵ: ���յ����ַ����׵�ַ
  * ˵    ������
  */
char * ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx )
{
	char * pRecStr = 0;
	
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	
	while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag );
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	
	if ( enumEnUnvarnishTx )
		pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
	else 
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;

	}
	return pRecStr;	
}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
