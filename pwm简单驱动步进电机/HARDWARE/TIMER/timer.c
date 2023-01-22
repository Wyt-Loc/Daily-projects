#include "timer.h"
#include "led.h"
#include "usart.h"

void DIR_Init(void)		//DIR——PB5初始化
{	
 GPIO_InitTypeDef  GPIO_InitStructure; //结构体命名
	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能GPIOB通道
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //频率50hz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 
 GPIO_SetBits(GPIOB,GPIO_Pin_5);		//默认高电平 正方向
 
}


void TIM4_PWM_Init(u16 arr,u16 psc)
{  
    //结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	//使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	  
	
   //设置该引脚为复用输出功能,输出TIM4 CH1的PWM脉冲波形	GPIOB.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	
    //设置定时器参数
  TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //边沿对齐模式。计数器依据方向位(DIR)向上计数
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
    
		
    //中断设置
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  
	
	
	//初始化TIM4 Channel1 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;  //TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器,即TIM3_CCR2的预装载值在更新事件到来时才能被传送至当前寄存器中。
	
    TIM_Cmd(TIM4, ENABLE);  //使能TIM4
}

//定时器4中断服务程序
void TIM4_IRQHandler(void)   
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
	}
}

