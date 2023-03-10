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
 	u16 led0pwmval=300;
	u8 dir=1;	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
 	TIM3_PWM_Init(200-1,8400-1);	 // 周期为20ms
	 	 			
		
   	while(1)        									//  占空比led0pwmval/arr
	{																	
 		delay_ms(10);	          				 
		if(dir)
			led0pwmval++;
		else 
			led0pwmval--;			
 		if(led0pwmval>300)
			dir=0;
		if(led0pwmval==0)
			dir=1;										 
		TIM_SetCompare2(TIM3,led0pwmval);	  
	}	 
 }
 
 

