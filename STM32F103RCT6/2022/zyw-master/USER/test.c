#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "adc.h" 
#include "key.h"
#include "timer.h"
#include "exti.h"
#include "math.h"
#include "stm32_dsp.h"
#include "table_fft.h"

#define NPT 1024   //����
#define PI2 6.28318530717959

void lcd_huadian(u16 a,u16 b);
void lcd_huaxian(u16 x1,u16 y1,u16 x2,u16 y2);
void lcd_init();
void clear_point();//������ʾ����ǰ��
void InitBufInArray();
void GetPowerMag();
void TIM3_IRQHandler(void);
void TIM6_IRQHandler(void);	
void EXTI0_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

u16 i;
u32 adcx[NPT];
u32 adcmax = 0;
u32 adcmin = 0;
int long fftin [NPT];
int long fftout[NPT];
u32 magout[NPT];
u32 magout2[NPT];
u16 temp=0;
u16 t=0;
u16 T=1000;//TIM3�Զ���װ��ֵ
u16 T1=1;//�����ʣ���λKHZ
u32 F=0;//Ƶ��ֵ
u16 U=41;
u16 V=660;//mv/div
u16 vpp=0;
u16 x=0;
u16 v=0;
u16 v1=0;
u8 flag=0;
u8 adc_flag=0;
u8 clear_flag=1;
u16 table[15] ={16,32,48,64,80,96,112,128,144,160,176,192,208,224,240};
u16 h=0;

 int main(void)
 { 
	Stm32_Clock_Init(9);	//ϵͳʱ������
	delay_init(72);	    	 //��ʱ������ʼ��	  
	uart_init(72,9600);	 	//���ڳ�ʼ��Ϊ9600
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�//PA8,PD2
 	LCD_Init();
 	Adc_Init();	 //PA2
	lcd_init(); //���û�����Ļ
	 //72��ƵΪ1M,T*T1=1000
	 //T=1000ʱ��1kHZ
	 //TIM3���Ʋ�����
	TIM3_Int_Init((T-1),71);    //(10-1,71)100kHz  (1000-1,71)1kHz
	 //PA0,PC5,PA15�ж�
	 //PC5���Ʋ�����
	 //PA15������ͣ�˶�
	EXTI_Init();
	 //ģ���������
//	InitBufInArray();
	printf("system is OK \r\n");		 
 
  	while(1) 
	{
		while(adc_flag == 0){}			//�ȴ����ݲɼ����
        adc_flag=0;
			//NPT��1024���㣬ȷ��max��min
		for(i=0;i<NPT;i++)
		{
		fftin[i] = 0;
		fftin[i] = adcx[i] << 16;
		
		if(adcx[i] >= adcmax)
			{			
			adcmax = adcx[i];
			}			
		if(adcx[i] <= adcmin)
			{			
			adcmin = adcx[i];
			}						
		}
		
		GetPowerMag();
		LCD_ShowNum(260,20,V,4,16);//mv/div
		adcmax=adcmax*0.8;   //0.8 = 3300/4096		
		LCD_ShowNum(260,75,adcmax-adcmin,4,16);				
		clear_point();    //������ʾ����ǰ��
						
		LED0=!LED0;	
		TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3			
		delay_ms(100);	
	} 
}

void lcd_huadian(u16 a,u16 b)
{							    
	LCD_Fast_DrawPoint(a,240-b,BLUE);
}
void lcd_huaxian(u16 x1,u16 y1,u16 x2,u16 y2)
{
	LCD_DrawLine(x1,240-y1,x2,240-y2);
}
void lcd_init()
{
		POINT_COLOR=RED;	  
		LCD_ShowString(5,8,200,24,24,"mini_OSC");	
		LCD_ShowString(128,12,200,16,16,"INPUT:PTA2");
	    LCD_ShowString(260,0,200,16,16,"mV/div:");
	    LCD_ShowString(260,55,200,16,16,"vpp(mv):");	
	    LCD_ShowString(260,165,200,16,16,"f(Hz):");
	    LCD_ShowString(260,200,200,16,16,"OSR:");  //������
	
		POINT_COLOR=BLUE;		
		
		lcd_huaxian(0,0,0,200);
		lcd_huaxian(256,0,256,200);
		lcd_huaxian(0,0,256,0);		
		lcd_huaxian(0,200,256,200);
	
		LCD_ShowNum(260,220,T1,4,16);
	  LCD_ShowString(295,220,200,16,16,"kHz");	
		LCD_ShowString(260,110,200,16,16,"ing...");
}

