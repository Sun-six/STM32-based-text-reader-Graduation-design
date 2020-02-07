#include "app_gui.h"
#include "start_font.h"
void Strat_Gui(void)
{
	u8 i;
	LCD_Clear(BLACK);
	
	Show_cStr((lcddev.width-(48*5+24))/2,50,400,48,(u8 *)"基于STM32的",48,1,WHITE);
	Show_cStr((lcddev.width-(48*5))/2,50+48,400,48,(u8 *)"小说阅读器",48,1,WHITE);
	
	for(i=0;i<16;i++)
	{
		if(i<8)
		StartFont(lcddev.width/2-32-30,200+i*35,i,WHITE);
		else
		StartFont(lcddev.width/2+30,300+(i-8)*35,i,WHITE);
	}
	
	Show_cStr(100,600,400,32,(u8 *)"姓名：刘洋",32,1,WHITE);
	Show_cStr(100,600+35,400,32,(u8 *)"班级：1530403",32,1,WHITE);
	Show_cStr(100,600+35*2,400,32,(u8 *)"学号：201530040306",32,1,WHITE);
}

void Read_Gui(u8 full,u8 *path)
{	
	if(!full)
	{
		gui_0();		
		if(strlen((char *)(path+12))>14)
		{
			Show_cStr((lcddev.width-32*7)/2,(50-32)/2,32*6,32,(u8 *)(path+12),32,1,WHITE);
			Show_cStr((lcddev.width-32*7)/2+32*6,(50-32)/2,32*6,32,(u8 *)"...",32,1,WHITE);
		}
		else
		{
			Show_cStr((lcddev.width-32*(strlen((char *)(path+12))/2))/2,(50-32)/2,32*(strlen((char *)path))/2,32,(u8 *)(path+12),32,1,WHITE);
		}
		Show_cStr((100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"配置",32,1,WHITE);
		Show_cStr(385+(100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"返回",32,1,WHITE);
	}
}

void Pick_Gui(u8 current,u8 all)
{
	LCD_Clear(WHITE);
	gui_0();
	gui_1();
	gui_2(current,all);
	gui_3((current-1)%14);
}
void Test_Gui(int j)
{
	LCD_Clear(BLACK);
	if(j)
	Show_cStr(lcddev.width/2-(32*7)/2,lcddev.height/2-32/2,32*7,32,(u8*)"这个功能OJBK啦",32,1,WHITE);
	else
	Show_cStr(lcddev.width/2-(32*7)/2,lcddev.height/2-32/2,32*7,32,(u8*)"这个功能不OJBK",32,1,WHITE);	
}
////////////////////////////////////////////////////////
//界面基本元素接口
#if 1
//最底层蓝白界面
void gui_0(void)
{
	//蓝色顶栏
	LCD_Fill(0,0,480,50,DARKBLUE);
	//蓝色底栏
	LCD_Fill(0,750,480,800,DARKBLUE);
	//分割线
	LCD_Fill(100,750,105,800,GRAY);
	LCD_Fill(380,750,385,800,GRAY);
}
//罗列框
void gui_1(void)
{
	u8 i;
	for(i=0;i<14;i++)
	{
		LCD_DrawRectangle(0,50+i*50,460,100+i*50);
	}
}
//进度条
void gui_2(u8 current,u8 all)
{
	float a=all/1.0;
	float c=current/1.0;
	LCD_DrawLine(460,50,470,750);
	LCD_Fill(461,50,480,750,WHITE);
	LCD_Fill(461,50+(700/a*(c-1)),480,50+(700/a)+(700/a*(c-1)),BLUE);
}
//选中框
void gui_3(u8 current)
{
	LCD_Fill(1,50+current*50+1,459,100+current*50-1,GRAYBLUE);
}
//还原框
void gui_4(u8 current)
{
	LCD_Fill(1,50+current*50+1,459,100+current*50-1,WHITE);
}
//顶进度比例
void gui_5(float schedule)
{
	char num[10];
	sprintf(num,"%2.2f%%",schedule);
	LCD_ShowString(10,10,strlen(num)*24/2,24,24,(u8 *)num);
}
//选择进度比例框
void gui_6(u8 current,u8 all)
{
	char num[10];
	sprintf(num,"%d/%d",current,all);
	LCD_ShowString(10,10,strlen(num)*24,24,24,(u8 *)num);
}
//阅读进度比例框
void gui_7(u32 current,u32 all)
{
	double a=current/1.0/all*439;
	//进度框
	LCD_DrawRectangle(20,380,460,400);
	//进度条
	LCD_Fill(21,381,459,399,WHITE);
	LCD_Fill(21,380,a+21,399,BLUE);
}
#endif
////////////////////////////////////////////////////////
//基本图形API接口
#if 1
/*
函数功能：任意角度画直线 
参    数：
	x,y:坐标
	du :度数
	len:半径
	w  :线段的长度
	c  :颜色值 0或者1
例如:LCD_DrawAngleLine(60,30,45,20,20,1);//角度画线
*/
void LCD_DrawAngleLine(u32 x,u32 y,float du,u32 len,u32 w,u16 c)
{
	int i;
	int x0,y0;
	float k=du*(3.1415926535/180);	
	for(i=len-w;i<len;i++)
	{
		x0=cos(k)*i;
		y0=sin(k)*i;
		LCD_Fast_DrawPoint(x+x0,y+y0,c);
	}	
}
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
void StartFont(u16 x,u16 y,u8 num,u16 c)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=128;
	for(t=0;t<csize;t++)
	{   
		temp=start_font[num][t];
		for(t1=0;t1<8;t1++)
		{		
			//DelayMs(30);
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,c);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//超区域了
			if((y-y0)==32)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
} 
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}
//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
} 
#endif
