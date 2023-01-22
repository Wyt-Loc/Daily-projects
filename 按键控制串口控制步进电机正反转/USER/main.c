#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "stdio.h"
 
/************************************************
 ALIENTEK精英STM32开发板实验4
 串口 实验   
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/



//初始化IO口
void Io_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//上拉输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_15;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}





//IO读取高低电平  A12
short Du_Qu_Shu_Ju(void) 
{
	uint8_t gaodi = 0;  
	
	gaodi = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);  //A12口
	
	return gaodi;
}


short Du_Qu_Shu_chu(void)
{
	uint8_t  gaodi1 = 0;
	gaodi1 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15);
	 return gaodi1;
}












void USART_SendString(USART_TypeDef* USARTx, char *DataString)
{
	int i = 0;
	USART_ClearFlag(USARTx,USART_FLAG_TC);										//发送字符前清空标志位（否则缺失字符串的第一个字符）
	while(DataString[i] != 0xff)												//字符串结束符
	{
		USART_SendData(USARTx,DataString[i]);									//每次发送字符串的一个字符
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == 0);					//等待数据发送成功
		USART_ClearFlag(USARTx,USART_FLAG_TC);									//发送字符后清空标志位
		i++;
	}
}

void USART1_IRQHandler1(void)                	//串口1中断服务程序
	{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
} 



 int main(void)
 {		
	 char zhenfan = 0,fa=1;
	 char abc[10];
	 vu8 key=0;
 	u16 t;  
	u16 len;	
	u16 times=0;

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(38400);	 //串口初始化为9600
  //LED_Init();			     //LED端口初始化
 KEY_Init();          //初始化与按键连接的硬件接口	
  
 	while(1)
			{
			key=KEY_Scan(0);	//得到键值		
		if(key==2)
			{
				if(zhenfan == 0)
					zhenfan = 1;
				else 
					zhenfan = 0;
				fa = 1;
		}
			
		if(zhenfan && fa){
			abc[0]=0xe0;
			abc[1]=0xf6;
			abc[2]=0x10;
			abc[3]=0xff;					
		USART_SendString(USART1, abc);
			fa = 0;
		}
		else if(!zhenfan && fa){
			abc[0]=0xe0;
			abc[1]=0xf7;
			abc[2]=0xff;
		USART_SendString(USART1, abc);
			fa = 0;
		}
	}
			return 0;
}
		  	
	


		/*	
			if(USART_RX_STA&0x8000)
		{
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			USART_RX_STA=0;
			delay_ms(10);
			*/
	
		





	/*	
	{						
		
		key=KEY_Scan(0);	//得到键值		
		if(key){											
		//USART_SendString(USART1, "e0 f6 00");					
					
			/*if(Du_Qu_Shu_Ju() == 0)
			{			
			USART_SendData(USART1,0xe0);
				delay_ms(1);
			USART_SendData(USART1,0xf6);
				delay_ms(1);
			USART_SendData(USART1,0x00);
				delay_ms(1);
			//向串口1发送数据						
		  }*/

 
 
 
 
 
	/*
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的消息为:\r\n\r\n");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%5000==0)
			{
				printf("\r\n精英STM32开发板 串口实验\r\n");
				printf("正点原子@ALIENTEK\r\n\r\n");
			}
			if(times%200==0)printf("请输入数据,以回车键结束\n");  
			if(times%30==0)LED0=!LED0;//闪烁LED,提示系统正在运行.
			delay_ms(10);   
		}
		 */

