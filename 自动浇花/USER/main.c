//  功能描述   : OLED 接口演示例程(STM32F4系列)
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PD6（SCL）
//              D1   接PD5（SDA）
//              RES  PD4（SPI模块改成IIC模块需要接此引脚，IIC模块用户请忽略）

//接线方式

//pwm 接线 PA7 --- 白线
//adc 接线 PA5 --- 被检测引脚


#include "sys.h"
#include "delay.h"
#include "led.h"
#include "oled.h"
#include "bmp.h"
#include "usart.h"
#include "adc.h"
#include "pwm.h"
#include "timer.h"

// > 800 此数值根据实际情况改变  干了 需要浇水

void TIM3_Int_Init(u16 arr,u16 psc);

	u16 a[1] = {0};

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		//LED1=!LED1;//DS1翻转
		printf("\r\n开始通道5\t");
		a[0] = Get_Adc(ADC_Channel_5);
		printf("%d",a[0]);
		if(a[0] > 800) 
		TIM_SetCompare1(TIM14, 5); //对应0度	//对应0°
		else
		TIM_SetCompare1(TIM14, 15); //对应90度
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	uart_init(38400);
	delay_init(168);
	//LED_Init();					//初始化LED
 	OLED_Init();				//初始化OLED
	Adc_Init();
	//LED0=0;
	TIM14_PWM_Init(200-1,8400-1); //PWM初始化
	
	TIM3_Int_Init(5000-1,8400-1); //500ms 执行检测一次
		//STM初始化
	
	//周期=（arr+1）*（psc+1）/CLK
	
	// (arr) * (psc) / 8000000 = 20ms
	
	//定时器时钟为8M，分频系数为800 所以计数频率为 84M / 84 =  10 重装栽值为 200 
	//所以输出频率为 10 / 500 =  20ms
	
	while(1)
	{
		
		//根据此公式计算倒水多少
		//t = x / 20 * 200
		TIM_SetCompare1(TIM14, 5); //对应0度
		delay_ms(2000);
		TIM_SetCompare1(TIM14, 10); //对应45度
		delay_ms(2000);
		TIM_SetCompare1(TIM14, 15); //对应90度
		delay_ms(2000);
		TIM_SetCompare1(TIM14, 20); //对应135度
		delay_ms(2000);
		TIM_SetCompare1(TIM14, 25); //对应180度
		delay_ms(2000);
	
		
		/*
		OLED_Refresh();
		delay_ms(500);
		OLED_Clear();
		OLED_ShowString(8,16,"温度为",16,1); 
		OLED_ShowString(8,23,a,16,1);
		*/
	}
}




