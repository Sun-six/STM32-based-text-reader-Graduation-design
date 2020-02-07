#ifndef APP_GUI_H
#define APP_GUI_H

#include "stm32f10x.h"
#include "lcd.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
#include "text.h"

void gui_0(void);
void gui_1(void);
void gui_2(u8 current,u8 all);
void gui_3(u8 current);
void gui_4(u8 current);
void gui_5(float schedule);
void gui_6(u8 current,u8 all);
void gui_7(u32 current,u32 all);

void Strat_Gui(void);
void Read_Gui(u8 full,u8 *path);
void Pick_Gui(u8 current,u8 all);
void Test_Gui(int j);

void LCD_DrawAngleLine(u32 x,u32 y,float du,u32 len,u32 w,u16 c);
void StartFont(u16 x,u16 y,u8 num,u16 c);
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color);
#endif
