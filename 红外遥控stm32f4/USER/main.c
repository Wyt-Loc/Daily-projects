#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "remote.h"
#include "oled.h" 
//ALIENTEK ̽����STM32F407������ ʵ��29
//����ң����ʵ�� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

/* ����
oled scl -- PD6
		 sda -- PD5
		������չ�
		���ն� PA8
*/
 
int main(void)
{ 
	
	u8 key;
	u8 t=0;	
	u8 *str=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	
	LED_Init();					//��ʼ��LED
 	//LCD_Init();
	OLED_Init();				//��ʼ��OLED
 	Remote_Init();				//������ճ�ʼ��		 	
	
 	//POINT_COLOR=RED;//��������Ϊ��ɫ 
	OLED_ShowString(8,16,"�¶�Ϊ",16,1); 
	
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
