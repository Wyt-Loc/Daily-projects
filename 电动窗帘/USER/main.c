#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "remote.h"
#include "oled.h" 
#include "pwm.h"
#include "timer.h"
#include "Balance.h"

//ALIENTEK ̽����STM32F407������ ʵ��29
//����ң����ʵ�� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

/* ���� 
oled scl -- PD6
		 sda -- PD5
		������չ�
		���ն� PA8
*/
/*

�������������
������ 
������A�� --- D12
������B�� --- D13
*/


extern void TIM3_Int_Init(u16 arr,u16 psc);
extern void TIM5_Int_Init(u16 arr,u16 psc);
extern void Encoder_Init_TIM4(u16 arr,u16 psc);
extern u32 Read_Encoder(void);

int encode;
float speed,t;
float sum = 0.0f;
//extern int Encoder_Timer_Overflow;


void TIM5_IRQHandler(void)                         
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET)     
	{
		encode=Read_Encoder();                          
		printf("50ms�ڱ���������Ϊ��%d\r\n",encode);
		speed=(float)(encode*1.0/(500*30*0.5));
		printf("���ת�٣�%.4f\r\n",speed);
		sum += encode;
		printf("%.4f",sum);
		}	
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);       
}
//1Ȧ390 * 4 ������
//Ȧ�� = ������ / ����ʱ��
// s = encode / 

int main(void)
{
	u8 key;
	u8 duty = 10;
	u8 *str=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(9600);		//��ʼ�����ڲ�����Ϊ9600
	OLED_Init();				//��ʼ��OLED
 	Remote_Init();				//������ճ�ʼ��
	TIM3_Int_Init(500-1,8400-1); //500msִ��һ�� //�����  ʹ��TIM3b
	TIM5_Int_Init(5000-1,8400-1); // 84000000 / 8400 = 10000   5000 / 10000 = 0.5s ����һ������������
  TIM14_PWM_Init(200-1,8400-1); //����20ms //����� pa7
	TIM13_PWM_Init(200-1,8400-1); //����20ms //����� pa6
	
	Encoder_Init_TIM4((500)*4,1-1); //��ʱ��4���������AB�������
	
	//GPIO_SetBits(GPIOA,GPIO_Pin_5);
	while(1)
	{
		//TIM_SetCompare1(TIM14,10);
		OLED_Refresh();
		delay_ms(200);
		OLED_Clear();
		OLED_ShowString(10,10,"jian zhi:",8,1);
		
		TIM_SetCompare1(TIM14,0);
		delay_ms(1000);
		TIM_SetCompare1(TIM14,50);
		
		key=Remote_Scan();	
		if(key)
		{	 
			switch(key)
			{
				case 162: {
					str="1"; 
					GPIO_SetBits(GPIOA,GPIO_Pin_5);
					TIM_SetCompare1(TIM14,0);
					TIM_SetCompare1(TIM13,50);
					//t = x / 20 * 200
					//ռ�ձ�Ϊ = x / 20 * 200;
					OLED_ShowString(10,10,"kaishi",8,1);
				break;
				}
				case 98:{
					str="2";
					GPIO_SetBits(GPIOA,GPIO_Pin_5);
					TIM_SetCompare1(TIM14,50);
					TIM_SetCompare1(TIM13,0);
					OLED_ShowString(10,10,"fanzhuan",8,1);
					break;
				}
				case 226:
				{
					str="3";
					GPIO_SetBits(GPIOA,GPIO_Pin_5);
					OLED_ShowString(10,10,"tingzhi",8,1);
					TIM_SetCompare1(TIM14,0);
					TIM_SetCompare1(TIM14,0);
					break;
				}	    		
				case 34:{
					str="4";
					duty += 10;
					OLED_ShowString(10,10,"111",8,1);
					break;
				}					
				case 2:str="5";break;		  
				case 194:str="6";break;	   
				case 224:str="7";break;		   
				case 168:str="8";break;		  
				case 144:str="9";break;		   
				case 152:str="0";break;		    
				case 104:str="*";break;		  
				case 176:str="#";break;	   
				case 24:str="^";break;	    
				case 16:str="<";break;		   
				case 74:str="v";break;		   
				case 90:str=">";break;		 
				case 56:str="OK";break;		 
			}
			OLED_ShowString(80,10,str,16,1);
		}
	}
}


