#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "remote.h"
#include "oled.h" 
//ALIENTEK 探索者STM32F407开发板 实验29
//红外遥控器实验 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

/* 接线
oled scl -- PD6
		 sda -- PD5
		红外接收管
		接收端 PA8
*/
 
int main(void)
{ 
	
	u8 key;
	u8 t=0;	
	u8 *str=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	
	LED_Init();					//初始化LED
 	//LCD_Init();
	OLED_Init();				//初始化OLED
 	Remote_Init();				//红外接收初始化		 	
	
 	//POINT_COLOR=RED;//设置字体为红色 
	OLED_ShowString(8,16,"温度为",16,1); 
	
	while(1)
	{
		
		OLED_Refresh();
		delay_ms(200);
		OLED_Clear();
		OLED_ShowString(10,10,"jian zhi:",8,1);
		
		key=Remote_Scan();	
		if(key)
		{	 
			switch(key)
			{
				case 162: {
					str="1"; 
					GPIO_ResetBits(GPIOE,GPIO_Pin_9);
					GPIO_SetBits(GPIOE,GPIO_Pin_10); 
				break;
				}						
				case 98:{
					str="2";
						GPIO_SetBits(GPIOE,GPIO_Pin_9);
						GPIO_ResetBits(GPIOE,GPIO_Pin_10);
					break;
				}
				
				case 226:str="3";break;	    		
				case 34:str="4";break;		 
				case 2:str="5";break;		  
				case 194:str="6";break;	   
				case 224:str="7";break;		   
				case 168:str="8";break;		  
				case 144:str="9";break;		   
				case 152:str="0";break;		    
				case 104:str="*";break;		  
				case 176:str="#";break;	   
				case 24:str="^";break;	    
				case 16:str="<";break;		   
				case 74:str="v";break;		   
				case 90:str=">";break;		 
				case 56:str="OK";break;		 
			}
			OLED_ShowString(80,10,str,16,1);
		}
	}
}
