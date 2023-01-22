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

//Mini STM32F103开发板实验
//AS608指纹识别模块串口通讯实验-库函数版本   

//作者：沈彦明 @SYM

#define usart2_baund  57600//串口2波特率，根据指纹模块波特率更改（注意：指纹模块默认57600）
SysPara AS608Para;//指纹模块AS608参数
u16 ValidN;//模块内有效模板个数
u8** kbd_tbl;

void Add_FR(void);	//录指纹
void Del_FR(void);	//删除指纹
void press_FR(void);//刷指纹

void ShowErrMessage(u8 ensure);//显示确认码错误信息 

int main(void)
{		 
	u8 ensure;
	u8 key_num;
	char *str;
	
	//初始化函数
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();  	    //初始化延时函数
	uart_init(115200);	//初始化串口1波特率为115200，用于支持USMART
	usart2_init(usart2_baund);//初始化串口2,用于与指纹模块通讯
	PS_StaGPIO_Init();	//初始化FR读状态引脚
	KEY_Init();					//按键初始化 
	mem_init();					//初始化内存池 
	exfuns_init();			//为fatfs相关变量申请内存  
 	f_mount(fs[1],"1:",1);  //挂载FLASH.
	POINT_COLOR=RED;
	
	
	
	/*提醒用户语句*/
	printf("AS608 Fingerprint module test\r\n");
	printf("Author: SYM\r\n");
	printf("Connect with AS608.....\r\n");
	while(PS_HandShake(&AS608Addr))//与AS608模块握手
	{
		printf("Cannot connect with AS608!\r\n");
		delay_ms(1000);
		printf("Try to connect again....\r\n");
		delay_ms(1000);	  
	}
	//连接模块成功，初始化
	printf("Connect Success!\r\n");
	str=mymalloc(30);
	sprintf(str,"Baudrate:%d   Addr:%x",usart2_baund,AS608Addr);//显示波特率
	printf("%s",str);
	delay_ms(100);
	ensure=PS_ValidTempleteNum(&ValidN);//读库指纹个数
	if(ensure!=0x00)
		ShowErrMessage(ensure);//显示确认码错误信息	
	ensure=PS_ReadSysPara(&AS608Para);  //读AS608模块参数 
	if(ensure==0x00)
	{
		mymemset(str,0,50);
		sprintf(str,"RemainNum:%d    Level:%d",AS608Para.PS_max-ValidN,AS608Para.PS_level);//显示剩余指纹数量和安全等级
		printf("%s",str);
	}
	else
		ShowErrMessage(ensure);	
	myfree(str);

	
	
	/*
	核心代码部分：
	在死循环里，不断对按键1与按键0扫描，如果按键0按下则为删除指纹功能，按键1按下则为录取指纹功能！
	在扫描按键的同时，如果有指纹按下，则判断该指纹是否与指纹库有对应，若有输出对应的位置信息
	*/
	while(1)
	{
		key_num=KEY_Scan(0);
		if(key_num)
		{
			if(key_num==KEY0_PRES)Del_FR();		//删指纹
			if(key_num==KEY1_PRES)Add_FR();		//录指纹									
		}
		if(PS_Sta)	 //检测PS_Sta状态，如果有手指按下
		{
			press_FR();//刷指纹			
		}				 
	} 	
}



//显示确认码错误信息
void ShowErrMessage(u8 ensure)
{
	//LCD_Fill(0,120,lcddev.width,160,WHITE);
	Show_Str_Mid(0,120,(u8*)EnsureMessage(ensure),16,240);
	printf("%s",EnsureMessage(ensure));
	delay_ms(1000);
}



