#include "Motor.h"
#include <stdio.h>
#include "timer.h"
#include "spi.h"




// 1  A12 A13   2  A14 A15    3  F12 F13   4  F14  F15   4��
void Encoder_GPIO_L298N(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;			 //LED0-->PB.5 �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;			 //LED0-->PB.5 �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOF, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
}



/*
//TIM4 CH1 PWM �������
//PWM �����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
STM32F103ZET6
PB6 - TIM4_CH1
PB7 - TIM4_CH2
PB8 - TIM4_CH3
PB9 - TIM4_CH4
*/
void Tim4_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ�� TIMx ����
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ�� PB ʱ��
	TIM_DeInit(TIM4);
 
  //���ø�����Ϊ�����������,��� TIM4 CH1 �� PWM ���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM4_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //���ù������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ�� GPIO
 
	TIM_TimeBaseStructure.TIM_Period = arr; //�����Զ���װ������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����Ԥ��Ƶֵ ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //��ʼ�� TIMx
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH1 PWM2 ģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	
	TIM_OCInitStructure.TIM_Pulse = 200; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //OC1 �ߵ�ƽ��Ч
	TIM_OC1Init(TIM4, &TIM_OCInitStructure); //����ָ���Ĳ�����ʼ������ TIMx
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //CH1 Ԥװ��ʹ��
	
	TIM_OCInitStructure.TIM_Pulse = 400; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //OC2 �ߵ�ƽ��Ч
	TIM_OC2Init(TIM4, &TIM_OCInitStructure); //����ָ���Ĳ�����ʼ������ TIMx
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //CH2 Ԥװ��ʹ��
	
	TIM_OCInitStructure.TIM_Pulse = 600; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //OC1 �ߵ�ƽ��Ч
	TIM_OC3Init(TIM4, &TIM_OCInitStructure); //����ָ���Ĳ�����ʼ������ TIMx
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //CH3 Ԥװ��ʹ��
	
	TIM_OCInitStructure.TIM_Pulse = 800; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //OC1 �ߵ�ƽ��Ч
	TIM_OC4Init(TIM4, &TIM_OCInitStructure); //����ָ���Ĳ�����ʼ������ TIMx
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //CH4 Ԥװ��ʹ��
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ�� TIMx �� ARR �ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM4, ENABLE); //ʹ�� TIMx
}


 int zuo1js = 0;
 int zuo2js = 0;
 int zuo3js = 0;
 int zuo4js = 0;


//������1 ��ʼ�� B1 B2
void Encoder_exit_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource2);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line1 | EXTI_Line2;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//�����ش���
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}


//������4 ��ʼ�� A1 A2
void Encoder_EXit_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource4);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line3 | EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//�����ش���
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}


//������1 B1 B2
void EXTI1_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line1) != RESET) //����A�� PB12
	{
		EXTI_ClearITPendingBit(EXTI_Line1);  //���LINE�ϵ��жϱ�־λ
		if(PBin(1)==0)   //�����жϼ�⵽���Ƿ����½���
		{
			if(PBin(2)==0)  zuo1js++;//B��ĵ�ƽ����ǵͣ����������ת��1
			else             zuo1js--;//������Ƿ�ת��1
		}
		else                  //������
		{
			if(PBin(2)==1)  zuo1js++; //B���ƽ���Ϊ�ߣ����������ת��1
			else             zuo1js--;//������Ƿ�ת��1
		}
	}
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET) //����A�� PB12
	{
		EXTI_ClearITPendingBit(EXTI_Line2);  //���LINE�ϵ��жϱ�־λ
		if(PBin(2)==0)   //�����жϼ�⵽���Ƿ����½���
		{
			if(PBin(1)==1)  zuo1js++;//B��ĵ�ƽ����ǵͣ����������ת��1
			else             zuo1js--;//������Ƿ�ת��1
		}
		else                  //������
		{
			if(PBin(1)==0)  zuo1js++; //B���ƽ���Ϊ�ߣ����������ת��1
			else             zuo1js--;//������Ƿ�ת��1
		}
	}
}

//������4 F3 F4
void EXTI3_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line3) != RESET) //����A�� PB12
	{
		EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE�ϵ��жϱ�־λ
		if(PFin(3)==0)   //�����жϼ�⵽���Ƿ����½���
		{
			if(PFin(4)==0)  zuo4js++;//B��ĵ�ƽ����ǵͣ����������ת��1
			else             zuo4js--;//������Ƿ�ת��1
		}
		else                  //������
		{
			if(PFin(4)==1)  zuo4js++; //B���ƽ���Ϊ�ߣ����������ת��1
			else             zuo4js--;//������Ƿ�ת��1
		}
	}
}

