#include "timer.h"
#include "led.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTE精英STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,增加TIM3_PWM_Init函数。
//2,增加LED0_PWM_VAL宏定义，控制TIM3_CH2脉宽									  
//////////////////////////////////////////////////////////////////////////////////  
   	  
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM8_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);	//使能定时器1时钟
	
	TIM_TimeBaseStructure.TIM_Period = arr;        //装载值18k/144=125hz
	TIM_TimeBaseStructure.TIM_Prescaler = psc;       //预分频(时钟分频)72M/(7199+1)=10khz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //向上计数

	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0; //是重复计数，重复溢出多少次才给你来一个溢出中断,
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);
	TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE);
	
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;  //TIM8中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //抢占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_Cmd(TIM8, ENABLE); 
							 
}

//定时器3中断服务程序
void TIM8_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
			TIM_ClearITPendingBit(TIM8, TIM_IT_Update  );  //清除TIMx更新中断标志 	
				LED0 = !LED0;
		}
}

//TIM8 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM8_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);  
	                                                                     	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); 
	
	
	
	//此处结构体必须全部配置，否则输出不了PWM波！！！！
	TIM_OCInitStructure.TIM_Pulse = 0; 		//待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //TIM8脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//使能输出比较状态
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM8 输出比较极性高
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//使能输出比较N状态
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//TIM8 输出比较N极性高
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//当 MOE=0 重置 TIM8 输出比较空闲状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//当 MOE=0 重置 TIM8 输出比较 N 空闲状态
	
	TIM_OC1Init(TIM8, &TIM_OCInitStructure); //输出比较通道1初始化  
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);//使能TIMx在 CCR1 上的预装 
	
	TIM_OC2Init(TIM8, &TIM_OCInitStructure); //输出比较通道1初始化  
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);//使能TIMx在 CCR1 上的预装 
	
	TIM_OC3Init(TIM8, &TIM_OCInitStructure); //输出比较通道1初始化  
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);//使能TIMx在 CCR1 上的预装 
	
	TIM_OC4Init(TIM8, &TIM_OCInitStructure); //输出比较通道1初始化  
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);//使能TIMx在 CCR1 上的预装 
	
	TIM_ARRPreloadConfig(TIM8, ENABLE);//使能预装载寄存器
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
	TIM_Cmd(TIM8, ENABLE);  //使能定时器	
}
