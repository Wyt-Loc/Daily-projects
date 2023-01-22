#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
//������ʼ������


void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOB,GPIO_Pin_5);  
	
	GPIO_InitStructure.GPIO_Pin  = KEY1|KEY2|KEY3|KEY4;   //��  a  4 5 6 7 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0���ó����룬Ĭ������  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;  //lie  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);       //a 0 1 2 3
}


u8 Key = 0;
u8 flag=0;
u16 abc[9] = {0,0,0,0,0,0,0,0,0};

u8 i = 0;
u8 Flag = 0;
int sum1 = 0;

int sun1_read()
{
   return sum1;
}

u16 * abd_read();


u16 *abc_read()
{
		return &abc[0];
}


int Wei_Shu(u8 a)
{
	int sum = 1, i = 0;
	for(;i<a;i++)
	{
	sum =	sum*10;
	}
	return sum;
}


u16 time(u16 abc[9],u8 j)
{	
	int time = 0 ,a = j ,shijian = 0;
		for(time = 0; time < j; time++ )
	{
		shijian +=  abc[time]*Wei_Shu(a--);  
	}
	return shijian;		
}

char ok = 0;
char led_liang = 0;
void Key_Input( u8 *key1)
{	
	if(ok==1){
	
	
		if(*key1 == 16 && Flag == 0)
	{
		Flag = 1;
	}
	else if(Flag == 1)
	{		
		
	 if(*key1 == 16 && Flag == 1)
	{
		Flag = 0;
		
			sum1 = (short)(time(abc,i)/10);
			printf(" shijian =%d\r\n",sum1);
		
			 if(0<sum1)
			{
				sum1 = sum1*1000;
				led_liang = 1;
			}
			//
		for(i=0;i<9;i++)
			abc[i] = 0;	
		i = 0;	
	 }	
			else if(*key1<10)
				{
					abc[i] = *key1;
					i++;
						if(i>9)
						{
							 i = 0;
					  }
		}
	}
	
	else if(*key1 == 13){
		sum1 = 0;
					//
		for(i=0;i<9;i++)
			abc[i] = 0;	
		i = 0;	
	}
	
	
	ok = 0;
}
	
		if(led_liang==1 && sum1>0){
				GPIO_SetBits(GPIOB,GPIO_Pin_5);	
				//delay_ms(1);		
				sum1--;			
			}
		
			if(sum1 == 0)
			{
			
				
			}
			
			if(sum1==0){
				led_liang = 0;
				GPIO_ResetBits(GPIOB,GPIO_Pin_5);	
			}
}


//���󰴼����
void KEY_config(u8 *Key)
{
	char i=1,j=1;
	*Key = 0; //��ʼ��  ��ֵ
	for(;i<5;i++)
	{
		 	GPIO_ResetBits(KEY_PROT,GPIO_Pin_0<<(i-1));     //������
			if((KEY1_Input == RESET) || (KEY2_Input == RESET) || (KEY3_Input == RESET) || (KEY4_Input == RESET)) //��һ�б����� 4567 Ĭ������
			{
					delay_ms(10); //����
					for(j=1;j<5;j++) //�����һ��������
				{
						if(GPIO_ReadInputDataBit(KEY_PROT,GPIO_Pin_4<<(j-1)) == RESET) //�����µ���   Ĭ���������
					 {
							flag = 1;
							*Key = (i-1)*4+j;
							while(!GPIO_ReadInputDataBit(KEY_PROT,GPIO_Pin_4<<(j-1))){}; //�ȴ�����̧��
							//Key_Input(Key);
								ok = 1;
							break;						
						}
						else
							flag = 0;
					}
		  }
			GPIO_SetBits(GPIOA,GPIO_Pin_0<<(i-1)); //�ٴ�����
	 }
}


/*
*/

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY_UP!!
/*
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))   //! > & > ^ > | > && > ||
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0) return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	    
 	return 0;// �ް�������
}*/
