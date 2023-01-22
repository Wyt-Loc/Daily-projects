#include "timer.h"
#include "led.h"
#include "usart.h"

void DIR_Init(void)		//DIR����PB5��ʼ��
{	
 GPIO_InitTypeDef  GPIO_InitStructure; //�ṹ������
	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��GPIOBͨ��
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//������� 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //Ƶ��50hz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 
 GPIO_SetBits(GPIOB,GPIO_Pin_5);		//Ĭ�ϸߵ�ƽ ������
 
}


void TIM4_PWM_Init(u16 arr,u16 psc)
{  
    //�ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	//ʹ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	  
	
   //���ø�����Ϊ�����������,���TIM4 CH1��PWM���岨��	GPIOB.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	
    //���ö�ʱ������
  TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ض���ģʽ�����������ݷ���λ(DIR)���ϼ���
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
    
		
    //�ж�����
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  
	
	
	//��ʼ��TIM4 Channel1 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;  //TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���,��TIM3_CCR2��Ԥװ��ֵ�ڸ����¼�����ʱ���ܱ���������ǰ�Ĵ����С�
	
    TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
}

//��ʱ��4�жϷ������
void TIM4_IRQHandler(void)   
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	}
}