//������2  F2 F5
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) //����A�� PB12
	{
		EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE�ϵ��жϱ�־λ
		if(PFin(4)==0)   //�����жϼ�⵽���Ƿ����½���
		{
			if(PFin(3)==1)  zuo4js++;//B��ĵ�ƽ����ǵͣ����������ת��1
			else             zuo4js--;//������Ƿ�ת��1
		}
		else                  //������
		{
			if(PFin(3)==0)  zuo4js++; //B���ƽ���Ϊ�ߣ����������ת��1
			else             zuo4js--;//������Ƿ�ת��1
		}
	}
}

//������23 
void Encoder_Exit_Init(void){
	
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource13);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource14);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource15);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//�����ش���
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}

void EXTI15_10_IRQHandler(void){
	//������2 F12 F13
	if(EXTI_GetITStatus(EXTI_Line12) != RESET) //����A�� PB12
	{
		EXTI_ClearITPendingBit(EXTI_Line12);  //���LINE�ϵ��жϱ�־λ
		if(PFin(12)==0)   //�����жϼ�⵽���Ƿ����½���
		{
			if(PFin(13)==0)  zuo2js++;//B��ĵ�ƽ����ǵͣ����������ת��1
			else             zuo2js--;//������Ƿ�ת��1
		}
		else                  //������
		{
			if(PFin(13)==1)  zuo2js++; //B���ƽ���Ϊ�ߣ����������ת��1
			else             zuo2js--;//������Ƿ�ת��1
		}
	}
	
if(EXTI_GetITStatus(EXTI_Line13) != RESET) //����A�� PB12
	{
		EXTI_ClearITPendingBit(EXTI_Line13);  //���LINE�ϵ��жϱ�־λ
		if(PFin(13)==0)   //�����жϼ�⵽���Ƿ����½���
		{
			if(PFin(12)==1)  zuo2js++;//B��ĵ�ƽ����ǵͣ����������ת��1
			else             zuo2js--;//������Ƿ�ת��1
		}
		else                  //������
		{
			if(PFin(12)==0)  zuo2js++; //B���ƽ���Ϊ�ߣ����������ת��1
			else             zuo2js--;//������Ƿ�ת��1
		}
	}
	
		//������3 F14 F15
		if(EXTI_GetITStatus(EXTI_Line14) != RESET) //����A�� PB12
	{
		EXTI_ClearITPendingBit(EXTI_Line14);  //���LINE�ϵ��жϱ�־λ
		if(PFin(14)==0)   //�����жϼ�⵽���Ƿ����½���
		{
			if(PFin(15)==0)  zuo3js++;//B��ĵ�ƽ����ǵͣ����������ת��1
			else             zuo3js--;//������Ƿ�ת��1
		}
		else                  //������
		{
			if(PFin(15)==1)  zuo3js++; //B���ƽ���Ϊ�ߣ����������ת��1
			else             zuo3js--;//������Ƿ�ת��1
		}
	}
	
if(EXTI_GetITStatus(EXTI_Line15) != RESET) //����A�� PB12
	{
		EXTI_ClearITPendingBit(EXTI_Line15);  //���LINE�ϵ��жϱ�־λ
		if(PFin(15)==0)   //�����жϼ�⵽���Ƿ����½���
		{
			if(PFin(14)==1)  zuo3js++;//B��ĵ�ƽ����ǵͣ����������ת��1
			else             zuo3js--;//������Ƿ�ת��1
		}
		else                  //������
		{
			if(PFin(14)==0)  zuo3js++; //B���ƽ���Ϊ�ߣ����������ת��1
			else             zuo3js--;//������Ƿ�ת��1
		}
	}
	
}







#define xianshu  500.0f //����������
#define jiansubi 30.0f //������ٱ�
#define beipin 4.0f //����4��Ƶ��ʽ
#define T  1  //72000000 / arr / psc //��ʱ��6������ //�ݶ�Ϊ1s 1�� �������ֱ�Ӹ�ֵ

