#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "spi.h"

//STM32F103RCT6   主
//DS0的先亮后灭表示初始化完成，DS1的先亮后灭表示发送完成
 int main(void)
 { 
	 u16 arr[] = {1,2,3};
	 int i = 0;
		SystemInit(); //系统时钟设置
		delay_init();	    	 //延时函数初始化	  
		uart_init(9600);	 	//串口初始化为9600
	  LED_Init();
	  delay_ms(1000);
	  //主机是提供SCK给从机的，所以主机要设置时钟分频
		SPI1_SetSpeed(SPI_BaudRatePrescaler_256); //SPI速度256分频  
	  SPI1_Init(); //SPI1初始化
		while(1)
		{
			delay_ms(1000);
		  SPI1_ReadWriteByte(arr[i]);
			i++;
			if(i == 3) i = 0;
		}
}
