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
 ALIENTEK精英STM32开发板实验33
 摄像头OV7725 实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
//由于OV7725传感器安装方式原因,OV7725_WINDOW_WIDTH相当于LCD的高度，OV7725_WINDOW_HEIGHT相当于LCD的宽度

//注意：此宏定义只对OV7725有效
#define  OV7725_WINDOW_WIDTH	188//320	//320 	// <=320
#define  OV7725_WINDOW_HEIGHT	120//240	//240 	// <=240

const u8*LMODE_TBL[6]={"Auto","Sunny","Cloudy","Office","Home","Night"};//6种光照模式	    
const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7种特效 

extern u8 ov_sta;	//在exit.c里 面定义
extern u8 ov_frame;	//在timer.c里面定义

u8 Img_Value[OV7725_WINDOW_WIDTH][OV7725_WINDOW_HEIGHT];

//更新LCD显示(OV7725)
void OV7725_camera_refresh(void)
{
	u32 i,j;
 	u16 color;	
	if(ov_sta) //有帧中断更新
	{
		LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右
		LCD_Set_Window(0,0,600,480);
		//LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//将显示区域设置到屏幕中央
		if(lcddev.id==0X1963)
		LCD_Set_Window(0,0,480,600);
		//LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_HEIGHT,OV7725_WINDOW_WIDTH);//将显示区域设置到屏幕中央
		LCD_WriteRAM_Prepare();     //开始写入GRAM
		OV7725_RRST=0;				//开始复位读指针 
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST=1;				//复位读指针结束 
		OV7725_RCK_H;
		for(i=0;i<OV7725_WINDOW_HEIGHT;i++)
		{
			for(j=0;j<OV7725_WINDOW_WIDTH;j++)
			{
				OV7725_RCK_L;
				color=GPIOC->IDR&0XFF;	//读数据
				OV7725_RCK_H;
				color<<=8;
				OV7725_RCK_L;
				color|=GPIOC->IDR&0XFF;	//读数据
				OV7725_RCK_H;
				LCD->LCD_RAM=color;
				Img_Value[i][j] = color; //图像数据存储
				//for(i = 0; i < OV7725_WINDOW_WIDTH; i++){
					//for(j = 0; j < OV7725_WINDOW_HEIGHT; j++){
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET); //循环发送,直到发送完毕
						USART_SendData(USART1,color);
				//	}
				//}
			}
		}
 		ov_sta=0;					//清零帧中断标记
		ov_frame++;
		LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向
	}
}

//摄像头初始化
void sxt_init(){
	u8 msgbuf[15];//消息缓存区
	u8 lightmode=0,effect=0;
	s8 saturation=0,brightness=0,contrast=0;
	 if(OV7725_Init()==0){
			OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,0); //QVGA模式输出
	 		OV7725_Light_Mode(lightmode);
			OV7725_Color_Saturation(saturation);
			OV7725_Brightness(brightness);
			OV7725_Contrast(contrast);
			OV7725_Special_Effects(effect);
			OV7725_CS=0;
	 }
		TIM6_Int_Init(10000,7199);			//10Khz计数频率,1秒钟中断
		EXTI8_Init();					    //使能外部中断8,捕获帧中断
		LCD_Clear(BLACK);  //背景色
		OV7725_Light_Mode(4);
		OV7725_Color_Saturation(4);
		OV7725_Contrast(4);
		OV7725_Special_Effects(0);		//设置特效
		sprintf((char*)msgbuf,"%s",LMODE_TBL[lightmode]);
		sprintf((char*)msgbuf,"Saturation:%d",saturation);
		sprintf((char*)msgbuf,"Contrast:%d",contrast);
		sprintf((char*)msgbuf,"%s",EFFECTS_TBL[effect]);
	 	POINT_COLOR=RED;			//设置字体为红色  
}

//显示ATK-HC05模块的主从状态
void HC05_Role_Show(void)
{
	if(HC05_Get_Role()==1)LCD_ShowString(30,150,200,16,16,"ROLE:Master");	//主机
	else LCD_ShowString(30,150,200,16,16,"ROLE:Slave ");			 		//从机
}

