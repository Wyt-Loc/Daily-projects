#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


#define KEY_HANG1 GPIO_Pin_0
#define KEY_HANG2 GPIO_Pin_1
#define KEY_HANG3 GPIO_Pin_2
#define KEY_HANG4 GPIO_Pin_3


#define KEY1  GPIO_Pin_4 
#define KEY2  GPIO_Pin_5
#define KEY3  GPIO_Pin_6
#define KEY4  GPIO_Pin_7


#define KEY1_Input  GPIO_ReadInputDataBit(KEY_PROT,KEY1)
#define KEY2_Input  GPIO_ReadInputDataBit(KEY_PROT,KEY2)
#define KEY3_Input  GPIO_ReadInputDataBit(KEY_PROT,KEY3)
#define KEY4_Input  GPIO_ReadInputDataBit(KEY_PROT,KEY4)
 
 
#define KEY_PROT GPIOA

extern  u8  flag;  


u16 *abc_read();
int sun1_read();
void KEY_Init(void);//IO初始化
void KEY_config(u8 *key);
u8 Read_KeyValue(void);
void Key_Input( u8 *key1);
					    
#endif
