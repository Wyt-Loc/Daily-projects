/************************************************************************************
						
*************************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "HX711.h"
#include "usart.h"



int main(void)
{		
	Init_HX711pin();
	delay_init();
	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	
	Get_Maopi();				//��ëƤ����
	delay_ms(1000);
	delay_ms(1000);
	Get_Maopi();				//���»�ȡëƤ����
	
	while(1)
	{
		Get_Weight();

		printf("������ = %d g\r\n",Weight_Shiwu); //��ӡ 
		delay_ms(1000);


	}
}






