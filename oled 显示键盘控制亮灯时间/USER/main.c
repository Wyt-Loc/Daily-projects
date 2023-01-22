#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "exti.h"
#include "beep.h"
#include "timer.h"
#include "stm32f10x_tim.h"
#include "oled.h"
/************************************************
 ALIENTEK精英STM32开发板实验5
 外部中断 实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
u8 Key000=0;

//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志 
			KEY_config(&Key000);
			Key_Input(&Key000);
		}
}


char a[9]={0,0,0,0,0,0,0,0,0};
char *abd_xiugai(u16 abc[9]){
	char i = 0;
  while(abc[i]!=0){
		a[i] = abc[i]  +  48; //转换为
		i++;
		if(i==7)
			break;
		}
	return a;
}
	
char b[5];
char *sun1_read_xiu_acill(int a){
	//1234000s
	
	 a = a/1000;
	
	 b[3] = (char)(a%10+48);
	 b[2] = (char)((a/10)%10)+48;
	 b[1] = (char)((a/100)%10)+48;
	 b[0] = (char)(a/1000)+48;
	 b[4] = 0;
	 return b;
} 


 int main(void)
 {		
	u8 is_anxia = 0;
	delay_init();	    	 //延时函数初始化	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(4800);	 //串口初始化4800
	 KEY_Init();
	TIM3_Int_Init(89999,2);  
	OLED_Init();
	OLED_ColorTurn(0);//0正常显示，1 反色显示
  OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示

	 
	while(1)
	{	 		

		OLED_Clear();
		
		if(*(abc_read()+0) == 0 && sun1_read()==0)
			OLED_ShowString(10,0,"P 16",16,1);
		else if(sun1_read()==0)
			OLED_ShowString(10,0,abd_xiugai(abc_read()),16,1);
		else if(sun1_read()!=0)
			OLED_ShowString(10,0,sun1_read_xiu_acill(sun1_read()),16,1);
		
		OLED_Refresh(); 
	}
	}

	
