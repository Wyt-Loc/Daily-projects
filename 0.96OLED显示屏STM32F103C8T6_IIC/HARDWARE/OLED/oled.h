#ifndef __OLED_H
#define __OLED_H 

#include "sys.h"
#include "stdlib.h"	

//-----------------OLED�˿ڶ���---------------- 





#define KEY_HANG1 GPIO_Pin_6
#define KEY_HANG2 GPIO_Pin_7
#define KEY_HANG3 GPIO_Pin_2
#define KEY_HANG4 GPIO_Pin_3


#define KEY1  GPIO_Pin_4 
#define KEY2  GPIO_Pin_5
#define KEY3  GPIO_Pin_6
#define KEY4  GPIO_Pin_7


#define KEY1_Input  GPIO_ReadInputDataBit(KEY_PROT,KEY1)
#define KEY2_Input  GPIO_ReadInputDataBit(KEY_PROT,KEY2)
#define KEY3_Input  GPIO_ReadInputDataBit(KEY_PROT,KEY3)
#define KEY4_Input  GPIO_ReadInputDataBit(KEY_PROT,KEY4)
 
 
#define KEY_PROT GPIOA








#define OLED_SCL_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_0)//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOA,GPIO_Pin_0)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_1)//DIN
#define OLED_SDA_Set() GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define OLED_RES_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_2)//RES
#define OLED_RES_Set() GPIO_SetBits(GPIOA,GPIO_Pin_2)


#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


extern  u8  flag;  

void KEY_Init(void);//IO��ʼ��
void KEY_config(u8 *key);
u8 Read_KeyValue(void);
void Key_Input( u8 *key1);




void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(u8 dat);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void OLED_Init(void);

#endif