//录指纹功能
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
				printf("请按手指！\r\n");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//生成特征
					if(ensure==0x00)
					{
						
						//指纹正确
						printf("指纹正确\r\n");
						i=0;
						processnum=1;//跳到第二步						
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);						
				break;
			
			case 1:
				i++;
				printf("再按一次手指\r\n");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure= (CharBuffer2);//生成特征			
					if(ensure==0x00)
					{
						printf("指纹正确\r\n");
						i=0; 
						processnum=2;//跳到第三步
					}else ShowErrMessage(ensure);	
				}else ShowErrMessage(ensure);		
				break;

			case 2:
				printf("对比两次指纹\r\n");
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					printf("两次指纹对比一样\r\n");
					processnum=3;//跳到第四步
				}
				else 
				{
					printf("对比失败，请重按手指！\r\n");
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//跳回第一步		
				}
				delay_ms(1000);
				break;

			case 3:
				printf("产生一个指纹模块\r\n");
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					printf("生成指纹模板成功\r\n");
					processnum=4;//跳到第五步
				}else {processnum=0;ShowErrMessage(ensure);}
				delay_ms(1000);
				break;
				
			case 4:	
				printf("请输入该指纹模块存放的位置（必须小于300）：\r\n");
				while(1)
				{
					if(USART_RX_STA&0X8000)
					{
						len1=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
						printf("\r\n您发送的位置为:\r\n");
						for(a1=0;a1<len1;a1++)
						{
								a1=a1*10+USART_RX_BUF[a1]-'0';
						}		
							a1=a1-1;
					
					for(t1=0;t1<len1;t1++)
					{
						USART1->DR=USART_RX_BUF[t1];
						while((USART1->SR&0X40)==0);//等待发送结束
					}
						printf("\r\n\r\n");//插入换行
						USART_RX_STA=0;
					}
					if(a1>0)
						break;
				}				
				ensure=PS_StoreChar(CharBuffer2,a1);//储存模板
				if(ensure==0x00) 
				{			

					printf("添加指纹成功！\r\n");
					PS_ValidTempleteNum(&ValidN);//读库指纹个数
					//显示剩余指纹个数
					printf("剩余指纹容量:%d\r\n",AS608Para.PS_max-ValidN);
					delay_ms(1500);//延时后清除显示	
					printf("请继续你的表演！");
					return ;
				}else {processnum=0;ShowErrMessage(ensure);}					
				break;				
		}
		delay_ms(1000);
		if(i==5)//超过5次没有按手指则退出
		{
			printf("现在退出了！\r\n");
			break;	
		}				
	}
}



//刷指纹功能
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *str;
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//搜索成功
			{						
				printf("搜索指纹成功\r\n");
				str=mymalloc(50);
				sprintf(str,"Match ID:%d  Match score:%d",seach.pageID,seach.mathscore);//显示匹配指纹的ID和分数
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



//删除指纹功能
void Del_FR(void)
{
	u8  ensure;
	u8 len;	
	u8 t;
	u8 a;
	printf("删除指纹：\r\n");
	printf("请输入要删除指纹的位置:(范围：0=< ID <=299)\r\n");
	
	while(1)
	{
		//接受串口信息的核心部分代码！
		if(USART_RX_STA&0X8000)
		{
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的位置为:\r\n");
			//下面代码的功能是将串口缓冲区的数组，转换成int，默认+1.所以-1
			for(a=0;a<len;a++)
			{
					a=a*10+USART_RX_BUF[a]-'0';
			}
			a=a-1;
			for(t=0;t<len;t++)
				{
					USART1->DR=USART_RX_BUF[t];
					while((USART1->SR&0X40)==0);//等待发送结束
				}
				printf("\r\n\r\n");//插入换行
				USART_RX_STA=0;
		}
		if(a>0)
			break;
		
	
	}
	//如果接收到的是0 则清空指纹库，其他位置删除对应位置的指纹，该功能目前不能用！前面判断依据退出了
	if(a==0)
		ensure=PS_Empty();//清空指纹库
	else 
		ensure=PS_DeletChar(a,1);//删除单个指纹
	if(ensure==0)
	{
		printf("删除指纹成功\r\n");
	}
  else
		ShowErrMessage(ensure);	
	delay_ms(1500);//延时后清除显示
	PS_ValidTempleteNum(&ValidN);//读库指纹个数
	printf("剩下指纹数量：%d\r\n",AS608Para.PS_max-ValidN);
	delay_ms(50);
	printf("请继续你的表演！");
	return;

}




