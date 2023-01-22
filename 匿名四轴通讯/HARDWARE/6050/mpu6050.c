#include "mpu6050.h"


unsigned char TX_DATA[6];  	 //显示据缓存区
unsigned char BUF[10];       //接收数据缓存区
unsigned char ACC_BUF[10];   //加速度接收数据缓存区
char  test=0; 				       //IIC用到
short T_X,T_Y,T_Z,T_T;		   //X,Y,Z轴，温度
short A_X,A_Y,A_Z;           //X Y Z轴加速度


	
/**************************************
**************重要*********************
************如果想修改引脚*************
****仅需要对下面的内容进行修改即可*****
***************************************/	



#define GPIO_PORT_I2C	GPIOA			/* GPIO端口 */
#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define I2C_SCL_PIN		GPIO_Pin_0			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN		GPIO_Pin_1			/* 连接到SDA数据线的GPIO */

#define I2C_SCL_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 1 */
#define I2C_SCL_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 0 */
	
#define I2C_SDA_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 1 */
#define I2C_SDA_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 0 */
	
#define I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)	/* 读SDA口线状态 */


void NDATA_printf(u8 *s,short temp_data)
{
	if(temp_data<0){
	temp_data=-temp_data;
    *s='-';
	}
	else *s=' ';
		*++s =temp_data/10000+0x30;
    temp_data=temp_data%10000;    //取余运算
	
	  *++s =temp_data/1000+0x30;
    temp_data=temp_data%1000;     //取余运算
	
    *++s =temp_data/100+0x30;
    temp_data=temp_data%100;      //取余运算
	
    *++s =temp_data/10+0x30;
    temp_data=temp_data%10;       //取余运算
	
    *++s =temp_data+0x30; 	
}

void i2c_GPIO_Config(void)
{	 
	  GPIO_InitTypeDef GPIO_InitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);	
} 

bool I2C_Start(void)
{
	I2C_SDA_1();
	I2C_SCL_1();
	delay_us(3);
	if(!I2C_SDA_READ())return FALSE;	//SDA线为低电平则总线忙,退出
	I2C_SDA_0();
	delay_us(3);
	if(I2C_SDA_READ()) return FALSE;	//SDA线为高电平则总线出错,退出
	I2C_SDA_0();
	delay_us(3);
	return TRUE;
}

/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Stop(void)
{
	I2C_SCL_0();
	delay_us(3);
	I2C_SDA_0();
	delay_us(3);
	I2C_SCL_1();
	delay_us(3);
	I2C_SDA_1();
	delay_us(3);
} 
/*******************************************************************************
* Function Name  : I2C_Ack
* Description    : Master Send Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Ack(void)
{	
	I2C_SCL_0();
	delay_us(3);
	I2C_SDA_0();
	delay_us(3);
	I2C_SCL_1();
	delay_us(3);
	I2C_SCL_0();
	delay_us(3);
}   
/*******************************************************************************
* Function Name  : I2C_NoAck
* Description    : Master Send No Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_NoAck(void)
{	
	I2C_SCL_0();
	delay_us(3);
	I2C_SDA_1();
	delay_us(3);
	I2C_SCL_1();
	delay_us(3);
	I2C_SCL_0();
	delay_us(3);
} 
/*******************************************************************************
* Function Name  : I2C_WaitAck
* Description    : Master Reserive Slave Acknowledge Single
* Input          : None
* Output         : None
* Return         : Wheather	 Reserive Slave Acknowledge Single
****************************************************************************** */
bool I2C_WaitAck(void) 	 //返回为:=1有ACK,=0无ACK
{
	I2C_SCL_0();
	delay_us(3);
	I2C_SDA_1();			
	delay_us(3);
	I2C_SCL_1();
	delay_us(3);
	if(I2C_SDA_READ())
	{
      I2C_SCL_0();
			delay_us(3);
      return FALSE;
	}
	I2C_SCL_0();
	delay_us(3);
	return TRUE;
}
/*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : Master Send a Byte to Slave
* Input          : Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_SendByte(u8 SendByte) //数据从高位到低位//
{
    u8 i=8;
    while(i--)
    {
        I2C_SCL_0();
				delay_us(3);
      if(SendByte&0x80)
        I2C_SDA_1();  
      else 
        I2C_SDA_0();   
        SendByte<<=1;
				delay_us(3);
		I2C_SCL_1();
				delay_us(3);
    }
    I2C_SCL_0();
}  
/*******************************************************************************
* Function Name  : I2C_RadeByte
* Description    : Master Reserive a Byte From Slave
* Input          : None
* Output         : None
* Return         : Date From Slave 
****************************************************************************** */
unsigned char I2C_RadeByte(void)  //数据从高位到低位//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    I2C_SDA_1();				
    while(i--)
    {
      ReceiveByte<<=1;      
      I2C_SCL_0();
			delay_us(3);
			I2C_SCL_1();
			delay_us(3);
      if(I2C_SDA_READ())
      {
        ReceiveByte|=0x01;
      }
    }
    I2C_SCL_0();
    return ReceiveByte;
} 
//ZRX          
//单字节写入*******************************************

