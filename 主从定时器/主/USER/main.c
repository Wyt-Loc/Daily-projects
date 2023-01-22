#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "spi.h"

//STM32F103RCT6   ��
//DS0�����������ʾ��ʼ����ɣ�DS1�����������ʾ�������
 int main(void)
 { 
	 u16 arr[] = {1,2,3};
	 int i = 0;
		SystemInit(); //ϵͳʱ������
		delay_init();	    	 //��ʱ������ʼ��	  
		uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	  LED_Init();
	  delay_ms(1000);
	  //�������ṩSCK���ӻ��ģ���������Ҫ����ʱ�ӷ�Ƶ
		SPI1_SetSpeed(SPI_BaudRatePrescaler_256); //SPI�ٶ�256��Ƶ  
	  SPI1_Init(); //SPI1��ʼ��
		while(1)
		{
			delay_ms(1000);
		  SPI1_ReadWriteByte(arr[i]);
			i++;
			if(i == 3) i = 0;
		}
}
