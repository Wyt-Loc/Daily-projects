#ifndef __Motor_H
#define __Motor_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
///////////////////////////////////////////////////////////////////////////////////
#define EN1  PBout(0)// PB0

#define DIR1 PBout(3)// PB1


void CLK_Init(void);//�������ʹ��
void EN_Init(void);//ʹ�����ų�ʼ��
void DIR_Init(void);//�����ʼ��
void Encoder_EXTIX_Init(void);
void TIM_6_init(u16 arr,u16 psc);
float read_sudu(int TIMX);

void main_Init(void);


#endif
