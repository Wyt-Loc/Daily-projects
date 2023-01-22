#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "Motor.h"
#include "PID.h"
#include <stdio.h>
#include "spi.h"
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
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();            //��ʱ������ʼ��
    uart_init(115200);   //���ڳ�ʼ��Ϊ115200
		main_Init();
		LED_Init();
	  //Pulse_output_1(625,3200); //Ƶ�� = 72M / 72 / Ŀ��Ƶ��     ����
	  SPI1_Init();
    while(1)
    {
			  Control_Motor(Recv_Data_SPI());
			//Control_Motor(Recv_Data_SPI());
			//printf("!! %f %f\r\n", read_sudu(1),read_sudu(2));
    }
}




