#include "spi.h"
#include "delay.h"
#include "stdio.h"

void SPI1_Init(void);			 //��ʼ��SPI��
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler); //����SPI�ٶ�   
uint16_t SPI1_ReadWriteByte(uint16_t TxData);//SPI���߶�дһ���ֽ�



void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;//����ǰ�����������õ���SPI���Ծ������涨��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		//SPI_SCK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;		//SPI_MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;        //SPI_MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	 /*�������ǿ�������Щ��������д
	 SPI1_ReadWriteByte(0xff);
	 ����Ӳ��Ӷ�����ν�ġ�
	 */
}


//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8��Ƶ   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16��Ƶ  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256��Ƶ (SPI 281.25K@sys 72M)


void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1
}

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
uint16_t SPI1_ReadWriteByte(uint16_t TxData)
{
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ��byte  ����
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����
}





uint16_t reply = 0;
//�õ��ӻ��Ĵ�   ����ϢΪ 666
//��Ϊ1  ��ӻ��յ��������͵����ݿ�ʼ����
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




//����״̬����
//����  ��һЩ�����ĳ�ʼ������ ����ٶȵ�����
void  Start(void){
	while( Get_Reply(360) ){  //�ȴ���
	delay_ms(1000);  //��ʱ1000ms  ÿ1000ms���·���һ��
	}
}

//��ת  ֱ��ת90������
void Left(uint16_t angle){
	while( Get_Reply(LEFT) ){  //�ȴ���
	delay_ms(1000);  //��ʱ1000ms  ÿ1000ms���·���һ��
	}
}

//��ת  ֱ��ת90������
void Right(uint16_t angle){
	while( Get_Reply(RIGHT) ){  //�ȴ���
	delay_ms(1000);  //��ʱ1000ms  ÿ1000ms���·���һ��
	}
}

//ԭ����ת
void Ring(void){
	while( Get_Reply(RING) ){  //�ȴ���
	delay_ms(1000);  //��ʱ1000ms  ÿ1000ms���·���һ��
	}
}


//ǰ��
void Go(void){
	Forward_rotation1;
	Forward_rotation2;
	Forward_rotation3;
	Forward_rotation4;
}

//����
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

//����  ���� ÿ�μ��ٵķ���
void Hasten(uint16_t amplitude){
	TIM_SetCompare1(TIM4,CRR1 + amplitude);
	TIM_SetCompare2(TIM4,CRR2 + amplitude);
	//TIM_SetCompare3(TIM4,CRR3 + amplitude);  //������Ӱ����ʱȥ��
	TIM_SetCompare4(TIM4,CRR4 + amplitude);
}

//����  ���� ÿ�μ��ٵķ���
void Decelerate(uint16_t amplitude){
	TIM_SetCompare1(TIM4,CRR1 - amplitude);
	TIM_SetCompare2(TIM4,CRR2 - amplitude);
	//TIM_SetCompare3(TIM4,CRR3 + amplitude);  //������Ӱ����ʱȥ��
	TIM_SetCompare4(TIM4,CRR4 - amplitude);
}





