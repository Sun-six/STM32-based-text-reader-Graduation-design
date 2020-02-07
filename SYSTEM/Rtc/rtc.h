#ifndef _RTC_H_
#define _RTC_H_

#include "stm32f10x.h"
#include "sys.h"
#include <stdio.h>
#include "led.h"

struct tm
{
  u8 tm_sec;	//秒
  u8 tm_min;	//分
  u8 tm_hour;	//时
  u8 tm_mday;	//日
  u8 tm_mon;	//月
  u32 tm_year;	//年
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
