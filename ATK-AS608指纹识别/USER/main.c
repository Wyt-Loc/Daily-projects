#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "touch.h"	  
#include "malloc.h"  
#include "MMC_SD.h"   
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "usart2.h"
#include "AS608.h"
#include "timer.h"

//Mini STM32F103������ʵ��
//AS608ָ��ʶ��ģ�鴮��ͨѶʵ��-�⺯���汾   

//���ߣ������� @SYM

#define usart2_baund  57600//����2�����ʣ�����ָ��ģ�鲨���ʸ��ģ�ע�⣺ָ��ģ��Ĭ��57600��
SysPara AS608Para;//ָ��ģ��AS608����
u16 ValidN;//ģ������Чģ�����
u8** kbd_tbl;

void Add_FR(void);	//¼ָ��
void Del_FR(void);	//ɾ��ָ��
void press_FR(void);//ˢָ��

void ShowErrMessage(u8 ensure);//��ʾȷ���������Ϣ 

int main(void)
{		 
	u8 ensure;
	u8 key_num;
	char *str;
	
	//��ʼ������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();  	    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ������1������Ϊ115200������֧��USMART
	usart2_init(usart2_baund);//��ʼ������2,������ָ��ģ��ͨѶ
	PS_StaGPIO_Init();	//��ʼ��FR��״̬����
	KEY_Init();					//������ʼ�� 
	mem_init();					//��ʼ���ڴ�� 
	exfuns_init();			//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[1],"1:",1);  //����FLASH.
	POINT_COLOR=RED;
	
	
	
	/*�����û����*/
	printf("AS608 Fingerprint module test\r\n");
	printf("Author: SYM\r\n");
	printf("Connect with AS608.....\r\n");
	while(PS_HandShake(&AS608Addr))//��AS608ģ������
	{
		printf("Cannot connect with AS608!\r\n");
		delay_ms(1000);
		printf("Try to connect again....\r\n");
		delay_ms(1000);	  
	}
	//����ģ��ɹ�����ʼ��
	printf("Connect Success!\r\n");
	str=mymalloc(30);
	sprintf(str,"Baudrate:%d   Addr:%x",usart2_baund,AS608Addr);//��ʾ������
	printf("%s",str);
	delay_ms(100);
	ensure=PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
	if(ensure!=0x00)
		ShowErrMessage(ensure);//��ʾȷ���������Ϣ	
	ensure=PS_ReadSysPara(&AS608Para);  //��AS608ģ����� 
	if(ensure==0x00)
	{
		mymemset(str,0,50);
		sprintf(str,"RemainNum:%d    Level:%d",AS608Para.PS_max-ValidN,AS608Para.PS_level);//��ʾʣ��ָ�������Ͱ�ȫ�ȼ�
		printf("%s",str);
	}
	else
		ShowErrMessage(ensure);	
	myfree(str);

	
	
	/*
	���Ĵ��벿�֣�
	����ѭ������϶԰���1�밴��0ɨ�裬�������0������Ϊɾ��ָ�ƹ��ܣ�����1������Ϊ¼ȡָ�ƹ��ܣ�
	��ɨ�谴����ͬʱ�������ָ�ư��£����жϸ�ָ���Ƿ���ָ�ƿ��ж�Ӧ�����������Ӧ��λ����Ϣ
	*/
	while(1)
	{
		key_num=KEY_Scan(0);
		if(key_num)
		{
			if(key_num==KEY0_PRES)Del_FR();		//ɾָ��
			if(key_num==KEY1_PRES)Add_FR();		//¼ָ��									
		}
		if(PS_Sta)	 //���PS_Sta״̬���������ָ����
		{
			press_FR();//ˢָ��			
		}				 
	} 	
}



//��ʾȷ���������Ϣ
void ShowErrMessage(u8 ensure)
{
	//LCD_Fill(0,120,lcddev.width,160,WHITE);
	Show_Str_Mid(0,120,(u8*)EnsureMessage(ensure),16,240);
	printf("%s",EnsureMessage(ensure));
	delay_ms(1000);
}



