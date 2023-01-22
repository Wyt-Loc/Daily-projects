#include "ds18b20.h"
#include "delay.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//DS18B20驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
/************
2022/4/5   DS18B20重写
wyt
******************/
#define  DS18B20 GPIO_Pin_0
#define  DS18B20_PROT  GPIOA
#define DS18B20_LOW     GPIO_ResetBits(DS18B20_PROT,DS18B20)
#define DS18B20_Height  GPIO_SetBits(DS18B20_PROT,DS18B20)



void DS18B20_UserConfig(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = DS18B20;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(DS18B20_PROT,&GPIO_InitStructure);   // a 0
}


void DS18B20_Output_Input(u8 cmd)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	if(cmd)
	{	//OUT
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = DS18B20;   
	}
	else
	{
	GPIO_InitStructure.GPIO_Pin = DS18B20;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		
	}
	GPIO_Init(DS18B20_PROT,&GPIO_InitStructure);  //初始化 a 0
}


u8 DS18B20_Stratup(void)
{
	u8 data=0;
	DS18B20_Output_Input(1);//1
	DS18B20_LOW;
	delay_us(480);
	DS18B20_Height;
	delay_us(30);
	DS18B20_Output_Input(0);//2
	data=GPIO_ReadInputDataBit(DS18B20_PROT,GPIO_Pin_0);
	delay_us(200);
	
	return data; //返回状态值
}


void DS18B20_Write_Byte(u8 data)
{
	u8 i=0;
	for( i=0;i<8;i+=1)
	{
		DS18B20_Output_Input(1);
		DS18B20_LOW;
		delay_us(2);
		(data&0x01)?DS18B20_Height:DS18B20_LOW;
		delay_us(45);
		DS18B20_Height;
		data >>= 1;	
	}
}


u8 ds18b20_read_byte(void)
{
 u8 data = 0;
	u8 i = 0;
	for( i = 0; i<8;i++)
	{
		data>>=1;
		DS18B20_Output_Input(1);
		DS18B20_LOW;
		delay_us(2);
		DS18B20_Height;
		DS18B20_Output_Input(0);
		if(GPIO_ReadInputDataBit(DS18B20_PROT,GPIO_Pin_0) == 1)
		{
			data|= 0x80;
		}
		delay_us(45);
	}
	return  data;
}



void *ds18b20_read_temp(int *data)
{
	u16 Temp1 =0;
	u8 LSB=0,MSB=0;
	DS18B20_Stratup();
	DS18B20_Write_Byte(0xcc);
	DS18B20_Write_Byte(0x44);
	//delay_ms(750);
	DS18B20_Stratup();
	DS18B20_Write_Byte(0xcc);
	DS18B20_Write_Byte(0xbe);
	LSB = ds18b20_read_byte();
	MSB = ds18b20_read_byte();
	Temp1 = MSB<<8|LSB;
	
	if((Temp1&0xf800) == 0xf800)
	{
		*data =(((~Temp1+0x01)*-0.0625)+0.5)*10.0;		
	}
	else
	{
		*data=((Temp1*0.0625)+0.5)*10.0;
	}
}




  /*
//复位DS18B20
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); 	//SET PG11 OUTPUT
    DS18B20_DQ_OUT=0; 	//拉低DQ
    delay_us(750);    	//拉低750us
    DS18B20_DQ_OUT=1; 	//DQ=1 
	delay_us(15);     	//15US
}
//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN();	//SET PG11 INPUT	 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}
//从DS18B20读取一个位
//返回值：1/0
u8 DS18B20_Read_Bit(void) 	 
{
    u8 data;
	DS18B20_IO_OUT();	//SET PG11 OUTPUT
    DS18B20_DQ_OUT=0; 
	delay_us(2);
    DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();	//SET PG11 INPUT
	delay_us(12);
	if(DS18B20_DQ_IN)data=1;
    else data=0;	 
    delay_us(50);           
    return data;
}
//从DS18B20读取一个字节
//返回值：读到的数据
u8 DS18B20_Read_Byte(void)     
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}
//写一个字节到DS18B20
//dat：要写入的字节
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
	DS18B20_IO_OUT();	//SET PG11 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT=0;	// Write 1
            delay_us(2);                            
            DS18B20_DQ_OUT=1;
            delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_OUT=0;	// Write 0
            delay_us(60);             
            DS18B20_DQ_OUT=1;
            delay_us(2);                          
        }
    }
}
//开始温度转换
void DS18B20_Start(void) 
{   						               
    DS18B20_Rst();	   
	DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	// skip rom
    DS18B20_Write_Byte(0x44);	// convert
} 

//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
u8 DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PORTG口时钟 
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				//PORTG.11 推挽输出
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOA,GPIO_Pin_1);    //输出1

	DS18B20_Rst();

	return DS18B20_Check();
}  
//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 
short DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
	short tem;
    DS18B20_Start ();  			// ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	// skip rom
    DS18B20_Write_Byte(0xbe);	// convert	    
    TL=DS18B20_Read_Byte(); 	// LSB   
    TH=DS18B20_Read_Byte(); 	// MSB  
	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;					//温度为负  
    }else temp=1;				//温度为正	  	  
    tem=TH; 					//获得高八位
    tem<<=8;    
    tem+=TL;					//获得底八位
    tem=(float)tem*0.625;		//转换     
	if(temp)return tem; 		//返回温度值
	else return -tem;    
}

*/

 
