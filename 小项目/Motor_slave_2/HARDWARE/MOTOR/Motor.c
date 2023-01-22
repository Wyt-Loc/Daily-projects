#include "Motor.h"
#include <stdio.h>
#include "timer.h"



// B4 B7 B1
// A11 B0
//脉冲输出使能
//CLK1 接A11  CLK2 接B7
void CLK_Init(void){
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
		//TIM1_4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                   //TIM1_CH4 PA11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		//TIM4_2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH1和T
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


//使能引脚初始化
//EN1 接B0  EN2 接B1
void EN_Init(void){
	
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;				 //LED0-->PB.5 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
}

//b2引脚不用 做
//DIR1 接B3  DIR4 接B4
void DIR_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;				 //LED0-->PB.5 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
}








//假设轮的半径为
#define  R  0.3f
#define PI 3.1415f


//步进电机速度设置
//返回走指定距离所需的 脉冲数
int bu_sudu_change(int juli){
	float quanshu = juli / 2 / PI / R;
	float maichongshu = 3200 * quanshu;
	return  maichongshu;
}


// 1 正 其他 反转
void Motor1_fx(int fx){if(fx == 1) DIR1 = 1; else DIR1 = 0;}
// 电机1 是否使能 // 0 使能
void Motor1_go_stop(int stop){if(stop == 1) EN1 = 0; else EN1 = 1;}
// 1 正  其他 反转
void Motor2_fx(int fx){if(fx == 1) DIR2 = 1; else DIR2 = 0;}
// 电机2 是否使能
void Motor2_go_stop(int stop){if(stop == 1) EN2 = 0; else EN2 = 1;}



//步进电机设置
void qianjin(void){
	Motor1_go_stop(0);
	Motor2_go_stop(0);
	Motor1_fx(1);
	Motor2_fx(1);
	//现在是2秒一圈
	Pulse_output_1(625,3200); //频率 = 72M / 72 / 目标频率
	Pulse_output_2(625,3200); //频率 = 72M / 72 / 目标频率
}



void main_Init(void){
	CLK_Init();//脉冲输出使能
	EN_Init();//使能引脚初始化
	DIR_Init();//方向初始化
}


