#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
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
void KEY_Init(void);//IO��ʼ��
void KEY_config(u8 *key);
u8 Read_KeyValue(void);
void Key_Input( u8 *key1);
					    
#endif
