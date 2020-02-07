#ifndef APP_WORK_H
#define APP_WORK_H

#include "app_gui.h"
#include "app_control.h"
#include "lcd.h"
#include "exti.h"
#include "sdio_sdcard.h" 
#include "w25qxx.h" 
#include "ff.h"
#include "fontupd.h"
#include "text.h"
#include "gt9147.h"
#include "delay.h"
#include "stdlib.h"

extern FATFS fs; // 文件系统盘符结构体
extern FIL file; // 文件系统文件描述结构体
extern DIR dir;  // 文件系统目录描述结构体

typedef struct{
	u8 font_size;
	u16 font_color;
	u16 backdrop_color;
	u8 full_ctrl;
	unsigned long current;
	unsigned long top_p;
	unsigned long top_n;
}READ_CONFIG;

extern READ_CONFIG read_config;


u8 Strat_Work(void);
u8 Read_Work(u8 * filepath);
u8* Pick_DirWork(const char* dirpath);


READ_CONFIG Show_TXT(READ_CONFIG read_config);
u32 Pre_read_next(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size);
void Pick_ConfigWork(void);
void Pick_ConfigFontSizeWork(void);
void Pick_ConfigFontColorWork(void);
void Pick_ConfigBackColorWork(void);
void Reading_progress(void);
#endif
