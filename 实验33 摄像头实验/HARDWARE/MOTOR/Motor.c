#include "Motor.h"
#include <stdio.h>
#include "timer.h"
#include "spi.h"




// 1  A12 A13   2  A14 A15    3  F12 F13   4  F14  F15   4组
void Encoder_GPIO_L298N(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;			 //LED0-->PB.5 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;			 //LED0-->PB.5 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOF, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
}



/*
//TIM4 CH1 PWM 输出设置
//PWM 输出初始化
//arr：自动重装值
//psc：时钟预分频数
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
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能 TIMx 外设
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能 PB 时钟
	TIM_DeInit(TIM4);
 
  //设置该引脚为复用输出功能,输出 TIM4 CH1 的 PWM 脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM4_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用功能输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化 GPIO
 
	TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载周期值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置预分频值 不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //初始化 TIMx
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH1 PWM2 模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	
	TIM_OCInitStructure.TIM_Pulse = 200; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //OC1 高电平有效
	TIM_OC1Init(TIM4, &TIM_OCInitStructure); //根据指定的参数初始化外设 TIMx
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //CH1 预装载使能
	
	TIM_OCInitStructure.TIM_Pulse = 400; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //OC2 高电平有效
	TIM_OC2Init(TIM4, &TIM_OCInitStructure); //根据指定的参数初始化外设 TIMx
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //CH2 预装载使能
	
	TIM_OCInitStructure.TIM_Pulse = 600; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //OC1 高电平有效
	TIM_OC3Init(TIM4, &TIM_OCInitStructure); //根据指定的参数初始化外设 TIMx
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //CH3 预装载使能
	
	TIM_OCInitStructure.TIM_Pulse = 800; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //OC1 高电平有效
	TIM_OC4Init(TIM4, &TIM_OCInitStructure); //根据指定的参数初始化外设 TIMx
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //CH4 预装载使能
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //使能 TIMx 在 ARR 上的预装载寄存器
	TIM_Cmd(TIM4, ENABLE); //使能 TIMx
}


 int zuo1js = 0;
 int zuo2js = 0;
 int zuo3js = 0;
 int zuo4js = 0;


//编码器1 初始化 B1 B2
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
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//跳变沿触发
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


//编码器4 初始化 A1 A2
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
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//跳变沿触发
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


//编码器1 B1 B2
void EXTI1_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line1) != RESET) //左轮A相 PB12
	{
		EXTI_ClearITPendingBit(EXTI_Line1);  //清除LINE上的中断标志位
		if(PBin(1)==0)   //这里判断检测到的是否是下降沿
		{
			if(PBin(2)==0)  zuo1js++;//B相的电平如果是低，电机就是正转加1
			else             zuo1js--;//否则就是反转减1
		}
		else                  //上升沿
		{
			if(PBin(2)==1)  zuo1js++; //B相电平如果为高，电机就是正转加1
			else             zuo1js--;//否则就是反转减1
		}
	}
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET) //左轮A相 PB12
	{
		EXTI_ClearITPendingBit(EXTI_Line2);  //清除LINE上的中断标志位
		if(PBin(2)==0)   //这里判断检测到的是否是下降沿
		{
			if(PBin(1)==1)  zuo1js++;//B相的电平如果是低，电机就是正转加1
			else             zuo1js--;//否则就是反转减1
		}
		else                  //上升沿
		{
			if(PBin(1)==0)  zuo1js++; //B相电平如果为高，电机就是正转加1
			else             zuo1js--;//否则就是反转减1
		}
	}
}

//编码器4 F3 F4
void EXTI3_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line3) != RESET) //左轮A相 PB12
	{
		EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE上的中断标志位
		if(PFin(3)==0)   //这里判断检测到的是否是下降沿
		{
			if(PFin(4)==0)  zuo4js++;//B相的电平如果是低，电机就是正转加1
			else             zuo4js--;//否则就是反转减1
		}
		else                  //上升沿
		{
			if(PFin(4)==1)  zuo4js++; //B相电平如果为高，电机就是正转加1
			else             zuo4js--;//否则就是反转减1
		}
	}
}

//编码器2  F2 F5
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) //左轮A相 PB12
	{
		EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE上的中断标志位
		if(PFin(4)==0)   //这里判断检测到的是否是下降沿
		{
			if(PFin(3)==1)  zuo4js++;//B相的电平如果是低，电机就是正转加1
			else             zuo4js--;//否则就是反转减1
		}
		else                  //上升沿
		{
			if(PFin(3)==0)  zuo4js++; //B相电平如果为高，电机就是正转加1
			else             zuo4js--;//否则就是反转减1
		}
	}
}

//编码器23 
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
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//跳变沿触发
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}

void EXTI15_10_IRQHandler(void){
	//编码器2 F12 F13
	if(EXTI_GetITStatus(EXTI_Line12) != RESET) //左轮A相 PB12
	{
		EXTI_ClearITPendingBit(EXTI_Line12);  //清除LINE上的中断标志位
		if(PFin(12)==0)   //这里判断检测到的是否是下降沿
		{
			if(PFin(13)==0)  zuo2js++;//B相的电平如果是低，电机就是正转加1
			else             zuo2js--;//否则就是反转减1
		}
		else                  //上升沿
		{
			if(PFin(13)==1)  zuo2js++; //B相电平如果为高，电机就是正转加1
			else             zuo2js--;//否则就是反转减1
		}
	}
	
if(EXTI_GetITStatus(EXTI_Line13) != RESET) //左轮A相 PB12
	{
		EXTI_ClearITPendingBit(EXTI_Line13);  //清除LINE上的中断标志位
		if(PFin(13)==0)   //这里判断检测到的是否是下降沿
		{
			if(PFin(12)==1)  zuo2js++;//B相的电平如果是低，电机就是正转加1
			else             zuo2js--;//否则就是反转减1
		}
		else                  //上升沿
		{
			if(PFin(12)==0)  zuo2js++; //B相电平如果为高，电机就是正转加1
			else             zuo2js--;//否则就是反转减1
		}
	}
	
		//编码器3 F14 F15
		if(EXTI_GetITStatus(EXTI_Line14) != RESET) //左轮A相 PB12
	{
		EXTI_ClearITPendingBit(EXTI_Line14);  //清除LINE上的中断标志位
		if(PFin(14)==0)   //这里判断检测到的是否是下降沿
		{
			if(PFin(15)==0)  zuo3js++;//B相的电平如果是低，电机就是正转加1
			else             zuo3js--;//否则就是反转减1
		}
		else                  //上升沿
		{
			if(PFin(15)==1)  zuo3js++; //B相电平如果为高，电机就是正转加1
			else             zuo3js--;//否则就是反转减1
		}
	}
	
if(EXTI_GetITStatus(EXTI_Line15) != RESET) //左轮A相 PB12
	{
		EXTI_ClearITPendingBit(EXTI_Line15);  //清除LINE上的中断标志位
		if(PFin(15)==0)   //这里判断检测到的是否是下降沿
		{
			if(PFin(14)==1)  zuo3js++;//B相的电平如果是低，电机就是正转加1
			else             zuo3js--;//否则就是反转减1
		}
		else                  //上升沿
		{
			if(PFin(14)==0)  zuo3js++; //B相电平如果为高，电机就是正转加1
			else             zuo3js--;//否则就是反转减1
		}
	}
	
}







#define xianshu  500.0f //编码器线数
#define jiansubi 30.0f //电机减速比
#define beipin 4.0f //采用4倍频方式
#define T  1  //72000000 / arr / psc //定时器6的周期 //暂定为1s 1次 方便计算直接赋值

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



//由于定时器6 7 太普通 但是 正好可以做计算速度
void TIM_7_init(u16 arr,u16 psc){
	
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef NVIC_InitStrcture;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);

	TIM_InitStructure.TIM_Period = arr;//自动重装载寄存器周期的值
	TIM_InitStructure.TIM_Prescaler = psc;//预分频系数
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//系统时钟分割：不分割
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数 TIM6 TIM7只有向上计数
	TIM_InitStructure.TIM_RepetitionCounter = DISABLE;//基本定时器不支持重复计数
	TIM_TimeBaseInit(TIM7,&TIM_InitStructure);

	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//清空中断状态
	TIM_ITConfig(TIM7,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//使能中断更新和中断源
	TIM_Cmd(TIM7,ENABLE);//开启全局使能

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//使用0号分组
	NVIC_InitStrcture.NVIC_IRQChannel = TIM7_IRQn;//中断入口：使用TIM6
	NVIC_InitStrcture.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级
	NVIC_InitStrcture.NVIC_IRQChannelSubPriority = 0;//子占优先级
	NVIC_InitStrcture.NVIC_IRQChannelCmd = ENABLE;//中断使能
	NVIC_Init(&NVIC_InitStrcture);//初始化
}


void TIM7_IRQHandler(void){
	if(TIM_GetITStatus(TIM7,TIM_IT_Update) != RESET){
		//占空比 = CRR / 100
		printf("%d",byte);
		//printf("!!%.4f %.4f %.4f %.4f\r\n", read_sudu(1),read_sudu(2),read_sudu(3),read_sudu(4));
		}
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//清空中断状态
}


//A0 对应IN1  A1 对应IN2  B6 对应ENA
void Encoder_1_stop(void){ PAout(0) = 0; PAout(1) = 0;}
void Encoder_1_go(void){PAout(1) = 1;PAout(0) = 0;}
void Encoder_1_back(void){PAout(0) = 0;PAout(1) = 1;}

//A12 对应IN1  A13 对应IN2  B7 对应ENB
void Encoder_2_stop(void){ PAout(12) = 0; PAout(13) = 0;}
void Encoder_2_go(void){PAout(12) = 1;PAout(13) = 0;}
void Encoder_2_back(void){PAout(12) = 0;PAout(13) = 1;}

//A14 对应IN1  A15 对应IN2  B8 对应ENA
void Encoder_3_stop(void){PDout(14) = 1;PDout(9) = 15;}
void Encoder_3_go(void){PDout(14) = 1;PDout(9) = 15;}
void Encoder_3_back(void){PDout(14) = 0;PDout(9) = 15;}

//F5 对应IN1  F6 对应IN2  B9 对应ENB
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

//都是步进电机的事了
void zuozhuan(void){
	
}

void youzhuan(void){
	
}

void zhuanquan(void){
	
}

void main_Init(void){
	Encoder_exit_Init();  //编码器1外部中断初始化
	Encoder_Exit_Init();  //编码器23外部中断初始化
	Encoder_EXit_Init();  //编码器4外部中断初始化
	Encoder_GPIO_L298N();  // A0 1  12 - 15
	Tim4_PWM_Init(99,719); //1000
}


