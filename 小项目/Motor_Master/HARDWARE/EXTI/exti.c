#include "exti.h"
#include "delay.h" 
//#include "led.h" 
//#include "key.h"
//#include "beep.h"
#include "ov7725.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板V3
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/10
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved		
//********************************************************************************
//V1.1 20150118
//1,增加EXTI8_Init函数。
//2,增加EXTI9_5_IRQHandler函数											  
//////////////////////////////////////////////////////////////////////////////////  
u8 ov_sta;	//帧中断标记

//外部中断8初始化
void EXTI8_Init(void)
{												  
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);//PA8对中断线8

	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级0 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}


void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line8)==SET)	//是8线的中断
	{
		OV7725_WRST=0;  //复位写指针     
		OV7725_WRST=1;
		OV7725_WREN=1;  //允许写入FIFO    
		ov_sta++;		//帧中断加1
	}
	EXTI_ClearITPendingBit(EXTI_Line8);  //清除EXTI8线路挂起位						  
}












