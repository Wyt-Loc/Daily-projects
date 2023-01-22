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
 ALIENTEK��ӢSTM32������ʵ��8
 ��ʱ���ж�ʵ��  
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
	
 int main(void)
 {		
	 int len =0;
	 int t = 0;
	 float dis = 0;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	My_CSB_Init();
	 LCD_Init();
	 POINT_COLOR=RED;

 //	LED_Init();			     //LED�˿ڳ�ʼ��
 //	TIM3_Int_Init(4999,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  
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

 

