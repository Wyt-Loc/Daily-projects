#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"

 
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
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	TIM4_Int_Init(49999,7199);//���ֵ��Ӧu16  0 - 65535	 // 72M / 7199   10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms   5000 / 100000 = 
   	while(1)
	{
		TIM4_IRQHandler();
	//	LED0=!LED0;
	//	delay_ms(200);		   
	} 
}	 
 
