#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "csb.h"
#include "stdio.h"
#include "lcd.h"
 
/************************************************
 ALIENTEK精英STM32开发板实验8
 定时器中断实验  
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
	
 int main(void)
 {		
	 int len =0;
	 int t = 0;
	 float dis = 0;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
	My_CSB_Init();
	 LCD_Init();
	 POINT_COLOR=RED;

 //	LED_Init();			     //LED端口初始化
 //	TIM3_Int_Init(4999,7199);//10Khz的计数频率，计数到5000为500ms  
   	while(1)
	{
		LCD_Clear(WHITE);
		dis = Distance();
		LCD_ShowxNum(30,40,dis,4,24,1);
	//	LCD_ShowChar(30,40,dis,24,0);
//	USART_SendData(USART1,dis);
	//printf("%lf\r\n",dis);
	delay_ms(100);		   
	}
}

 

