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

//ALIENTEK 探索者STM32F407开发板 实验29
//红外遥控器实验 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

/* 接线 
oled scl -- PD6
		 sda -- PD5
		红外接收管
		接收端 PA8
*/
/*

编码器电机接线
编码器 
编码器A相 --- D12
编码器B相 --- D13
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
		printf("50ms内编码器脉冲为：%d\r\n",encode);
		speed=(float)(encode*1.0/(500*30*0.5));
		printf("电机转速：%.4f\r\n",speed);
		sum += encode;
		printf("%.4f",sum);
		}	
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);       
}
//1圈390 * 4 个脉冲
//圈数 = 脉冲数 / 采样时间
// s = encode / 

int main(void)
{
	u8 key;
	u8 duty = 10;
	u8 *str=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(9600);		//初始化串口波特率为9600
	OLED_Init();				//初始化OLED
 	Remote_Init();				//红外接收初始化
	TIM3_Int_Init(500-1,8400-1); //500ms执行一次 //电机用  使能TIM3b
	TIM5_Int_Init(5000-1,8400-1); // 84000000 / 8400 = 10000   5000 / 10000 = 0.5s 进行一次脉冲数做差
  TIM14_PWM_Init(200-1,8400-1); //周期20ms //电机用 pa7
	TIM13_PWM_Init(200-1,8400-1); //周期20ms //电机用 pa6
	
	Encoder_Init_TIM4((500)*4,1-1); //定时器4处理编码器AB相的数据
	
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
					//占空比为 = x / 20 * 200;
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