float read_sudu(int TIMX)
{
	 float maichong;
    switch(TIMX)
	{
		case 1:{  maichong =  zuo1js / xianshu / jiansubi / beipin / T;  zuo1js = 0; break;}
		case 2:{  maichong =  zuo2js / xianshu / jiansubi / beipin / T;  zuo2js = 0; break;}
		case 3:{  maichong =  zuo3js / xianshu / jiansubi / beipin / T;  zuo3js = 0; break;}
		case 4:{  maichong =  zuo4js / xianshu / jiansubi / beipin / T;  zuo4js = 0; break;}
	}
	return maichong;
}



//���ڶ�ʱ��6 7 ̫��ͨ ���� ���ÿ����������ٶ�
void TIM_7_init(u16 arr,u16 psc){
	
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef NVIC_InitStrcture;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);

	TIM_InitStructure.TIM_Period = arr;//�Զ���װ�ؼĴ������ڵ�ֵ
	TIM_InitStructure.TIM_Prescaler = psc;//Ԥ��Ƶϵ��
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ϵͳʱ�ӷָ���ָ�
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ��� TIM6 TIM7ֻ�����ϼ���
	TIM_InitStructure.TIM_RepetitionCounter = DISABLE;//������ʱ����֧���ظ�����
	TIM_TimeBaseInit(TIM7,&TIM_InitStructure);

	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//����ж�״̬
	TIM_ITConfig(TIM7,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//ʹ���жϸ��º��ж�Դ
	TIM_Cmd(TIM7,ENABLE);//����ȫ��ʹ��

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//ʹ��0�ŷ���
	NVIC_InitStrcture.NVIC_IRQChannel = TIM7_IRQn;//�ж���ڣ�ʹ��TIM6
	NVIC_InitStrcture.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�
	NVIC_InitStrcture.NVIC_IRQChannelSubPriority = 0;//��ռ���ȼ�
	NVIC_InitStrcture.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	NVIC_Init(&NVIC_InitStrcture);//��ʼ��
}


void TIM7_IRQHandler(void){
	if(TIM_GetITStatus(TIM7,TIM_IT_Update) != RESET){
		//ռ�ձ� = CRR / 100
		printf("%d",byte);
		//printf("!!%.4f %.4f %.4f %.4f\r\n", read_sudu(1),read_sudu(2),read_sudu(3),read_sudu(4));
		}
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//����ж�״̬
}


//A0 ��ӦIN1  A1 ��ӦIN2  B6 ��ӦENA
void Encoder_1_stop(void){ PAout(0) = 0; PAout(1) = 0;}
void Encoder_1_go(void){PAout(1) = 1;PAout(0) = 0;}
void Encoder_1_back(void){PAout(0) = 0;PAout(1) = 1;}

//A12 ��ӦIN1  A13 ��ӦIN2  B7 ��ӦENB
void Encoder_2_stop(void){ PAout(12) = 0; PAout(13) = 0;}
void Encoder_2_go(void){PAout(12) = 1;PAout(13) = 0;}
void Encoder_2_back(void){PAout(12) = 0;PAout(13) = 1;}

//A14 ��ӦIN1  A15 ��ӦIN2  B8 ��ӦENA
void Encoder_3_stop(void){PDout(14) = 1;PDout(9) = 15;}
void Encoder_3_go(void){PDout(14) = 1;PDout(9) = 15;}
void Encoder_3_back(void){PDout(14) = 0;PDout(9) = 15;}

//F5 ��ӦIN1  F6 ��ӦIN2  B9 ��ӦENB
void Encoder_4_stop(void){PFout(5) = 1;PFout(6) = 1;}
void Encoder_4_go(void){PFout(5) = 1;PFout(6) = 0;}
void Encoder_4_back(void){PFout(5) = 0;PFout(6) = 1;}


void qianjin(){
	Encoder_1_go();
	Encoder_2_go();
	Encoder_3_go();
	Encoder_4_go();
}

void houtui(){
	Encoder_1_back();
	Encoder_2_back();
	Encoder_3_back();
	Encoder_4_back();
}

void jiasu(void){
	
}

void jiansu(void){
	
}

//���ǲ������������
void zuozhuan(void){
	
}

void youzhuan(void){
	
}

void zhuanquan(void){
	
}

void main_Init(void){
	Encoder_exit_Init();  //������1�ⲿ�жϳ�ʼ��
	Encoder_Exit_Init();  //������23�ⲿ�жϳ�ʼ��
	Encoder_EXit_Init();  //������4�ⲿ�жϳ�ʼ��
	Encoder_GPIO_L298N();  // A0 1  12 - 15
	Tim4_PWM_Init(99,719); //1000
}