//显示ATK-HC05模块的连接状态
void HC05_Sta_Show(void)
{
	if(HC05_LED)LCD_ShowString(120,150,120,16,16,"STA:Connected ");			//连接成功
	else LCD_ShowString(120,150,120,16,16,"STA:Disconnect");	 			//未连接				 
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
	
	delay_init();	    	 //延时函数初始化
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(921600);	 	//串口初始化为9600
	LCD_Init();			   		//初始化LCD
	sxt_init(); 		// 摄像头初始化
	//main_Init();   //直流电机IO口初始化
	Remote_Init();		//红外接收初始化
	LED_Init();
	//KEY_Init();
	//SPI1_Init();
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_256); //SPI速度256分频
	 
	//蓝牙初始化
	POINT_COLOR=RED;
	LCD_ShowString(30,30,200,16,16,"HC05 ^_^ BLUETOOTH");
	delay_ms(1000);			//等待蓝牙模块上电稳定
 	while(HC05_Init()) 		//初始化ATK-HC05模块
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
	
	//遥控显示区域
  LCD_ShowString(30,200,200,16,16,"KEYVAL:");
  LCD_ShowString(30,220,200,16,16,"KEYCNT:");
  LCD_ShowString(30,240,200,16,16,"SYMBOL:");
	//电机初始化速度
	//TIM_SetCompare1(TIM4,100);
	//TIM_SetCompare2(TIM4,100);
	//TIM_SetCompare3(TIM4,CRR3 + amplitude);  //蜂鸣器影响暂时去掉
	//TIM_SetCompare4(TIM4,100);
	
	/*
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY1_PRES)						//切换模块主从设置
		{
   		key=HC05_Get_Role();
			if(key!=0XFF)
			{
				key=!key;  					//状态取反	   
				if(key==0)HC05_Set_Cmd("AT+ROLE=0");
				else HC05_Set_Cmd("AT+ROLE=1");
				HC05_Role_Show();
				HC05_Set_Cmd("AT+RESET");	//复位ATK-HC05模块
				delay_ms(200);
			}
		}
		
		else if(key==KEY0_PRES)
		{
			sendmask=!sendmask;				//发送/停止发送
			if(sendmask==0)LCD_Fill(30+40,160,240,160+16,WHITE);//清除显示
		}else delay_ms(10);
		
		if(t==50)
		{
			if(sendmask)					//定时发送
			{
				sprintf((char*)sendbuf,"ALIENTEK HC05 %d\r\n",sendcnt);
	  		LCD_ShowString(30+40,160,200,16,16,sendbuf);	//显示发送数据
				u3_printf("ALIENTEK HC05 %d\r\n",sendcnt);		//发送到蓝牙模块
				sendcnt++;
				if(sendcnt>99)sendcnt=0;
			}
			HC05_Sta_Show();
			t=0;
			LED0=!LED0;
		}
		
		if(USART3_RX_STA&0X8000)			//接收到一次数据了
		{
			LCD_Fill(30,200,240,320,WHITE);	//清除显示
 			reclen=USART3_RX_STA&0X7FFF;	//得到数据长度
		  USART3_RX_BUF[reclen]=0;	 	//加入结束符
			if(reclen==2||reclen==3) 		//控制DS1检测  加一个判断
			{
				if(strcmp((const char*)USART3_RX_BUF,"11")==0) LED1=0;
					//打开LED1`
				if(strcmp((const char*)USART3_RX_BUF,"22")==0) LED1=1;
				//关闭LED1
			}
 			LCD_ShowString(30,200,209,119,16,USART3_RX_BUF);//显示接收到的数据
			printf("%s",USART3_RX_BUF);
 			USART3_RX_STA=0;	 
		}
		t++;
	}
	*/
	
	while(1)//初始化OV7725
	{
		OV7725_camera_refresh();		//显示摄像头图像
		
		//遥控接收
		key=Remote_Scan();
		if(key)
		{
			HC05_flag = 0;
			LCD_ShowNum(86,130,key,3,16);		//显示键值
			LCD_ShowNum(86,150,RmtCnt,3,16);	//显示按键次数
			switch(key)
			{
				case 16:{str="<"; //左转
					LCD_ShowNum(30,350,1,3,16);		//显示键值
					//Left(90);
					LED0 = 0;
					break;
				}
				case 90:{str=">"; //右转
					LCD_ShowNum(30,350,2,3,16);		//显示键值
					//Right(90);
					LED0 = 0;
					break;
				}
				case 24:{str="^"; //前进
					Go();
				break;
				}
				case 74:{str="v"; //后退
					Back();
					break;
				}
				case 56:{str="OK"; //原地旋转
					Ring();
					break;
				}
				case 162:{str="1"; //停止
					Stop();
					break;
				}
				case 98:{str="2"; //加速
					Hasten(10);
					break;
				}
				case 226:{str="3"; //减速
					Decelerate(10);
					break;
				}
				case 176:{str="#";   //切换蓝牙主从
					break;
				}
				//蓝牙控制
				case 104: {str="*";  //切换到蓝牙控制
					while(1){
						OV7725_camera_refresh();		//显示摄像头图像
						key=Remote_Scan();
						if(key == 104) HC05_flag++;
						if(USART3_RX_STA&0X8000)			//接收到一次数据了
						{
						LCD_Fill(30,200,240,320,BLACK);	//清除显示
						reclen=USART3_RX_STA&0X7FFF;	//得到数据长度
						USART3_RX_BUF[reclen]=0;	 	//加入结束符
						if(reclen==2||reclen==3){//控制DS1检测  加一个判断
						if(strcmp((const char*)USART3_RX_BUF,"11")==0){LED0 = 1; LCD_ShowString(86,240,200,16,16,"zuozhuan");}	//显示操作
						if(strcmp((const char*)USART3_RX_BUF,"22")==0){LED0 = 0; LCD_ShowString(86,240,200,16,16,"youzhuan");}	//显示操作
						if(strcmp((const char*)USART3_RX_BUF,"33")==0){LCD_ShowString(86,240,200,16,16,"qianjin");}	//显示操作							
						if(strcmp((const char*)USART3_RX_BUF,"44")==0){LCD_ShowString(86,240,200,16,16,"houtui");}	//显示操作							
						if(strcmp((const char*)USART3_RX_BUF,"55")==0){LCD_ShowString(86,240,200,16,16,"tingzhi");}	//显示操作		
						if(strcmp((const char*)USART3_RX_BUF,"66")==0){LCD_ShowString(86,240,200,16,16,"yuandizhuan");}	//显示操作								
						if(strcmp((const char*)USART3_RX_BUF,"77")==0){LCD_ShowString(86,240,200,16,16,"jiasu");}	//显示操作		
						if(strcmp((const char*)USART3_RX_BUF,"88")==0){LCD_ShowString(86,240,200,16,16,"jiansu");}	//显示操作	
						if(strcmp((const char*)USART3_RX_BUF,"99")==0){LCD_ShowString(86,240,200,16,16,"qianjin");}	//显示操作
						if(strcmp((const char*)USART3_RX_BUF,"10")==0){HC05_flag++;}	//显示操作
						}
						USART3_RX_STA=0;
					}
						if(HC05_flag >= 3) break; //退出蓝牙控制
						LCD_Fill(86,240,116+8*8,170+16,BLACK);	//清楚之前的显示
						LCD_ShowString(86,240,200,16,16,str);	//显示SYMBOL
						delay_ms(10);
					}
					break; //退出大switch
			}
				case 34:str="4";break;		 
				case 2:str="5";break;		  
				case 194:str="6";break;	   
				case 224:str="7";break;		   
				case 168:str="8";break;		  
				case 144:str="9";break;		   
				case 152:str="0";break;		    
			}
			LCD_Fill(86,240,116+8*8,170+16,WHITE);	//清楚之前的显示
			LCD_ShowString(86,240,200,16,16,str);	//显示SYMBOL
			delay_ms(10);
		}
		//Left(11);
	}
}















