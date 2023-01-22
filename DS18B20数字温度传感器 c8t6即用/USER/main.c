#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "ds18b20.h" 
#include "stdio.h"


/************************************************
 ALIENTEK精英STM32开发板实验28
 DS18B20数字温度传感器 实验   
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/



 int main(void)
 {	 
	 int Temp = 0;	
	 char usart1_buf[10];
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(4800);	 	//串口初始化为 115200
	DS18B20_UserConfig();
	 

	 
	 while(1)
	 {
		 ds18b20_read_temp(&Temp);
		 sprintf(usart1_buf,"%.1f\r\n",Temp/10.0);
		 delay_ms(100);
		 printf("%s",usart1_buf);
	 }
 }
 
 
 
 
 /*	while(DS18B20_Init())												//DS18B20初始化	
	{
		LCD_ShowString(30,130,200,16,16,"DS18B20 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}								   
	LCD_ShowString(30,130,200,16,16,"DS18B20 OK");
	POINT_COLOR=BLUE;//设置字体为蓝色 
 	LCD_ShowString(30,150,200,16,16,"Temp:   . C");
	while(1)
	{	    
 		if(t%10==0)			//每100ms读取一次
		{									  
			temperature=DS18B20_Get_Temp();	
			if(temperature<0)
			{
				LCD_ShowChar(30+40,150,'-',16,0);			//显示负号  
				temperature=-temperature;					//转为正数
			}else 
			LCD_ShowChar(30+40,150,' ',16,0);			//去掉负号
			LCD_ShowNum(30+40+8,150,temperature/10,2,16);	//显示正数部分	    
   		LCD_ShowNum(30+40+32,150,temperature%10,1,16);	//显示小数部分 		 
		}				   
	 	delay_ms(10);
		t++;
		if(t==20)
		{
			t=0;
			LED0=!LED0;
		}
	}
}
*/
