#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"
#include "stdio.h"

/*************************************
//完成时间：2019年11月18日 12:03:32
//功能介绍：实现匿名上位机飞控状态反映陀螺仪数据
//制作：程皖
//联系方式：QQ：110662822
***************************************/

/*************************************
			A_X:X轴数据 -90-90
			A_Y:Y轴数据 
			A_Z:Z轴数据 -90-90
***************************************/
extern short T_X,T_Z,T_Y;	
extern short A_X,A_Z,A_Y;    
unsigned char ANO_BUFF[17];

/*************************************
函数名称：void Send_FLY()；
功能介绍：根据匿名协议向上位机发送下位机姿态
0x88+FUN+LEN+DATA+SUM
***************************************/	
void Send_FLY()
{
	unsigned i,sum;
	READ_MPU6050();
	ANO_BUFF[0] = 0xAA;
	ANO_BUFF[1] = 0x88; //帧头
	ANO_BUFF[2] = 0xA1; //FUN
	ANO_BUFF[3] = 0x0C; //LEN
	
	ANO_BUFF[4] = A_X>>8;
	ANO_BUFF[5] = A_X<<8;
	
	ANO_BUFF[6] = A_Z>>8;
	ANO_BUFF[7] = A_Z>>8;

	A_Y = A_Y * 10;
	ANO_BUFF[8] = A_Y>>8;
	ANO_BUFF[9] = A_Y>>8; 
	
	ANO_BUFF[10] = 0x00;
	ANO_BUFF[11] = 0x00;
	ANO_BUFF[12] = 0x00;
	ANO_BUFF[13] = 0x00;
	
	ANO_BUFF[14] = 0x00;
	
	ANO_BUFF[15] = 0x01;//DATA
	sum = 0;
	for(i=1;i<16;i++)
	{
		sum=ANO_BUFF[i]+sum;
	}
	ANO_BUFF[16] = sum;
	
	for(i=1;i<17;i++)
	{
		sendByte(USART1,ANO_BUFF[i]);
	}
	
}

/*************************************
函数名称：void send_all()；
功能介绍：通过串口1向上位机发送数据（方便观测）
***************************************/	
void send_all()
{
		unsigned char buff[6];
	
		READ_MPU6050();
	
		NDATA_printf(buff,A_X);
	  printf("TX:%s\r\n",buff);
	
		NDATA_printf(buff,A_Y);
	  printf("TY:%s\r\n",buff);
	
		NDATA_printf(buff,A_Z);
	  printf("TZ:%s\r\n",buff);
		printf("---------------------------------------------\r\n");
	
}




int main(void)
 {	 

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	i2c_GPIO_Config();
	delay_ms(10);
	Init_MPU6050();
  	while(1) 
	{		 
		//send_all();
		Send_FLY();
		delay_ms(100);								//修改这里可以改变发送频率
	} 
}
 
