#include "Motor.h"
#include <stdio.h>
#include "timer.h"



// B4 B7 B1
// A11 B0
//�������ʹ��
//CLK1 ��A11  CLK2 ��B7
void CLK_Init(void){
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
		//TIM1_4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                   //TIM1_CH4 PA11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		//TIM4_2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH1��T
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


//ʹ�����ų�ʼ��
//EN1 ��B0  EN2 ��B1
void EN_Init(void){
	
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;				 //LED0-->PB.5 �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
}

//b2���Ų��� ��
//DIR1 ��B3  DIR4 ��B4
void DIR_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;				 //LED0-->PB.5 �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
}








//�����ֵİ뾶Ϊ
#define  R  0.3f
#define PI 3.1415f


//��������ٶ�����
//������ָ����������� ������
int bu_sudu_change(int juli){
	float quanshu = juli / 2 / PI / R;
	float maichongshu = 3200 * quanshu;
	return  maichongshu;
}


// 1 �� ���� ��ת
void Motor1_fx(int fx){if(fx == 1) DIR1 = 1; else DIR1 = 0;}
// ���1 �Ƿ�ʹ�� // 0 ʹ��
void Motor1_go_stop(int stop){if(stop == 1) EN1 = 0; else EN1 = 1;}
// 1 ��  ���� ��ת
void Motor2_fx(int fx){if(fx == 1) DIR2 = 1; else DIR2 = 0;}
// ���2 �Ƿ�ʹ��
void Motor2_go_stop(int stop){if(stop == 1) EN2 = 0; else EN2 = 1;}



//�����������
void qianjin(void){
	Motor1_go_stop(0);
	Motor2_go_stop(0);
	Motor1_fx(1);
	Motor2_fx(1);
	//������2��һȦ
	Pulse_output_1(625,3200); //Ƶ�� = 72M / 72 / Ŀ��Ƶ��
	Pulse_output_2(625,3200); //Ƶ�� = 72M / 72 / Ŀ��Ƶ��
}



void main_Init(void){
	CLK_Init();//�������ʹ��
	EN_Init();//ʹ�����ų�ʼ��
	DIR_Init();//�����ʼ��
}


