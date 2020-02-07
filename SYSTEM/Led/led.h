#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"
#include "sys.h"
/*
函数功能: LED初始化
硬件特性: 低电平亮
硬件接线:
        LED1-->PF7
        LED2-->PB5
        LED3-->PA8
        LED4-->PA1
*/

#define LED1	PFout(7)
#define LED2	PBout(5)
#define LED3	PAout(8)
#define LED4	PAout(1)

void LED_GPIO_Config(void);

#endif
