#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "stdio.h"
 
/************************************************
 ALIENTEK��ӢSTM32������ʵ��4
 ���� ʵ��   
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/



//��ʼ��IO��
void Io_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//�������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_15;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}





//IO��ȡ�ߵ͵�ƽ  A12
short Du_Qu_Shu_Ju(void) 
{
	uint8_t gaodi = 0;  
	
	gaodi = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);  //A12��
	
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
	USART_ClearFlag(USARTx,USART_FLAG_TC);										//�����ַ�ǰ��ձ�־λ������ȱʧ�ַ����ĵ�һ���ַ���
	while(DataString[i] != 0xff)												//�ַ���������
	{
		USART_SendData(USARTx,DataString[i]);									//ÿ�η����ַ�����һ���ַ�
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == 0);					//�ȴ����ݷ��ͳɹ�
		USART_ClearFlag(USARTx,USART_FLAG_TC);									//�����ַ�����ձ�־λ
		i++;
	}
}

void USART1_IRQHandler1(void)                	//����1�жϷ������
	{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
			{
			if(USART_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
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

	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(38400);	 //���ڳ�ʼ��Ϊ9600
  //LED_Init();			     //LED�˿ڳ�ʼ��
 KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�	
  
 	while(1)
			{
			key=KEY_Scan(0);	//�õ���ֵ		
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
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			USART_RX_STA=0;
			delay_ms(10);
			*/
	
		





	/*	
	{						
		
		key=KEY_Scan(0);	//�õ���ֵ		
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
			//�򴮿�1��������						
		  }*/

 
 
 
 
 
	/*
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\n�����͵���ϢΪ:\r\n\r\n");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			printf("\r\n\r\n");//���뻻��
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%5000==0)
			{
				printf("\r\n��ӢSTM32������ ����ʵ��\r\n");
				printf("����ԭ��@ALIENTEK\r\n\r\n");
			}
			if(times%200==0)printf("����������,�Իس�������\n");  
			if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
			delay_ms(10);   
		}
		 */

