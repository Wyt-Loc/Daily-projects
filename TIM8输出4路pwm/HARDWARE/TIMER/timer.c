#include "timer.h"
#include "led.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTE��ӢSTM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,����TIM3_PWM_Init������
//2,����LED0_PWM_VAL�궨�壬����TIM3_CH2����									  
//////////////////////////////////////////////////////////////////////////////////  
   	  
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM8_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);	//ʹ�ܶ�ʱ��1ʱ��
	
	TIM_TimeBaseStructure.TIM_Period = arr;        //װ��ֵ18k/144=125hz
	TIM_TimeBaseStructure.TIM_Prescaler = psc;       //Ԥ��Ƶ(ʱ�ӷ�Ƶ)72M/(7199+1)=10khz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //���ϼ���

	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0; //���ظ��������ظ�������ٴβŸ�����һ������ж�,
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);
	TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE);
	
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;  //TIM8�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_Cmd(TIM8, ENABLE); 
							 
}

//��ʱ��3�жϷ������
void TIM8_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
			TIM_ClearITPendingBit(TIM8, TIM_IT_Update  );  //���TIMx�����жϱ�־ 	
				LED0 = !LED0;
		}
}

//TIM8 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
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
	
	
	
	//�˴��ṹ�����ȫ�����ã������������PWM����������
	TIM_OCInitStructure.TIM_Pulse = 0; 		//��װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //TIM8�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//ʹ������Ƚ�״̬
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM8 ����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//ʹ������Ƚ�N״̬
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//TIM8 ����Ƚ�N���Ը�
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//�� MOE=0 ���� TIM8 ����ȽϿ���״̬
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//�� MOE=0 ���� TIM8 ����Ƚ� N ����״̬
	
	TIM_OC1Init(TIM8, &TIM_OCInitStructure); //����Ƚ�ͨ��1��ʼ��  
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);//ʹ��TIMx�� CCR1 �ϵ�Ԥװ 
	
	TIM_OC2Init(TIM8, &TIM_OCInitStructure); //����Ƚ�ͨ��1��ʼ��  
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);//ʹ��TIMx�� CCR1 �ϵ�Ԥװ 
	
	TIM_OC3Init(TIM8, &TIM_OCInitStructure); //����Ƚ�ͨ��1��ʼ��  
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);//ʹ��TIMx�� CCR1 �ϵ�Ԥװ 
	
	TIM_OC4Init(TIM8, &TIM_OCInitStructure); //����Ƚ�ͨ��1��ʼ��  
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);//ʹ��TIMx�� CCR1 �ϵ�Ԥװ 
	
	TIM_ARRPreloadConfig(TIM8, ENABLE);//ʹ��Ԥװ�ؼĴ���
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
	TIM_Cmd(TIM8, ENABLE);  //ʹ�ܶ�ʱ��	
}