//¼ָ�ƹ���
void Add_FR(void)
{
	u8 i=0,ensure ,processnum=0;
	u8 len1;
	u8 t1;
	u8 a1;
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
				printf("�밴��ָ��\r\n");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//��������
					if(ensure==0x00)
					{
						
						//ָ����ȷ
						printf("ָ����ȷ\r\n");
						i=0;
						processnum=1;//�����ڶ���						
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);						
				break;
			
			case 1:
				i++;
				printf("�ٰ�һ����ָ\r\n");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure= (CharBuffer2);//��������			
					if(ensure==0x00)
					{
						printf("ָ����ȷ\r\n");
						i=0; 
						processnum=2;//����������
					}else ShowErrMessage(ensure);	
				}else ShowErrMessage(ensure);		
				break;

			case 2:
				printf("�Ա�����ָ��\r\n");
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					printf("����ָ�ƶԱ�һ��\r\n");
					processnum=3;//�������Ĳ�
				}
				else 
				{
					printf("�Ա�ʧ�ܣ����ذ���ָ��\r\n");
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//���ص�һ��		
				}
				delay_ms(1000);
				break;

			case 3:
				printf("����һ��ָ��ģ��\r\n");
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					printf("����ָ��ģ��ɹ�\r\n");
					processnum=4;//�������岽
				}else {processnum=0;ShowErrMessage(ensure);}
				delay_ms(1000);
				break;
				
			case 4:	
				printf("�������ָ��ģ���ŵ�λ�ã�����С��300����\r\n");
				while(1)
				{
					if(USART_RX_STA&0X8000)
					{
						len1=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
						printf("\r\n�����͵�λ��Ϊ:\r\n");
						for(a1=0;a1<len1;a1++)
						{
								a1=a1*10+USART_RX_BUF[a1]-'0';
						}		
							a1=a1-1;
					
					for(t1=0;t1<len1;t1++)
					{
						USART1->DR=USART_RX_BUF[t1];
						while((USART1->SR&0X40)==0);//�ȴ����ͽ���
					}
						printf("\r\n\r\n");//���뻻��
						USART_RX_STA=0;
					}
					if(a1>0)
						break;
				}				
				ensure=PS_StoreChar(CharBuffer2,a1);//����ģ��
				if(ensure==0x00) 
				{			

					printf("���ָ�Ƴɹ���\r\n");
					PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
					//��ʾʣ��ָ�Ƹ���
					printf("ʣ��ָ������:%d\r\n",AS608Para.PS_max-ValidN);
					delay_ms(1500);//��ʱ�������ʾ	
					printf("�������ı��ݣ�");
					return ;
				}else {processnum=0;ShowErrMessage(ensure);}					
				break;				
		}
		delay_ms(1000);
		if(i==5)//����5��û�а���ָ���˳�
		{
			printf("�����˳��ˣ�\r\n");
			break;	
		}				
	}
}



//ˢָ�ƹ���
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *str;
	ensure=PS_GetImage();
	if(ensure==0x00)//��ȡͼ��ɹ� 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //���������ɹ�
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//�����ɹ�
			{						
				printf("����ָ�Ƴɹ�\r\n");
				str=mymalloc(50);
				sprintf(str,"Match ID:%d  Match score:%d",seach.pageID,seach.mathscore);//��ʾƥ��ָ�Ƶ�ID�ͷ���
				printf("%s\r\n",str);
				myfree(str);
			}
			else 
				ShowErrMessage(ensure);					
	  }
		else
			ShowErrMessage(ensure);
	 delay_ms(1000);
	}
		
}



//ɾ��ָ�ƹ���
void Del_FR(void)
{
	u8  ensure;
	u8 len;	
	u8 t;
	u8 a;
	printf("ɾ��ָ�ƣ�\r\n");
	printf("������Ҫɾ��ָ�Ƶ�λ��:(��Χ��0=< ID <=299)\r\n");
	
	while(1)
	{
		//���ܴ�����Ϣ�ĺ��Ĳ��ִ��룡
		if(USART_RX_STA&0X8000)
		{
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\n�����͵�λ��Ϊ:\r\n");
			//�������Ĺ����ǽ����ڻ����������飬ת����int��Ĭ��+1.����-1
			for(a=0;a<len;a++)
			{
					a=a*10+USART_RX_BUF[a]-'0';
			}
			a=a-1;
			for(t=0;t<len;t++)
				{
					USART1->DR=USART_RX_BUF[t];
					while((USART1->SR&0X40)==0);//�ȴ����ͽ���
				}
				printf("\r\n\r\n");//���뻻��
				USART_RX_STA=0;
		}
		if(a>0)
			break;
		
	
	}
	//������յ�����0 �����ָ�ƿ⣬����λ��ɾ����Ӧλ�õ�ָ�ƣ��ù���Ŀǰ�����ã�ǰ���ж������˳���
	if(a==0)
		ensure=PS_Empty();//���ָ�ƿ�
	else 
		ensure=PS_DeletChar(a,1);//ɾ������ָ��
	if(ensure==0)
	{
		printf("ɾ��ָ�Ƴɹ�\r\n");
	}
  else
		ShowErrMessage(ensure);	
	delay_ms(1500);//��ʱ�������ʾ
	PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
	printf("ʣ��ָ��������%d\r\n",AS608Para.PS_max-ValidN);
	delay_ms(50);
	printf("�������ı��ݣ�");
	return;

}