bool Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)		     //void
{
  	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(REG_Address );   //设置低起始地址      
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
		delay_ms(5);
    return TRUE;
}

//单字节读取*****************************************
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   
	unsigned char REG_data;     	
	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop();test=1; return FALSE;}
    I2C_SendByte((u8) REG_Address);   //设置低起始地址      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

		REG_data= I2C_RadeByte();
    I2C_NoAck();
    I2C_Stop();
    //return TRUE;
	return REG_data;

}


//初始化MPU6050，根据需要请参考pdf进行修改************************
void Init_MPU6050(void)
{

  Single_Write(MPU6050_Addr,PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_Write(MPU6050_Addr,SMPLRT_DIV, 0x07);  //125Hz
	Single_Write(MPU6050_Addr,CONFIG, 0x04);			//21Hz滤波
	Single_Write(MPU6050_Addr,GYRO_CONFIG, 0x08); //500'/s  65.5LSB/g
	Single_Write(MPU6050_Addr,ACCEL_CONFIG, 0x08);//+-4g    8192LSB/G 
}
	
//******读取MPU6050数据****************************************
void READ_MPU6050(void)
{
   BUF[0]=Single_Read(MPU6050_Addr,GYRO_XOUT_L); 
   BUF[1]=Single_Read(MPU6050_Addr,GYRO_XOUT_H);
   T_X=	(BUF[1]<<8)|BUF[0];
   T_X/=65.5; 						   //读取计算X轴数据


   BUF[4]=Single_Read(MPU6050_Addr,GYRO_ZOUT_L);
   BUF[5]=Single_Read(MPU6050_Addr,GYRO_ZOUT_H);
   T_Z=	(BUF[5]<<8)|BUF[4];
   T_Z/=65.5; 					       //读取计算Z轴数据

   BUF[2]=Single_Read(MPU6050_Addr,GYRO_YOUT_L);
   BUF[3]=Single_Read(MPU6050_Addr,GYRO_YOUT_H);
   T_Y=	(BUF[3]<<8)|BUF[2];
   T_Y/=65.5; 						   //读取计算Y轴数据
	
   ACC_BUF[0]=Single_Read(MPU6050_Addr,ACCEL_XOUT_H); 
   ACC_BUF[1]=Single_Read(MPU6050_Addr,ACCEL_XOUT_L);
   A_X=	(ACC_BUF[0]<<8)+ACC_BUF[1];
//   A_X/=8192.0; 						   //读取计算X轴数据

   ACC_BUF[3]=Single_Read(MPU6050_Addr,ACCEL_YOUT_L);
   A_Y=	(ACC_BUF[2]<<8)+ACC_BUF[3];
//   A_Y/=8192.0; 						   //读取计算Y轴数据

   ACC_BUF[4]=Single_Read(MPU6050_Addr,ACCEL_ZOUT_H);
   ACC_BUF[5]=Single_Read(MPU6050_Addr,ACCEL_ZOUT_L);
   A_Z=	(ACC_BUF[4]<<8)+ACC_BUF[5];
//   A_Z/=8192.0; 					     //读取计算Z轴数据
}