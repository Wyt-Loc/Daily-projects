#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"
#include "stdio.h"

/*************************************
//���ʱ�䣺2019��11��18�� 12:03:32
//���ܽ��ܣ�ʵ��������λ���ɿ�״̬��ӳ����������
//����������
//��ϵ��ʽ��QQ��110662822
***************************************/

/*************************************
			A_X:X������ -90-90
			A_Y:Y������ 
			A_Z:Z������ -90-90
***************************************/
extern short T_X,T_Z,T_Y;	
extern short A_X,A_Z,A_Y;    
unsigned char ANO_BUFF[17];

/*************************************
�������ƣ�void Send_FLY()��
���ܽ��ܣ���������Э������λ��������λ����̬
0x88+FUN+LEN+DATA+SUM
***************************************/	
void Send_FLY()
{
	unsigned i,sum;
	READ_MPU6050();
	ANO_BUFF[0] = 0xAA;
	ANO_BUFF[1] = 0x88; //֡ͷ
	ANO_BUFF[2] = 0xA1; //FUN
	ANO_BUFF[3] = 0x0C; //LEN
	
	ANO_BUFF[4] = A_X>>8;
	ANO_BUFF[5] = A_X<<8;
	
	ANO_BUFF[6] = A_Z>>8;
	ANO_BUFF[7] = A_Z>>8;

	A_Y = A_Y * 10;
	ANO_BUFF[8] = A_Y>>8;
	ANO_BUFF[9] = A_Y>>8; 
	
	ANO_BUFF[10] = 0x00;
	ANO_BUFF[11] = 0x00;
	ANO_BUFF[12] = 0x00;
	ANO_BUFF[13] = 0x00;
	
	ANO_BUFF[14] = 0x00;
	
	ANO_BUFF[15] = 0x01;//DATA
	sum = 0;
	for(i=1;i<16;i++)
	{
		sum=ANO_BUFF[i]+sum;
	}
	ANO_BUFF[16] = sum;
	
	for(i=1;i<17;i++)
	{
		sendByte(USART1,ANO_BUFF[i]);
	}
	
}

/*************************************
�������ƣ�void send_all()��
���ܽ��ܣ�ͨ������1����λ���������ݣ�����۲⣩
***************************************/	
void send_all()
{
		unsigned char buff[6];
	
		READ_MPU6050();
	
		NDATA_printf(buff,A_X);
	  printf("TX:%s\r\n",buff);
	
		NDATA_printf(buff,A_Y);
	  printf("TY:%s\r\n",buff);
	
		NDATA_printf(buff,A_Z);
	  printf("TZ:%s\r\n",buff);
		printf("---------------------------------------------\r\n");
	
}




int main(void)
 {	 

	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	i2c_GPIO_Config();
	delay_ms(10);
	Init_MPU6050();
  	while(1) 
	{		 
		//send_all();
		Send_FLY();
		delay_ms(100);								//�޸�������Ըı䷢��Ƶ��
	} 
}
 
