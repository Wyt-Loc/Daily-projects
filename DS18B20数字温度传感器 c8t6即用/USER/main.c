#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "ds18b20.h" 
#include "stdio.h"


/************************************************
 ALIENTEK��ӢSTM32������ʵ��28
 DS18B20�����¶ȴ����� ʵ��   
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/



 int main(void)
 {	 
	 int Temp = 0;	
	 char usart1_buf[10];
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(4800);	 	//���ڳ�ʼ��Ϊ 115200
	DS18B20_UserConfig();
	 

	 
	 while(1)
	 {
		 ds18b20_read_temp(&Temp);
		 sprintf(usart1_buf,"%.1f\r\n",Temp/10.0);
		 delay_ms(100);
		 printf("%s",usart1_buf);
	 }
 }
 
 
 
 
 /*	while(DS18B20_Init())												//DS18B20��ʼ��	
	{
		LCD_ShowString(30,130,200,16,16,"DS18B20 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}								   
	LCD_ShowString(30,130,200,16,16,"DS18B20 OK");
	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
 	LCD_ShowString(30,150,200,16,16,"Temp:   . C");
	while(1)
	{	    
 		if(t%10==0)			//ÿ100ms��ȡһ��
		{									  
			temperature=DS18B20_Get_Temp();	
			if(temperature<0)
			{
				LCD_ShowChar(30+40,150,'-',16,0);			//��ʾ����  
				temperature=-temperature;					//תΪ����
			}else 
			LCD_ShowChar(30+40,150,' ',16,0);			//ȥ������
			LCD_ShowNum(30+40+8,150,temperature/10,2,16);	//��ʾ��������	    
   		LCD_ShowNum(30+40+32,150,temperature%10,1,16);	//��ʾС������ 		 
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
