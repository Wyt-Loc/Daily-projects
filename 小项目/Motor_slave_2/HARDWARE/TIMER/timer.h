#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************

//���1��������
void TIM2_IRQHandler(void);
void TIM2_config(u32 PulseNum);
void TIM1_config(u32 Cycle);
void Pulse_output_1(u32 Cycle,u32 PulseNum);//Ƶ�� = 72M / 72 / Ŀ��Ƶ��     ����


//���2��������
void TIM4_IRQHandler(void);
void TIM3_config(u32 Cycle);
void TIM4_config(u32 PulseNum);
void Pulse_output_2(u32 Cycle,u32 PulseNum);//Ƶ�� = 72M / 72 / Ŀ��Ƶ��     ����




#endif
