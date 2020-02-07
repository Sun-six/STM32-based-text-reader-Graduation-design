#ifndef APP_CONTROL_H
#define APP_CONTROL_H

#include "app_gui.h"
#include "app_control.h"
#include "app_work.h"
#include "gt9147.h"
#include "delay.h"
typedef enum{
	CTOR_NOR				=(0),	/*没有触摸*/
	//选择栏
	CTOR_PICK1				=(1),
	CTOR_PICK2				=(2),
	CTOR_PICK3				=(3),
	CTOR_PICK4				=(4),
	CTOR_PICK5				=(5),
	CTOR_PICK6				=(6),
	CTOR_PICK7				=(7),
	CTOR_PICK8				=(8),
	CTOR_PICK9				=(9),
	CTOR_PICK10				=(10),
	CTOR_PICK11				=(11),
	CTOR_PICK12				=(12),
	CTOR_PICK13				=(13),
	CTOR_PICK14				=(14),
	//基本控制
	CTOR_RET				=(21),	/*返回上层*/
	CTOR_OK					=(22),	/*确定*/
	CTOR_CONFIG				=(23),	/*配置read*/
	CTOR_NEXT				=(24),	/*下一页*/
	CTOR_TOP				=(25),	/*上一页*/
	CTOR_FULL				=(26),	/*全屏*/
}TOUCH;


TOUCH Read_Ctrl(u8 page);
void Read_Ctrl_Clear(void);

#endif

