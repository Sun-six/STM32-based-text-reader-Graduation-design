#ifndef _RTC_H_
#define _RTC_H_

#include "stm32f10x.h"
#include "sys.h"
#include <stdio.h>
#include "led.h"

struct tm
{
  u8 tm_sec;	//��
  u8 tm_min;	//��
  u8 tm_hour;	//ʱ
  u8 tm_mday;	//��
  u8 tm_mon;	//��
  u32 tm_year;	//��
};
extern struct tm RTC_Time;
void RTC_NVIC_Config(uint32_t PreemptPriority, uint32_t SubPriority,u8 Alarm_flag);
void RTC_Config(void);


u32 NormTime_SecTime(struct tm *structTime);
void SecTime_NormTime(u32 time,struct tm *structTime);
void RTC_GetTime(struct tm *structTime);
void RTC_SetTime(struct tm *structTime);
void RTC_PutTime(void);

#endif
