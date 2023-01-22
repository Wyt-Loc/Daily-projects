#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "spi.h"

//STM32F103ZET6   从
//DS0的先亮后灭表示初始化完成，DS1的先亮后灭表示发送完成
 int main(void)
 {
	 u16 arr[] = {1,2,3};
	 u8 i = 0;
		SystemInit(); //系统时钟设置
		delay_init();	    	 //延时函数初始化	  
		uart_init(9600);	 	//串口初始化为9600
	  LED_Init();
	 	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	  delay_ms(1000);
	  //从机是接受主机传递的SCK的，所以不需要分频自己的SPI了，这里就注释掉了，就算你加上了这句话，实现上没差别
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_256);
		SPI1_Init(); //SPI1初始化
	  GPIO_SetBits(GPIOB,GPIO_Pin_5);
		while(1)
		{
			 u16 temp;

       GPIO_ResetBits(GPIOE,GPIO_Pin_5);
			 delay_ms(1000);
			
				temp = SPI1->DR; //返回通过SPIx最近接收的数据
				printf("%d\r\n",temp);
			
			 GPIO_SetBits(GPIOE,GPIO_Pin_5);
		}
	}

