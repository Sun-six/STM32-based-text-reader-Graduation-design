#include "led.h"
/*
函数功能: LED初始化
硬件特性: 低电平亮
硬件接线:
        LED1-->PF7
        LED2-->PB5
        LED3-->PA8
        LED4-->PA1
*/

void LED_GPIO_Config(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // 使能PB端口时钟  
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF, ENABLE); // 使能PF端口时钟  
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); // 使能PA端口时钟  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_1;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA端口  
	GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_1);	 // 关闭所有LED

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB端口 
	GPIO_SetBits(GPIOB, GPIO_Pin_5);	 // 关闭所有LED 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);  //初始化PF端口
	GPIO_SetBits(GPIOF, GPIO_Pin_7);	 // 关闭所有LED
}
