#include "timer.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);   
	
//定时器设置-------------------------------------------------------------	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  							 //预分频		
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=arr;   										//重装载值	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			//时钟分割
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 										     
	TIM_Cmd(TIM3,ENABLE); 																           
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; 									 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; 			 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; 						 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}



//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
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

//0.5s执行一次
u32 Read_Encoder(void)
{
  u32 Count;                                                      
  u16 Current_Count;                                              
	u16 Enc_Timer_Overflow_one;	                                   

  Enc_Timer_Overflow_one=Encoder_Timer_Overflow;                  
  Current_Count = TIM_GetCounter(TIM4);                           
  Encoder_Timer_Overflow=0;            
// 编码器分辨率 = 30 * 500 = 15000  电机轴转一圈产生15000个脉冲  使用4倍频 所以为 15000 * 4 = 60000      单位时间内编码器脉冲数	
	if((TIM4->CR1&0x0010) == 0x0010)                                
    Count = (u32)((-1*Enc_Timer_Overflow_one)*(4*500-4) + (Current_Count - Previous_Count));  
	else                                                            
		Count = (u32)(Current_Count - Previous_Count + (Enc_Timer_Overflow_one) * (4*500-4)); 
		     
  Previous_Count = Current_Count;  
  return(Count);
}




