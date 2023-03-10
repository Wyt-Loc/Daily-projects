#include "spi.h"
#include "delay.h"
#include "stdio.h"

void SPI1_Init(void);			 //初始化SPI口
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler); //设置SPI速度   
uint16_t SPI1_ReadWriteByte(uint16_t TxData);//SPI总线读写一个字节



void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;//由于前两个函数都用到了SPI所以就在外面定义

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		//SPI_SCK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;		//SPI_MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;        //SPI_MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	 /*至于你们看到的有些程序这里写
	 SPI1_ReadWriteByte(0xff);
	 这里加不加都无所谓的。
	 */
}


//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)


void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
}

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint16_t SPI1_ReadWriteByte(uint16_t TxData)
{
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个byte  数据
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据
}





uint16_t reply = 0;
//得到从机的答复   答复信息为 666
//答复为1  则从机收到主机发送的数据开始动作
uint16_t Get_Reply(uint16_t data){
		reply = SPI1_ReadWriteByte(data);
		//printf("%d",reply);
		if(reply == Reply_Signal){
			printf("MASTEROK\r\n");
			return 1;
		}
		else
			return 0;
}




//车体状态控制
//启动  做一些变量的初始化工作 电机速度的设置
void  Start(void){
	while( Get_Reply(360) ){  //等待答复
	delay_ms(1000);  //延时1000ms  每1000ms重新发送一次
	}
}

//左转  直接转90°那种
void Left(uint16_t angle){
	while( Get_Reply(LEFT) ){  //等待答复
	delay_ms(1000);  //延时1000ms  每1000ms重新发送一次
	}
}

//右转  直接转90°那种
void Right(uint16_t angle){
	while( Get_Reply(RIGHT) ){  //等待答复
	delay_ms(1000);  //延时1000ms  每1000ms重新发送一次
	}
}

//原地旋转
void Ring(void){
	while( Get_Reply(RING) ){  //等待答复
	delay_ms(1000);  //延时1000ms  每1000ms重新发送一次
	}
}


//前进
void Go(void){
	Forward_rotation1;
	Forward_rotation2;
	Forward_rotation3;
	Forward_rotation4;
}

//后退
void Back(void){
	rollback1;
	rollback2;
	rollback3;
	rollback4;
}

void Stop(void){
	Stop1;
	Stop2;
	Stop3;
	Stop4;
}

uint16_t CRR1 = 100;
uint16_t CRR2 = 100;
uint16_t CRR3 = 100;
uint16_t CRR4 = 100;

//加速  参数 每次加速的幅度
void Hasten(uint16_t amplitude){
	TIM_SetCompare1(TIM4,CRR1 + amplitude);
	TIM_SetCompare2(TIM4,CRR2 + amplitude);
	//TIM_SetCompare3(TIM4,CRR3 + amplitude);  //蜂鸣器影响暂时去掉
	TIM_SetCompare4(TIM4,CRR4 + amplitude);
}

//减速  参数 每次减速的幅度
void Decelerate(uint16_t amplitude){
	TIM_SetCompare1(TIM4,CRR1 - amplitude);
	TIM_SetCompare2(TIM4,CRR2 - amplitude);
	//TIM_SetCompare3(TIM4,CRR3 + amplitude);  //蜂鸣器影响暂时去掉
	TIM_SetCompare4(TIM4,CRR4 - amplitude);
}





