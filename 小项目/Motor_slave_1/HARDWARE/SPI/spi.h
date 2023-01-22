#ifndef __SPI_H
#define __SPI_H
#include "sys.h"


//DIR
#define LEFT1  PBout(3) = 1, Pulse_output_1(625,800) // 2��ת1Ȧ ����ת�� 0.5�����
#define LEFT2  PBout(4) = 1, Pulse_output_2(625,800) // 2��ת1Ȧ ����ת�� 0.5�����

#define RIGHT1 PBout(3) = 0, Pulse_output_1(625,800) // 2��ת1Ȧ ����ת�� 0.5�����
#define RIGHT2 PBout(4) = 0, Pulse_output_1(625,800) // 2��ת1Ȧ ����ת�� 0.5�����

//EN
#define EN1 	PBout(0) = 1
#define EN2   PBout(1) = 1

#define STOP1 PBout(0) = 0
#define STOP2 PBout(1) = 0

#define  RING1  Pulse_output_1(625,3200); // 2��ת1Ȧ ����ת�� 0.5�����
#define  RING2  Pulse_output_1(625,3200); // 2��ת1Ȧ ����ת�� 0.5�����




//ָ���  �Զ���  �ӻ�����
#define Reply_Signal  254 //���ź�
#define LEFT 253  //��ת
#define RIGHT 252 //��ת
#define RING 249  //ԭ����ת




			  	    													  
void SPI1_Init(void);			 //��ʼ��SPI��
uint16_t SPI1_ReadWriteByte(uint16_t TxData);//SPI���߶�дһ���ֽ�
uint16_t Recv_Data_SPI(void);
void Control_Motor(uint16_t id);
		 
#endif



