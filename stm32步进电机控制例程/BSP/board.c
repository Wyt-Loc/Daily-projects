#include "board.h"

/**
	*	@brief		����ʱ�ӳ�ʼ��
	*	@param		��
	*	@retval		��
	*/
void clock_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��GPIOA�˿�ʱ��
}



/**
	*	@brief		gpio���ų�ʼ��
	*	@param		��
	*	@retval		��
	*/
void gpio_init(void)
{
/**************************************************************
	*	PA5��En���ջ���������Ļ�ϵ�Enѡ��ѡ��L��Hold��
	*	PA6��Stp
	*	PA7��Dir
***************************************************************/
 GPIO_InitTypeDef  GPIO_InitStructure;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 GPIO_ResetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
}


/**
	*	@brief		���س�ʼ��
	*	@param		��
	*	@retval		��
	*/

void board_init(void)
{
	clock_init();	/* ��������ʱ�� */
	gpio_init();	/* ��ʼ��GPIO */
}
