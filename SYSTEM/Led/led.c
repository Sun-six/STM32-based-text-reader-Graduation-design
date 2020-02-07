#include "led.h"
/*
��������: LED��ʼ��
Ӳ������: �͵�ƽ��
Ӳ������:
        LED1-->PF7
        LED2-->PB5
        LED3-->PA8
        LED4-->PA1
*/

void LED_GPIO_Config(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PB�˿�ʱ��  
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF, ENABLE); // ʹ��PF�˿�ʱ��  
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); // ʹ��PA�˿�ʱ��  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_1;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA�˿�  
	GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_1);	 // �ر�����LED

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PB�˿� 
	GPIO_SetBits(GPIOB, GPIO_Pin_5);	 // �ر�����LED 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);  //��ʼ��PF�˿�
	GPIO_SetBits(GPIOF, GPIO_Pin_7);	 // �ر�����LED
}
