#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "Motor.h"
#include "PID.h"
#include <stdio.h>
/************************************************
 ALIENTEK精英STM32开发板实验9
 PWM输出实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司
 作者：正点原子 @ALIENTEK
************************************************/
 int main(void)
 {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();            //延时函数初始化
    uart_init(115200);   //串口初始化为115200
		main_Init();
	  //Pulse_output_2(625,3200*2); //频率 = 72M / 72 / 目标频率
	  //Pulse_output_1(625,3200); //频率 = 72M / 72 / 目标频率     距离
    while(1)
    {
			Pulse_output_1(625,3200); //频率 = 72M / 72 / 目标频率
			Pulse_output_2(625,3200); //频率 = 72M / 72 / 目标频率
			//printf("!! %f %f\r\n", read_sudu(1),read_sudu(2));
    }
}




