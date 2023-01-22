#include "Balance.h"
#include "stm32f4xx_gpio.h"


//编码器AB相处理
void Encoder_Init_TIM4(u16 arr,u16 psc)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;    

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);    
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  
		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12| GPIO_Pin_13;          
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                    
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;              
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                  
  GPIO_Init(GPIOD, &GPIO_InitStructure);                          

  GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);           
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);           
	
  TIM_TimeBaseStructure.TIM_Period = arr; 	                      
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                        
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;       
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;           
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                  
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;                  
  TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;	      
  TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;   
  TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;	            
  TIM_ICInitStructure.TIM_ICFilter =6;                            
  TIM_ICInit(TIM4,&TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;                  
  TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;	      
  TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;   
  TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;	            
  TIM_ICInitStructure.TIM_ICFilter=6;                             
  TIM_ICInit(TIM4,&TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising );//编码器配置（定时器、编码模式、上升沿、上升沿）
		
  NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;                   
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;      
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0x02;            
	NVIC_Init(&NVIC_InitStructure);                                 
		
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);                        
	TIM_Cmd(TIM4,ENABLE);   
}



