#include "app_control.h"

//定义	0：开始界面
//		1：选择见面
//		2：显示界面
//		3: 全屏显示界面

TOUCH Read_Ctrl(u8 page)
{
	TOUCH ret=CTOR_NOR;
	GT9147_Scan(0);
	if((tp_dev.sta)!=0)
	{
		switch(page)
		{
			case 0: ret=CTOR_OK;break;
			case 1: 
				if(tp_dev.y[0]>50 && tp_dev.y[0]<100) ret=CTOR_PICK1;
				else if(tp_dev.y[0]>100 && tp_dev.y[0]<150) ret=CTOR_PICK2;
				else if(tp_dev.y[0]>150 && tp_dev.y[0]<200) ret=CTOR_PICK3;
				else if(tp_dev.y[0]>200 && tp_dev.y[0]<250) ret=CTOR_PICK4;
				else if(tp_dev.y[0]>250 && tp_dev.y[0]<300) ret=CTOR_PICK5;
				else if(tp_dev.y[0]>300 && tp_dev.y[0]<350) ret=CTOR_PICK6;
				else if(tp_dev.y[0]>350 && tp_dev.y[0]<400) ret=CTOR_PICK7;
				else if(tp_dev.y[0]>400 && tp_dev.y[0]<450) ret=CTOR_PICK8;
				else if(tp_dev.y[0]>450 && tp_dev.y[0]<500) ret=CTOR_PICK9;
				else if(tp_dev.y[0]>500 && tp_dev.y[0]<550) ret=CTOR_PICK10;
				else if(tp_dev.y[0]>550 && tp_dev.y[0]<600) ret=CTOR_PICK11;
				else if(tp_dev.y[0]>600 && tp_dev.y[0]<650) ret=CTOR_PICK12;
				else if(tp_dev.y[0]>650 && tp_dev.y[0]<700) ret=CTOR_PICK13;
				else if(tp_dev.y[0]>700 && tp_dev.y[0]<750) ret=CTOR_PICK14;
				else if(tp_dev.y[0]>750 && tp_dev.y[0]<800 && tp_dev.x[0]<100) ret=CTOR_OK;
				else if(tp_dev.y[0]>750 && tp_dev.y[0]<800 && tp_dev.x[0]>380) ret=CTOR_NEXT;
				else ret=CTOR_NOR;
				break;
			case 2:
				if(tp_dev.y[0]>50  && tp_dev.y[0]<200 && tp_dev.x[0]>0   && tp_dev.x[0]<320) ret=CTOR_TOP;
				else if(tp_dev.y[0]>200 && tp_dev.y[0]<750 && tp_dev.x[0]>0   && tp_dev.x[0]<160) ret=CTOR_TOP;
				else if(tp_dev.y[0]>600 && tp_dev.y[0]<750 && tp_dev.x[0]>160 && tp_dev.x[0]<480) ret=CTOR_NEXT;
				else if(tp_dev.y[0]>50  && tp_dev.y[0]<600 && tp_dev.x[0]>320 && tp_dev.x[0]<480) ret=CTOR_NEXT;
				else if(tp_dev.y[0]>200 && tp_dev.y[0]<600 && tp_dev.x[0]>160 && tp_dev.x[0]<320) ret=CTOR_FULL;
				else if(tp_dev.y[0]>750 && tp_dev.y[0]<800 && tp_dev.x[0]<100 && (read_config.full_ctrl==0)) ret=CTOR_CONFIG;
				else if(tp_dev.y[0]>750 && tp_dev.y[0]<800 && tp_dev.x[0]>380 && (read_config.full_ctrl==0)) ret=CTOR_RET;
				else ret=CTOR_NOR;
				break;
			case 3:
				if(tp_dev.y[0]>0  && tp_dev.y[0]<200 && tp_dev.x[0]>0   && tp_dev.x[0]<320) ret=CTOR_TOP;
				else if(tp_dev.y[0]>200 && tp_dev.y[0]<800 && tp_dev.x[0]>0   && tp_dev.x[0]<160) ret=CTOR_TOP;
				else if(tp_dev.y[0]>600 && tp_dev.y[0]<800 && tp_dev.x[0]>160 && tp_dev.x[0]<480) ret=CTOR_NEXT;
				else if(tp_dev.y[0]>0   && tp_dev.y[0]<600 && tp_dev.x[0]>320 && tp_dev.x[0]<480) ret=CTOR_NEXT;
				else if(tp_dev.y[0]>200 && tp_dev.y[0]<600 && tp_dev.x[0]>160 && tp_dev.x[0]<320) ret=CTOR_FULL;
				else ret=CTOR_NOR;
				break;
			case 4: 
				if(tp_dev.y[0]>50 && tp_dev.y[0]<100) ret=CTOR_PICK1;
				else if(tp_dev.y[0]>100 && tp_dev.y[0]<150) ret=CTOR_PICK2;
				else if(tp_dev.y[0]>150 && tp_dev.y[0]<200) ret=CTOR_PICK3;
				else if(tp_dev.y[0]>200 && tp_dev.y[0]<250) ret=CTOR_PICK4;
				else if(tp_dev.y[0]>250 && tp_dev.y[0]<300) ret=CTOR_PICK5;
				else if(tp_dev.y[0]>300 && tp_dev.y[0]<350) ret=CTOR_PICK6;
				else if(tp_dev.y[0]>350 && tp_dev.y[0]<400) ret=CTOR_PICK7;
				else if(tp_dev.y[0]>400 && tp_dev.y[0]<450) ret=CTOR_PICK8;
				else if(tp_dev.y[0]>450 && tp_dev.y[0]<500) ret=CTOR_PICK9;
				else if(tp_dev.y[0]>500 && tp_dev.y[0]<550) ret=CTOR_PICK10;
				else if(tp_dev.y[0]>550 && tp_dev.y[0]<600) ret=CTOR_PICK11;
				else if(tp_dev.y[0]>600 && tp_dev.y[0]<650) ret=CTOR_PICK12;
				else if(tp_dev.y[0]>650 && tp_dev.y[0]<700) ret=CTOR_PICK13;
				else if(tp_dev.y[0]>700 && tp_dev.y[0]<750) ret=CTOR_PICK14;
				else if(tp_dev.y[0]>750 && tp_dev.y[0]<800 && tp_dev.x[0]<100) ret=CTOR_OK;
				else if(tp_dev.y[0]>750 && tp_dev.y[0]<800 && tp_dev.x[0]>380) ret=CTOR_RET;
				else ret=CTOR_NOR;
				break;
			case 5:
				if(tp_dev.y[0]>750 && tp_dev.y[0]<800 && tp_dev.x[0]>380) ret=CTOR_RET;
				else if(tp_dev.x[0]>20 && tp_dev.x[0]<460 && tp_dev.y[0]>380 && tp_dev.y[0]<400) ret=CTOR_OK;
				else ret=CTOR_NOR;
				break;
		}
	}
	return ret;
}

void Read_Ctrl_Clear(void)
{
	u8 i;
	for(i=0;i<20;i++)
	{
		GT9147_Scan(0);
		if(tp_dev.x[0]==0 && tp_dev.y[0]==0)
		{
			break;
		}
		DelayMs(5);
	}
}
