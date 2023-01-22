#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "string.h"
#include "ov7725.h"
#include "timer.h"
#include "exti.h"
#include "motor.h"
#include "spi.h"
#include "remote.h"
#include "hc05.h"
#include "usart3.h"
#include "key.h"
#include "led.h"

/************************************************
 ALIENTEK��ӢSTM32������ʵ��33
 ����ͷOV7725 ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
//����OV7725��������װ��ʽԭ��,OV7725_WINDOW_WIDTH�൱��LCD�ĸ߶ȣ�OV7725_WINDOW_HEIGHT�൱��LCD�Ŀ��

//ע�⣺�˺궨��ֻ��OV7725��Ч
#define  OV7725_WINDOW_WIDTH	188//320	//320 	// <=320
#define  OV7725_WINDOW_HEIGHT	120//240	//240 	// <=240

const u8*LMODE_TBL[6]={"Auto","Sunny","Cloudy","Office","Home","Night"};//6�ֹ���ģʽ	    
const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7����Ч 

extern u8 ov_sta;	//��exit.c�� �涨��
extern u8 ov_frame;	//��timer.c���涨��

u8 Img_Value[OV7725_WINDOW_WIDTH][OV7725_WINDOW_HEIGHT];

//����LCD��ʾ(OV7725)
void OV7725_camera_refresh(void)
{
	u32 i,j;
 	u16 color;	
	if(ov_sta) //��֡�жϸ���
	{
		LCD_Scan_Dir(U2D_L2R);		//���ϵ���,������
		LCD_Set_Window(0,0,600,480);
		//LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//����ʾ�������õ���Ļ����
		if(lcddev.id==0X1963)
		LCD_Set_Window(0,0,480,600);
		//LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_HEIGHT,OV7725_WINDOW_WIDTH);//����ʾ�������õ���Ļ����
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		OV7725_RRST=0;				//��ʼ��λ��ָ�� 
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST=1;				//��λ��ָ����� 
		OV7725_RCK_H;
		for(i=0;i<OV7725_WINDOW_HEIGHT;i++)
		{
			for(j=0;j<OV7725_WINDOW_WIDTH;j++)
			{
				OV7725_RCK_L;
				color=GPIOC->IDR&0XFF;	//������
				OV7725_RCK_H;
				color<<=8;
				OV7725_RCK_L;
				color|=GPIOC->IDR&0XFF;	//������
				OV7725_RCK_H;
				LCD->LCD_RAM=color;
				Img_Value[i][j] = color; //ͼ�����ݴ洢
				//for(i = 0; i < OV7725_WINDOW_WIDTH; i++){
					//for(j = 0; j < OV7725_WINDOW_HEIGHT; j++){
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET); //ѭ������,ֱ���������
						USART_SendData(USART1,color);
				//	}
				//}
			}
		}
 		ov_sta=0;					//����֡�жϱ��
		ov_frame++;
		LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽��
	}
}

//����ͷ��ʼ��
void sxt_init(){
	u8 msgbuf[15];//��Ϣ������
	u8 lightmode=0,effect=0;
	s8 saturation=0,brightness=0,contrast=0;
	 if(OV7725_Init()==0){
			OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,0); //QVGAģʽ���
	 		OV7725_Light_Mode(lightmode);
			OV7725_Color_Saturation(saturation);
			OV7725_Brightness(brightness);
			OV7725_Contrast(contrast);
			OV7725_Special_Effects(effect);
			OV7725_CS=0;
	 }
		TIM6_Int_Init(10000,7199);			//10Khz����Ƶ��,1�����ж�
		EXTI8_Init();					    //ʹ���ⲿ�ж�8,����֡�ж�
		LCD_Clear(BLACK);  //����ɫ
		OV7725_Light_Mode(4);
		OV7725_Color_Saturation(4);
		OV7725_Contrast(4);
		OV7725_Special_Effects(0);		//������Ч
		sprintf((char*)msgbuf,"%s",LMODE_TBL[lightmode]);
		sprintf((char*)msgbuf,"Saturation:%d",saturation);
		sprintf((char*)msgbuf,"Contrast:%d",contrast);
		sprintf((char*)msgbuf,"%s",EFFECTS_TBL[effect]);
	 	POINT_COLOR=RED;			//��������Ϊ��ɫ  
}

//��ʾATK-HC05ģ�������״̬
void HC05_Role_Show(void)
{
	if(HC05_Get_Role()==1)LCD_ShowString(30,150,200,16,16,"ROLE:Master");	//����
	else LCD_ShowString(30,150,200,16,16,"ROLE:Slave ");			 		//�ӻ�
}

//��ʾATK-HC05ģ�������״̬
void HC05_Sta_Show(void)
{
	if(HC05_LED)LCD_ShowString(120,150,120,16,16,"STA:Connected ");			//���ӳɹ�
	else LCD_ShowString(120,150,120,16,16,"STA:Disconnect");	 			//δ����				 
}


 int main(void)
 {
	u8 t;
	u8 sendmask=0;
	u8 sendcnt=0;
	u8 sendbuf[20];	  
	u8 reclen=0;  
	u8 key = 0;
	u8 *str=0;
	u8 HC05_flag = 0;
	u8 i,j;
	
	delay_init();	    	 //��ʱ������ʼ��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(921600);	 	//���ڳ�ʼ��Ϊ9600
	LCD_Init();			   		//��ʼ��LCD
	sxt_init(); 		// ����ͷ��ʼ��
	//main_Init();   //ֱ�����IO�ڳ�ʼ��
	Remote_Init();		//������ճ�ʼ��
	LED_Init();
	//KEY_Init();
	//SPI1_Init();
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_256); //SPI�ٶ�256��Ƶ
	 
	//������ʼ��
	POINT_COLOR=RED;
	LCD_ShowString(30,30,200,16,16,"HC05 ^_^ BLUETOOTH");
	delay_ms(1000);			//�ȴ�����ģ���ϵ��ȶ�
 	while(HC05_Init()) 		//��ʼ��ATK-HC05ģ��
	{
		LCD_ShowString(30,50,200,16,16,"ATK-HC05 Error!"); 
		delay_ms(500);
		LCD_ShowString(30,50,200,16,16,"Please Check!!!"); 
		delay_ms(500);
	}
	LCD_ShowString(30,70,210,16,16,"KEY1:ROLE KEY0:SEND/STOP");
	LCD_ShowString(30,90,200,16,16,"ATK-HC05 Standby!");
  LCD_ShowString(30,110,200,16,16,"Send:");
	LCD_ShowString(30,130,200,16,16,"Receive:");
	POINT_COLOR=BLUE;
	HC05_Role_Show();
	delay_ms(100);
	USART3_RX_STA=0;
	
	//ң����ʾ����
  LCD_ShowString(30,200,200,16,16,"KEYVAL:");
  LCD_ShowString(30,220,200,16,16,"KEYCNT:");
  LCD_ShowString(30,240,200,16,16,"SYMBOL:");
	//�����ʼ���ٶ�
	//TIM_SetCompare1(TIM4,100);
	//TIM_SetCompare2(TIM4,100);
	//TIM_SetCompare3(TIM4,CRR3 + amplitude);  //������Ӱ����ʱȥ��
	//TIM_SetCompare4(TIM4,100);
	
	/*
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY1_PRES)						//�л�ģ����������
		{
   		key=HC05_Get_Role();
			if(key!=0XFF)
			{
				key=!key;  					//״̬ȡ��	   
				if(key==0)HC05_Set_Cmd("AT+ROLE=0");
				else HC05_Set_Cmd("AT+ROLE=1");
				HC05_Role_Show();
				HC05_Set_Cmd("AT+RESET");	//��λATK-HC05ģ��
				delay_ms(200);
			}
		}
		
		else if(key==KEY0_PRES)
		{
			sendmask=!sendmask;				//����/ֹͣ����
			if(sendmask==0)LCD_Fill(30+40,160,240,160+16,WHITE);//�����ʾ
		}else delay_ms(10);
		
		if(t==50)
		{
			if(sendmask)					//��ʱ����
			{
				sprintf((char*)sendbuf,"ALIENTEK HC05 %d\r\n",sendcnt);
	  		LCD_ShowString(30+40,160,200,16,16,sendbuf);	//��ʾ��������
				u3_printf("ALIENTEK HC05 %d\r\n",sendcnt);		//���͵�����ģ��
				sendcnt++;
				if(sendcnt>99)sendcnt=0;
			}
			HC05_Sta_Show();
			t=0;
			LED0=!LED0;
		}
		
		if(USART3_RX_STA&0X8000)			//���յ�һ��������
		{
			LCD_Fill(30,200,240,320,WHITE);	//�����ʾ
 			reclen=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
		  USART3_RX_BUF[reclen]=0;	 	//���������
			if(reclen==2||reclen==3) 		//����DS1���  ��һ���ж�
			{
				if(strcmp((const char*)USART3_RX_BUF,"11")==0) LED1=0;
					//��LED1`
				if(strcmp((const char*)USART3_RX_BUF,"22")==0) LED1=1;
				//�ر�LED1
			}
 			LCD_ShowString(30,200,209,119,16,USART3_RX_BUF);//��ʾ���յ�������
			printf("%s",USART3_RX_BUF);
 			USART3_RX_STA=0;	 
		}
		t++;
	}
	*/
	
	while(1)//��ʼ��OV7725
	{
		OV7725_camera_refresh();		//��ʾ����ͷͼ��
		
		//ң�ؽ���
		key=Remote_Scan();
		if(key)
		{
			HC05_flag = 0;
			LCD_ShowNum(86,130,key,3,16);		//��ʾ��ֵ
			LCD_ShowNum(86,150,RmtCnt,3,16);	//��ʾ��������
			switch(key)
			{
				case 16:{str="<"; //��ת
					LCD_ShowNum(30,350,1,3,16);		//��ʾ��ֵ
					//Left(90);
					LED0 = 0;
					break;
				}
				case 90:{str=">"; //��ת
					LCD_ShowNum(30,350,2,3,16);		//��ʾ��ֵ
					//Right(90);
					LED0 = 0;
					break;
				}
				case 24:{str="^"; //ǰ��
					Go();
				break;
				}
				case 74:{str="v"; //����
					Back();
					break;
				}
				case 56:{str="OK"; //ԭ����ת
					Ring();
					break;
				}
				case 162:{str="1"; //ֹͣ
					Stop();
					break;
				}
				case 98:{str="2"; //����
					Hasten(10);
					break;
				}
				case 226:{str="3"; //����
					Decelerate(10);
					break;
				}
				case 176:{str="#";   //�л���������
					break;
				}
				//��������
				case 104: {str="*";  //�л�����������
					while(1){
						OV7725_camera_refresh();		//��ʾ����ͷͼ��
						key=Remote_Scan();
						if(key == 104) HC05_flag++;
						if(USART3_RX_STA&0X8000)			//���յ�һ��������
						{
						LCD_Fill(30,200,240,320,BLACK);	//�����ʾ
						reclen=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
						USART3_RX_BUF[reclen]=0;	 	//���������
						if(reclen==2||reclen==3){//����DS1���  ��һ���ж�
						if(strcmp((const char*)USART3_RX_BUF,"11")==0){LED0 = 1; LCD_ShowString(86,240,200,16,16,"zuozhuan");}	//��ʾ����
						if(strcmp((const char*)USART3_RX_BUF,"22")==0){LED0 = 0; LCD_ShowString(86,240,200,16,16,"youzhuan");}	//��ʾ����
						if(strcmp((const char*)USART3_RX_BUF,"33")==0){LCD_ShowString(86,240,200,16,16,"qianjin");}	//��ʾ����							
						if(strcmp((const char*)USART3_RX_BUF,"44")==0){LCD_ShowString(86,240,200,16,16,"houtui");}	//��ʾ����							
						if(strcmp((const char*)USART3_RX_BUF,"55")==0){LCD_ShowString(86,240,200,16,16,"tingzhi");}	//��ʾ����		
						if(strcmp((const char*)USART3_RX_BUF,"66")==0){LCD_ShowString(86,240,200,16,16,"yuandizhuan");}	//��ʾ����								
						if(strcmp((const char*)USART3_RX_BUF,"77")==0){LCD_ShowString(86,240,200,16,16,"jiasu");}	//��ʾ����		
						if(strcmp((const char*)USART3_RX_BUF,"88")==0){LCD_ShowString(86,240,200,16,16,"jiansu");}	//��ʾ����	
						if(strcmp((const char*)USART3_RX_BUF,"99")==0){LCD_ShowString(86,240,200,16,16,"qianjin");}	//��ʾ����
						if(strcmp((const char*)USART3_RX_BUF,"10")==0){HC05_flag++;}	//��ʾ����
						}
						USART3_RX_STA=0;
					}
						if(HC05_flag >= 3) break; //�˳���������
						LCD_Fill(86,240,116+8*8,170+16,BLACK);	//���֮ǰ����ʾ
						LCD_ShowString(86,240,200,16,16,str);	//��ʾSYMBOL
						delay_ms(10);
					}
					break; //�˳���switch
			}
				case 34:str="4";break;		 
				case 2:str="5";break;		  
				case 194:str="6";break;	   
				case 224:str="7";break;		   
				case 168:str="8";break;		  
				case 144:str="9";break;		   
				case 152:str="0";break;		    
			}
			LCD_Fill(86,240,116+8*8,170+16,WHITE);	//���֮ǰ����ʾ
			LCD_ShowString(86,240,200,16,16,str);	//��ʾSYMBOL
			delay_ms(10);
		}
		//Left(11);
	}
}















