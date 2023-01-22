#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
#include "stm32f10x_spi.h"


//ָ���  �Զ���
#define Reply_Signal  254 //���ź�
#define LEFT 253  //��ת
#define RIGHT 252 //��ת
#define RING 249  //ԭ����ת

//ͣ
#define Stop1  PAout(0) = 0,PAout(1) = 0
#define Stop2  PAout(12) = 0,PAout(13) = 0
#define Stop3  PAout(14) = 0,PAout(11) = 0
#define Stop4  PFout(5) = 0,PFout(6) = 0
//��ת
#define Forward_rotation1  PAout(0) = 0,PAout(1) = 1
#define Forward_rotation2  PAout(12) = 0,PAout(13) = 1
#define Forward_rotation3  PAout(14) = 0,PAout(11) = 1
#define Forward_rotation4  PFout(5) = 0,PFout(6) = 1
//��ת
#define rollback1  PAout(0) = 1,PAout(1) = 0
#define rollback2  PAout(12) = 1,PAout(13) = 0
#define rollback3  PAout(14) = 1,PAout(11) = 0
#define rollback4  PFout(5) = 1,PFout(6) = 0


void SPI1_Init(void);			 //��ʼ��SPI��
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler); //����SPI�ٶ�   
uint16_t SPI1_ReadWriteByte(uint16_t TxData);//SPI���߶�дһ���ֽ�
uint16_t Get_Reply(uint16_t data); //�õ��� ����������



void test(void);



void Start(void);
void Left(uint16_t angle);
void Right(uint16_t angle);
void Ring(void);
void Go(void);
void Back(void);
void Stop(void);
void Hasten(uint16_t amplitude);
void Decelerate(uint16_t amplitude);





		 
#endif

