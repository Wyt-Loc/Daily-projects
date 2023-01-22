#include "timer.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);   
	
//��ʱ������-------------------------------------------------------------	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  							 //Ԥ��Ƶ		
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period=arr;   										//��װ��ֵ	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			//ʱ�ӷָ�
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 										     
	TIM_Cmd(TIM3,ENABLE); 																           
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; 									 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; 			 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; 						 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}



//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM5_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);            	
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  										
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 	
	TIM_TimeBaseInitStructure.TIM_Period=arr;   										
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); 										     
	TIM_Cmd(TIM5,ENABLE); 																           
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; 									 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; 			 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; 						 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


u16 Encoder_Timer_Overflow = 0;
u32 Previous_Count = 0;
void TIM4_IRQHandler(void)
{
		if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)                    
	{   
		Encoder_Timer_Overflow++;     		
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  
}

//0.5sִ��һ��
u32 Read_Encoder(void)
{
  u32 Count;                                                      
  u16 Current_Count;                                              
	u16 Enc_Timer_Overflow_one;	                                   

  Enc_Timer_Overflow_one=Encoder_Timer_Overflow;                  
  Current_Count = TIM_GetCounter(TIM4);                           
  Encoder_Timer_Overflow=0;            
// �������ֱ��� = 30 * 500 = 15000  �����תһȦ����15000������  ʹ��4��Ƶ ����Ϊ 15000 * 4 = 60000      ��λʱ���ڱ�����������	
	if((TIM4->CR1&0x0010) == 0x0010)                                
    Count = (u32)((-1*Enc_Timer_Overflow_one)*(4*500-4) + (Current_Count - Previous_Count));  
	else                                                            
		Count = (u32)(Current_Count - Previous_Count + (Enc_Timer_Overflow_one) * (4*500-4)); 
		     
  Previous_Count = Current_Count;  
  return(Count);
}




