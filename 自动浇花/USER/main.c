//  ��������   : OLED �ӿ���ʾ����(STM32F4ϵ��)
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PD6��SCL��
//              D1   ��PD5��SDA��
//              RES  PD4��SPIģ��ĳ�IICģ����Ҫ�Ӵ����ţ�IICģ���û�����ԣ�

//���߷�ʽ

//pwm ���� PA7 --- ����
//adc ���� PA5 --- ���������


#include "sys.h"
#include "delay.h"
#include "led.h"
#include "oled.h"
#include "bmp.h"
#include "usart.h"
#include "adc.h"
#include "pwm.h"
#include "timer.h"

// > 800 ����ֵ����ʵ������ı�  ���� ��Ҫ��ˮ

void TIM3_Int_Init(u16 arr,u16 psc);

	u16 a[1] = {0};

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		//LED1=!LED1;//DS1��ת
		printf("\r\n��ʼͨ��5\t");
		a[0] = Get_Adc(ADC_Channel_5);
		printf("%d",a[0]);
		if(a[0] > 800) 
		TIM_SetCompare1(TIM14, 5); //��Ӧ0��	//��Ӧ0��
		else
		TIM_SetCompare1(TIM14, 15); //��Ӧ90��
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	uart_init(38400);
	delay_init(168);
	//LED_Init();					//��ʼ��LED
 	OLED_Init();				//��ʼ��OLED
	Adc_Init();
	//LED0=0;
	TIM14_PWM_Init(200-1,8400-1); //PWM��ʼ��
	
	TIM3_Int_Init(5000-1,8400-1); //500ms ִ�м��һ��
		//STM��ʼ��
	
	//����=��arr+1��*��psc+1��/CLK
	
	// (arr) * (psc) / 8000000 = 20ms
	
	//��ʱ��ʱ��Ϊ8M����Ƶϵ��Ϊ800 ���Լ���Ƶ��Ϊ 84M / 84 =  10 ��װ��ֵΪ 200 
	//�������Ƶ��Ϊ 10 / 500 =  20ms
	
	while(1)
	{
		
		//���ݴ˹�ʽ���㵹ˮ����
		//t = x / 20 * 200
		TIM_SetCompare1(TIM14, 5); //��Ӧ0��
		delay_ms(2000);
		TIM_SetCompare1(TIM14, 10); //��Ӧ45��
		delay_ms(2000);
		TIM_SetCompare1(TIM14, 15); //��Ӧ90��
		delay_ms(2000);
		TIM_SetCompare1(TIM14, 20); //��Ӧ135��
		delay_ms(2000);
		TIM_SetCompare1(TIM14, 25); //��Ӧ180��
		delay_ms(2000);
	
		
		/*
		OLED_Refresh();
		delay_ms(500);
		OLED_Clear();
		OLED_ShowString(8,16,"�¶�Ϊ",16,1); 
		OLED_ShowString(8,23,a,16,1);
		*/
	}
}




