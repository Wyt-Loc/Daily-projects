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
 ALIENTEK��ӢSTM32������ʵ��5
 �ⲿ�ж� ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
u8 Key000=0;

//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־ 
			KEY_config(&Key000);
			Key_Input(&Key000);
		}
}


char a[9]={0,0,0,0,0,0,0,0,0};
char *abd_xiugai(u16 abc[9]){
	char i = 0;
  while(abc[i]!=0){
		a[i] = abc[i]  +  48; //ת��Ϊ
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
	delay_init();	    	 //��ʱ������ʼ��	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(4800);	 //���ڳ�ʼ��4800
	 KEY_Init();
	TIM3_Int_Init(89999,2);  
	OLED_Init();
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
  OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ

	 
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

	
