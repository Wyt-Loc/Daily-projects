#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
 
/************************************************
 ALIENTEK精英STM32开发板实验9
 PWM输出实验  
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

 int main(void)
 {		
 	u16 led0pwmval=0;
	u8 dir=1;	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	//TIM8_Int_Init(49999,7199);   //72M / 7199 = 10Khz   50000 / 1000Khz = 5s
 	TIM8_PWM_Init(899,1);	 //不分频。PWM频率=72000000/900=80Khz   80khz
   	while(1)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
		
 		delay_ms(10);	 
		if(dir)led0pwmval++;
		else led0pwmval--;

 		if(led0pwmval>899)dir=0;
		if(led0pwmval==0)dir=1;
	
		TIM_SetCompare1(TIM8,led0pwmval);		 
		TIM_SetCompare2(TIM8,led0pwmval);
		TIM_SetCompare3(TIM8,led0pwmval);
		TIM_SetCompare4(TIM8,led0pwmval);		
	}
 }

