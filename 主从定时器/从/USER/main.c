#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "spi.h"

//STM32F103ZET6   ��
//DS0�����������ʾ��ʼ����ɣ�DS1�����������ʾ�������
 int main(void)
 {
	 u16 arr[] = {1,2,3};
	 u8 i = 0;
		SystemInit(); //ϵͳʱ������
		delay_init();	    	 //��ʱ������ʼ��	  
		uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	  LED_Init();
	 	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	  delay_ms(1000);
	  //�ӻ��ǽ����������ݵ�SCK�ģ����Բ���Ҫ��Ƶ�Լ���SPI�ˣ������ע�͵��ˣ��������������仰��ʵ����û���
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_256);
		SPI1_Init(); //SPI1��ʼ��
	  GPIO_SetBits(GPIOB,GPIO_Pin_5);
		while(1)
		{
			 u16 temp;

       GPIO_ResetBits(GPIOE,GPIO_Pin_5);
			 delay_ms(1000);
			
				temp = SPI1->DR; //����ͨ��SPIx������յ�����
				printf("%d\r\n",temp);
			
			 GPIO_SetBits(GPIOE,GPIO_Pin_5);
		}
	}

