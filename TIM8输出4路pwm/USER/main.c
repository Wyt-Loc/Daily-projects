#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
 
/************************************************
 ALIENTEK��ӢSTM32������ʵ��9
 PWM���ʵ��  
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

 int main(void)
 {		
 	u16 led0pwmval=0;
	u8 dir=1;	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	//TIM8_Int_Init(49999,7199);   //72M / 7199 = 10Khz   50000 / 1000Khz = 5s
 	TIM8_PWM_Init(899,1);	 //����Ƶ��PWMƵ��=72000000/900=80Khz   80khz
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

