#include <string.h>
#include <stdio.h>

#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "timer.h"
#include "rtc.h"
#include "sys.h"
#include "lcd.h"
#include "exti.h"
#include "sdio_sdcard.h" 
#include "w25qxx.h" 
#include "ff.h"
#include "fontupd.h"
#include "text.h"
#include "gt9147.h"
#include "app_main.h"


int main(void)
{
	SystemInit(); 

	Delay_Init();
	LED_GPIO_Config();
	BEEP_GPIO_Config();

	RTC_Config();
	RTC_NVIC_Config(1,1,0);
	TIMx_Current_Config(TIM2,1000,72);
	TIMx_Current_NVIC_Config(TIM2,1,1);
	USARTx_Config(USART1,115200);
	USARTx_NVIC_Config(USART1,0,1);

	app_main();
	while(1)
	{
	LED2=!LED2;
	//LCD_Clear(BLUE);
	DelayMs(500);
	//LCD_Clear(RED);
	DelayMs(500);
	}
}
