#include "app_main.h"

int app_main(void)
{
	int ret;
	u8 *path;
	Strat_Work();
	do
	{
		path=Pick_DirWork("0:/READ_TXT");
		printf("%s\n",path);
		if(path == 0)
		{
			return 0;
		}
		ret=Read_Work(path);
	}while(ret==2);
	//Test_Gui(ret);
	return ret;
}