void clear_point()//������ʾ����ǰ��
{
	for(x=0;x<NPT/4 && clear_flag;x++)
	{
//		if(n<256)            x=n;
//		if(n>=256 && n<512)  x=n-256;
//		if(n>=512 && n<768)  x=n-512;
//		if(n>=768 && n<1024) x=n-768;	
	
		POINT_COLOR=WHITE;	
		lcd_huaxian(x  ,1,x  ,199);
		POINT_COLOR=BLUE;
		lcd_huaxian(0,0,0,200);
		lcd_huadian(x,100);
		if(x == table[h])	
		{
		lcd_huaxian(x,1,x,3);
		lcd_huaxian(x,101,x,103);
		lcd_huaxian(x,199,x,197);
		h++;
			if(h>=16) h=0;
		}	
		if(x==128) 
		{
		lcd_huaxian(x,1,x,199);
			for(i=20;i<200;i+=20)
			{
			lcd_huaxian(125,i,127,i);
			}
		}
		v = adcx[x]/U+(90-((adcmax-adcmin)/(2*U)));
//		printf("adcmax:%lu\r\nadcmin:%lu\r\n",adcmax,adcmin);
//		printf("v: %d\r\n",v);
		lcd_huaxian(x,v1,x+1,v);
		//lcd_huadian(x,v);
		v1 = v;
//        delay_ms(10);	
	}
}

/******************************************************************
��������:InitBufInArray()
��������:ģ��������ݣ����������а���3��Ƶ�����Ҳ�(350Hz��8400Hz��18725Hz)
����˵��:
��    ע:��lBufInArray�����У�ÿ�����ݵĸ�16λ�洢�������ݵ�ʵ����
          ��16λ�洢�������ݵ��鲿(����Ϊ0)
*******************************************************************/
void InitBufInArray()
{
    unsigned short i;
    float fx;
    for(i=0; i<NPT; i++)
    {
        fx = 50*sin(PI2 * i * 250.0 / 1000);
        magout2[i] = ((signed short)fx) << 16;
    }
}

/******************************************************************
��������:GetPowerMag()
��������:�������г����ֵ
����˵��:
������ע:�Ƚ�lBufOutArray�ֽ��ʵ��(X)���鲿(Y)��Ȼ������ֵ(sqrt(X*X+Y*Y)
*******************************************************************/
void GetPowerMag()
{
    float X,Y,Mag;
	float magmax=0;
    unsigned short i;
	magmax=0;
	//������cr4_fft_1024_stm32
	cr4_fft_1024_stm32(fftout, fftin, NPT);	
    for(i=1; i<NPT/2; i++)
    {
        X  = (fftout[i] << 16) >> 16;
        Y  = (fftout[i] >> 16);
        Mag = sqrt(X * X + Y * Y);     
		if(Mag > magmax)
			{
			magmax = Mag;
			temp = i;
			}				
    }
	if(T==1000)		F=(u32)((double)temp/NPT*1000  );	
	if(T==100)		F=(u32)((double)temp/NPT*10010 );
	if(T==10)		F=(u32)((double)temp/NPT*100200);
	if(T==2)		F=(u32)((double)temp/NPT*249760);
	
		LCD_ShowNum(280,180,F,5,16);		
//		LCD_ShowNum(280,200,temp,4,16);					
//		LCD_ShowNum(280,220,(u32)(magmax*2.95),5,16);			
}

//��ʱ��3�жϷ������	
//ÿ������Ƶ�ʲ���һ��
void TIM3_IRQHandler(void)
{ 
	if(TIM3->SR&0X0001)//����ж�
	{
		adcx[t]=Get_Adc(ADC_CH1); 	
		//printf("adcx[t]:%lu\r\n",adcx[t]);
		t++;
		if(t==NPT) 
			{
			t=0;
			adc_flag = 1;
            TIM3->CR1 &= (uint16_t)(~((uint16_t)0x0001));//ʧ�ܶ�ʱ��3	
			}
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}

//�ⲿ�ж�9~5�������
void EXTI9_5_IRQHandler(void)
{			
	delay_ms(10);   //����			 
    if(KEY0==0)		//����0
	{
		POINT_COLOR=BLUE;		
		flag++;
		if(flag==1)
		{
		T = 100;
		T1 = 10;
		LCD_ShowNum(260,220,T1,4,16);		
		}
		else if(flag==2) 
		     {
			  T=10;
			  T1 = 100;
			  LCD_ShowNum(260,220,T1,4,16);				 
		     }
			 else if(flag==3)
			      {
				   T=2;
				   T1 = 500;
		           LCD_ShowNum(260,220,T1,4,16);					  
			      }
				  else if(flag==4)
				       {
				        T=1000;
						T1 = 1;
						LCD_ShowNum(260,220,T1,4,16);						   
						flag=0;
				       }
 	TIM3->ARR=(T-1);  	//�趨�������Զ���װֵ 
//	LCD_ShowNum(280,220,flag,4,16);
	}
 	EXTI->PR=1<<5;     //���LINE5�ϵ��жϱ�־λ  
}
//�ⲿ�ж�15~10�������
void EXTI15_10_IRQHandler(void)
{		
	delay_ms(10);   //����			 
    if(KEY1==0)		//����1
	{
		if(clear_flag == 1)
		{
        TIM3->CR1 &= (uint16_t)(~((uint16_t)0x0001));//ʧ�ܶ�ʱ��3					
		clear_flag=0;
	    LCD_ShowString(260,110,200,16,16,"STOP  ");			
		}
		else
		{
		TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3				
		clear_flag=1;
	    LCD_ShowString(260,110,200,16,16,"ing...");
		}
	}
 	EXTI->PR=1<<15; //���LINE15�ϵ��жϱ�־λ  
}