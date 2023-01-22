#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "string.h"
#include "ov7725.h"
#include "tpad.h"
#include "timer.h"
#include "exti.h"
#include "usmart.h"
#include "motor.h"
#include "spi.h"
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
#define  OV7725_WINDOW_WIDTH		320 	// <=320
#define  OV7725_WINDOW_HEIGHT		240 	// <=240

const u8*LMODE_TBL[6]={"Auto","Sunny","Cloudy","Office","Home","Night"};//6种光照模式	    
const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7种特效 

extern u8 ov_sta;	//在exit.c里 面定义
extern u8 ov_frame;	//在timer.c里面定义


//更新LCD显示(OV7725)
void OV7725_camera_refresh(void)
{
	u32 i,j;
 	u16 color;	 
	if(ov_sta)//有帧中断更新
	{
		LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右 
		LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//将显示区域设置到屏幕中央
		if(lcddev.id==0X1963)
		LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_HEIGHT,OV7725_WINDOW_WIDTH);//将显示区域设置到屏幕中央
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


 int main(void)
 {
	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为 115200
 	usmart_dev.init(72);		//初始化USMART		
	LCD_Init();			   		//初始化LCD
	TIM_7_init(9999,7199);
	sxt_init();
	main_Init();
	SPI_Master_Init();
	
	while(1)//初始化OV7725
	{
		byte = SPI2_ReadWriteByte(5);
		OV7725_camera_refresh();		//显示摄像头图像
	}
	
}











