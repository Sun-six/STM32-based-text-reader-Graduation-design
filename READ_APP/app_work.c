#include "app_work.h"

FATFS fs; // �ļ�ϵͳ�̷��ṹ��
FIL file; // �ļ�ϵͳ�ļ������ṹ��
DIR dir;  // �ļ�ϵͳĿ¼�����ṹ��

u8 Strat_Work(void)
{
	u16 i;
	u8 key=1;
	//��ʼ��LCD	
	LCD_Init();
	
	//�����ļ�ϵͳ
	f_mount(&fs,"0:",1);
	
	//W25Q128��ʼ��
	W25QXX_Init();
	
	//�ֿ���
	while(font_init()) 			//����ֿ�
	{    
		LCD_Clear(WHITE);		   	//����
 		POINT_COLOR=RED;			//��������Ϊ��ɫ	   	   	  
		LCD_ShowString(30,50,200,16,16,(u8 *)"WarShip STM32");
		while(SD_Init())			//���SD��
		{
			LCD_ShowString(30,70,200,16,16,(u8 *)"SD Card Failed!");
			DelayMs(200);
			LCD_Fill(30,70,200+30,70+16,WHITE);
			DelayMs(200);		    
		}								 						    
		LCD_ShowString(30,70,200,16,16,(u8 *)"SD Card OK");
		LCD_ShowString(30,90,200,16,16,(u8 *)"Font Updating...");
		key=update_font(20,110,16,(u8 *)"0:");//�����ֿ�
		while(key)//����ʧ��		
		{			
			printf("������� %d\n",key);
			LCD_ShowString(30,110,200,16,16,(u8 *)"Font Update Failed!");
			DelayMs(200);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			DelayMs(200);		       
		} 		  
		LCD_ShowString(30,110,200,16,16,(u8 *)"Font Update Success!   ");
		DelayMs(1500);	
		LCD_Clear(WHITE);//����	  
	}
	
	//��ʼ��������
	GT9147_Init();
	//��ʾ��������
	Strat_Gui();	
	//�ȴ�������������߹�5��
	for(i=0;i<1000;i++)
	{
		if(Read_Ctrl(0)==CTOR_OK)
		{
			return 1;
		}
		DelayMs(5);
	}
	return 0;
}



u8* Pick_DirWork(const char * dirpath)
{
	TOUCH ctrl_ret=CTOR_NOR;
	FILINFO fno;
	FRESULT res;
	u8 index=1,index_temp=1;
	u16 filecount=0,i;
	u8 *pathbuff;
	u8 *pathret;
	fno.lfsize=_MAX_LFN;
	fno.lfname=malloc(_MAX_LFN);
	//��readĿ¼
	res=f_opendir(&dir,(const TCHAR*)dirpath);
	if(res != FR_OK) 
	{
		printf("Ŀ¼%s��ʧ�ܣ�����\t������%d\n",dirpath,res);
		return 0;
	}
	printf("�ɹ���С˵���Ŀ¼��%s\n",dirpath);
	//��ȡĿ¼���ļ�����
	while(1)
	{
		res=f_readdir(&dir, &fno);
		if(res != FR_OK) 
		{
			printf("Ŀ¼%s��Ϣ��ȡʧ��!!!\t������%d\n",dirpath,res);
			break;
		}
		if(fno.fname[0] == 0)
		{
			printf("Ŀ¼%s�ļ�������ȡ���\tfilecount = %d\n",dirpath,filecount);
			break;
		}
		filecount++;
	}
	f_closedir(&dir);
	//����Ŀ¼��Ϣ
	pathbuff=malloc(_MAX_LFN*filecount);
	memset(pathbuff,0,_MAX_LFN*filecount);
	res=f_opendir(&dir,(const TCHAR*)dirpath);
	if(res != FR_OK) 
	{
		printf("Ŀ¼%s��ʧ�ܣ�����\t������%d\n",dirpath,res);
		free(fno.lfname);
		return 0;
	}
	for(i=0;i<filecount;i++)
	{
		res=f_readdir(&dir, &fno);
		if(res != FR_OK) 
		{
			printf("Ŀ¼%s��Ϣ��ȡʧ��!!!\t������%d\n",dirpath,res);
			break; //����ʧ�� �� ����β�����˳�
		}
		if(fno.fname[0] == 0)
		{
			printf("Ŀ¼%s�ļ�������ȡ���\tfilecount = %d\n",dirpath,filecount);
			break;
		}
		if(strlen(fno.lfname)>0)
		{
			strcpy((char *)(pathbuff+i*_MAX_LFN),fno.lfname);
		}
		else
		{
			strcpy((char *)(pathbuff+i*_MAX_LFN),fno.fname);
		}
	}
	f_closedir(&dir);
	free(fno.lfname);
	//��ʾpick����
	Pick_Gui(index_temp,filecount);
	gui_6(index_temp,filecount);
	Show_cStr((lcddev.width-32*7)/2,(50-32)/2,32*7,32,(u8 *)"<<С˵�Ķ���>>",32,1,WHITE);
	Show_cStr((100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"��",32,1,WHITE);
	Show_cStr(385+(100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"��ҳ",32,1,WHITE);
	//��ӡ�ļ���Ϣ
	for(i=0;i<filecount;i++)
	{
		Show_cStr(10,50+50*i+(50-32)/2,32*12,32,pathbuff+i*_MAX_LFN,32,1,BLACK);
		if(strlen((char *)(pathbuff+i*_MAX_LFN))>=24)
		{
			Show_cStr(13*32,50+50*i+(50-32)/2,16*3,32,(u8 *)"...",32,1,BLACK);
		}
	}
	DelayMs(1000);
	//�ȴ�����ѡ��
	while(1)
	{	
		DelayMs(50);
		Read_Ctrl_Clear();
		ctrl_ret=Read_Ctrl(1);
		if(ctrl_ret!=CTOR_NOR)
		{		
			if(ctrl_ret>=CTOR_PICK1 && (ctrl_ret<=CTOR_PICK14 && ctrl_ret<=filecount)) 
			{
				printf("pick%d\n",ctrl_ret);
				gui_4((index_temp-1)%14);
				Show_cStr(10,50+50*((index_temp-1)%14)+(50-32)/2,32*12,32,pathbuff+((index_temp-1)%14)*_MAX_LFN,32,1,BLACK);
				if(strlen((char *)(pathbuff+((index_temp-1)%14)*_MAX_LFN))>=24)
				{
					Show_cStr(13*32,50+50*((index_temp-1)%14)+(50-32)/2,16*3,32,(u8 *)"...",32,1,BLACK);
				}
				index_temp=ctrl_ret;
				gui_2(index_temp,filecount);
				gui_3((index_temp-1)%14);
				gui_6(index_temp,filecount);
				Show_cStr(10,50+50*((index_temp-1)%14)+(50-32)/2,32*12,32,pathbuff+((index_temp-1)%14)*_MAX_LFN,32,1,BLACK);
				if(strlen((char *)(pathbuff+((index_temp-1)%14)*_MAX_LFN))>=24)
				{
					Show_cStr(13*32,50+50*((index_temp-1)%14)+(50-32)/2,16*3,32,(u8 *)"...",32,1,BLACK);
				}
				if(index != index_temp)
				{
					index=index_temp;
				}
				else
				{
					break;
				}
			}
			else if(ctrl_ret==CTOR_OK) 
			{
				printf("CTOR_OK\n");
				index=index_temp;
				break;
			}
			else if(ctrl_ret==CTOR_NEXT)
			{
				printf("CTOR_NEXT\n");
			}
		}
	}
	pathret=malloc(2*_MAX_LFN);
	memset(pathret,0,2*_MAX_LFN);
	sprintf((char *)pathret,"%s/",dirpath);
	strcat((char *)pathret,(const char *)pathbuff+(index-1)*_MAX_LFN);
	free(pathbuff);
	return pathret;
}

READ_CONFIG read_config;//С˵�Ķ������ýṹ��

u8 Read_Work(u8 * filepath)
{
	TOUCH ctrl_ret=CTOR_NOR;
	FIL configfile; // �ļ�ϵͳ�ļ������ṹ��
	FRESULT res;
	u32 rlen;
	//�����ļ����Ʊ����ַ
	u8 path[_MAX_LFN*2];
	u8 configpath[_MAX_LFN];
	strcpy((char *)path,(const char *)filepath);
	strcpy((char *)configpath,(const char *)(path+12));
	strcat((char *)configpath,(const char *)".config");
	printf("%s\n",path);
	printf("%s\n",configpath);
	//�ͷ�MALLOC����Ŀռ�
	free(filepath);
	//��txt�ļ�
	res=f_open(&file,(const TCHAR*)path,FA_OPEN_EXISTING | FA_READ);
	if(res != FR_OK)
	{
		printf("�ļ�%s��ʧ��\t������%d\n",path,res);
		return 0;
	}
	//С˵�Ķ�����ʼ������
	//�������ļ�
	res=f_open(&configfile,(const TCHAR*)configpath,FA_OPEN_EXISTING | FA_READ);
	if(res != FR_OK)
	{
		res=f_open(&configfile,(const TCHAR*)configpath,FA_CREATE_ALWAYS | FA_READ);
		if(res != FR_OK)
		{
			printf("�ļ�%s��ʧ��\t������%d\n",path,res);
			return 0;
		}		
		read_config.font_size=24;
		read_config.font_color=BLACK;
		read_config.full_ctrl=0;
		read_config.backdrop_color=WHITE;
		read_config.current=0;
	}
	else
	{
		res=f_read(&configfile,&read_config,sizeof(READ_CONFIG),&rlen);
		if(res != FR_OK && rlen<sizeof(READ_CONFIG))
		{
			printf("�ļ�%s�Ķ����ö�ȡʧ��\t������%d\n",path,res);
			read_config.font_size=24;
			read_config.font_color=BLACK;
			read_config.full_ctrl=0;
			read_config.backdrop_color=WHITE;
			read_config.current=0;
		}
	}
	f_close(&configfile);
	//�Ķ�����������
	LCD_Init();
	//��ʾread����
	LCD_Clear(read_config.backdrop_color);
	Read_Gui(read_config.full_ctrl,path);
	read_config=Show_TXT(read_config);
	//��ʾTXT�ļ�����
	while(1)
	{
		DelayMs(60);
		Read_Ctrl_Clear();
		ctrl_ret=Read_Ctrl(read_config.full_ctrl?3:2);
		if(ctrl_ret!=CTOR_NOR)
		{
			if(ctrl_ret==CTOR_FULL)//ȫ���Ķ�
			{
				read_config.full_ctrl=!read_config.full_ctrl;
				printf("CTOR_FULL\n");
				if(read_config.full_ctrl)
				{
					LCD_Init();
					read_config=Show_TXT(read_config);
					printf("����Ϊȫ���Ķ�ģʽ\n");
				}
				else
				{
					Read_Gui(read_config.full_ctrl,path);
					read_config=Show_TXT(read_config);
					printf("����Ϊ��׼�Ķ�ģʽ\n");
				}
			}			
			if(ctrl_ret==CTOR_CONFIG && read_config.full_ctrl==0)//�Ķ�����
			{
				printf("CTOR_CONFIG\n");
				Pick_ConfigWork();
				LCD_Init();
	
				Read_Gui(read_config.full_ctrl,path);
				read_config=Show_TXT(read_config);
			}			
			if(ctrl_ret==CTOR_TOP)//��һҳ
			{
				read_config.current=read_config.top_p;
				read_config=Show_TXT(read_config);
				printf("CTOR_TOP\n");
			}			
			if(ctrl_ret==CTOR_NEXT)//��һҳ
			{
				read_config.current=read_config.top_n;
				read_config=Show_TXT(read_config);
				printf("CTOR_NEXT\n");
			}
			if(ctrl_ret==CTOR_RET && read_config.full_ctrl==0)//����
			{
				printf("CTOR_RET\n");
				res=f_open(&configfile,(const TCHAR*)configpath,FA_OPEN_EXISTING | FA_WRITE);
				f_write(&configfile,&read_config,sizeof(READ_CONFIG),&rlen);
				f_close(&file);
				f_close(&configfile);
				return 2;
			}
		}
	}
}
READ_CONFIG Show_TXT(READ_CONFIG read_config)
{
	u8 *read_buff;
	u32 read_len;
	u16 x=0,y;
	u32 rlen,fptr,count,i=0;
	u32 show_height;
	FRESULT res;
	if(read_config.full_ctrl)
	{
		y=0;
		show_height=lcddev.height;
		read_len=(lcddev.width/read_config.font_size)*(lcddev.height/read_config.font_size)*2;
		LCD_Fill(x,y,lcddev.width,lcddev.height,read_config.backdrop_color);
	}
	else
	{
		y=50;
		show_height=lcddev.height-100;
		read_len=(lcddev.width/read_config.font_size)*((lcddev.height-100)/read_config.font_size)*2;
		LCD_Fill(x,y,lcddev.width,lcddev.height-50,read_config.backdrop_color);
		gui_5(read_config.current/1.0/file.fsize*100);
	}
	read_buff=malloc(read_len);
	memset(read_buff,0,read_len);
	f_lseek(&file,read_config.current);
	res=f_read(&file,read_buff,read_len,&rlen);
	if(res != FR_OK)
	{
		printf("�ļ���ȡʧ��\t������%d\n",res);
		return read_config;
	}
	if(rlen<read_len)
	{
		printf("�ļ���ȡ���\n");
	}
	Show_cStr(x,y,lcddev.width,show_height,read_buff,read_config.font_size,1,read_config.font_color);
	//��ȡ��һҳ�׵�ַ
	read_config.top_n=Pre_read_next(x,y,lcddev.width,show_height,read_buff,read_config.font_size)+read_config.current;
	printf("��ҳ��ַ %ld\n",read_config.top_n);
	printf("��ǰ��ַ %ld\n",read_config.current);
	
	//��ȡ��һҳ�׵�ַ
	if(read_config.current!=0)
	{
		memset(read_buff,0,read_len);
		//������Ѱ��ʼ��ַ
		if(read_config.current>=read_len)
		{
			fptr=read_config.current-read_len;
		}
		else
		{
			fptr=0;
		}
		printf("fptr = %d\n",fptr);
		//���ļ�
		f_lseek(&file,fptr);
		res=f_read(&file,read_buff,read_len,&rlen);	
		if(res != FR_OK)
		{
			printf("�ļ���ȡʧ��\t������%d\n",res);
			return read_config;
		}
		if(rlen<read_len)
		{
			printf("�ļ���ȡ���\n");
		}
		if(fptr!=0)//��֤�׵�ַ���ں����м�
		{
			if(*read_buff<0x80)
			{
				i++;
			}
		}
		do
		{
			count=Pre_read_next(x,y,lcddev.width,show_height,read_buff+i,read_config.font_size);
			if(count>=read_config.current)
			{
				read_config.top_p=0;
				break;
			}
			if(count+fptr+i >= read_config.current)
			{
				read_config.top_p=fptr+i;			
				if(*(read_buff+i)<0x80)
				{
					read_config.top_p++;
				}
				break;
			}
			if(*(read_buff+i)>0x80) 
			{
				i+=2;
			}
			else 
			{
				i++;
			}
		}while(1);
	}
	else read_config.top_p=read_config.current;
	printf("��ҳ��ַ %ld\n",read_config.top_p);
	free(read_buff);
	return read_config;
}
u32 Pre_read_next(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size)
{	
	u16 x0=x;
	u16 y0=y;							  	  
    u8 bHz=0;     //�ַ���������  
	u32 count;
	u8 *str_temp=str;
    while(*str!=0)//����δ����
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//���� 
	        else              //�ַ�
	        {      
                if(x>(x0+width-size/2))//����
				{				   
					y+=size;
					x=x0;	   
				}							    
		        if(y>(y0+height-size))break;//Խ�緵��      
		        if(*str==13)//���з���
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
				str++; 
		        x+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
	        }
        }else//���� 
        {     
            bHz=0;//�к��ֿ�    
            if(x>(x0+width-size))//����
			{	    
				y+=size;
				x=x0;		  
			}
	        if(y>(y0+height-size))break;//Խ�緵��  						
	        str+=2; 
	        x+=size;//��һ������ƫ��	    
        }						 
    }
	count=str-str_temp;
	return count;
}
void Pick_ConfigWork(void)
{
	TOUCH ctrl_ret=CTOR_NOR;
	u8 index=1,index_temp;
	u8 i;
	u8 *config_buff[4]={(u8 *)"�����С",(u8 *)"������ɫ",(u8 *)"������ɫ",(u8 *)"���ȵ���"};
	index_temp=index;
	//��ʾ���ý���
	Pick_Gui(index_temp,4);
	gui_6(index_temp,4);
	Show_cStr((lcddev.width-32*7)/2,(50-32)/2,32*7,32,(u8 *)"<<�Ķ�������>>",32,1,WHITE);
	Show_cStr((100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"��",32,1,WHITE);
	Show_cStr(385+(100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"����",32,1,WHITE);
	//��ӡ���ý�����Ϣ
	for(i=0;i<4;i++)
	{
		Show_cStr(10,50+50*i+(50-32)/2,32*12,32,config_buff[i],32,1,BLACK);
	}
	//�ȴ�����ѡ��
	while(1)
	{	
		DelayMs(50);
		Read_Ctrl_Clear();
		ctrl_ret=Read_Ctrl(4);
		if(ctrl_ret!=CTOR_NOR)
		{		
			printf("ctrl_red = %d\n",ctrl_ret);
			if(ctrl_ret>=CTOR_PICK1 && (ctrl_ret<=CTOR_PICK14 && ctrl_ret<=4)) 
			{
				printf("pick%d\n",ctrl_ret);
				gui_4((index_temp-1)%14);
				Show_cStr(10,50+50*((index_temp-1)%14)+(50-32)/2,32*12,32,config_buff[(index_temp-1)%14],32,1,BLACK);
				index_temp=ctrl_ret;
				gui_2(index_temp,4);
				gui_3((index_temp-1)%14);
				gui_6(index_temp,4);
				Show_cStr(10,50+50*((index_temp-1)%14)+(50-32)/2,32*12,32,config_buff[(index_temp-1)%14],32,1,BLACK);			
				if(index != index_temp)
				{
					index=index_temp;
				}
				else
				{
					//���ý�����ת
					switch (index){
						case 1:Pick_ConfigFontSizeWork();break;
						case 2:Pick_ConfigFontColorWork();break;
						case 3:Pick_ConfigBackColorWork();break;
						case 4:Reading_progress();break;
					}
					//��ʾ���ý���
					Pick_Gui(index_temp,4);
					gui_6(index_temp,4);
					Show_cStr((lcddev.width-32*7)/2,(50-32)/2,32*7,32,(u8 *)"<<�Ķ�������>>",32,1,WHITE);
					Show_cStr((100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"��",32,1,WHITE);
					Show_cStr(385+(100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"����",32,1,WHITE);
					//��ӡ���ý�����Ϣ
					for(i=0;i<4;i++)
					{
						Show_cStr(10,50+50*i+(50-32)/2,32*12,32,config_buff[i],32,1,BLACK);
					}
				}			
			}
			else if(ctrl_ret==CTOR_OK) 
			{
				index=index_temp;
				//���ý����ת					
				switch (index){
					case 1:Pick_ConfigFontSizeWork();break;
					case 2:Pick_ConfigFontColorWork();break;
					case 3:Pick_ConfigBackColorWork();break;
					case 4:Reading_progress();break;
				}
				//��ʾ���ý���
				Pick_Gui(index_temp,4);
				gui_6(index_temp,4);
				Show_cStr((lcddev.width-32*7)/2,(50-32)/2,32*7,32,(u8 *)"<<�Ķ�������>>",32,1,WHITE);
				Show_cStr((100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"��",32,1,WHITE);
				Show_cStr(385+(100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"����",32,1,WHITE);
				//��ӡ���ý�����Ϣ
				for(i=0;i<4;i++)
				{
					Show_cStr(10,50+50*i+(50-32)/2,32*12,32,config_buff[i],32,1,BLACK);
				}
				printf("CTOR_OK\n");
			}
			else if(ctrl_ret==CTOR_RET)
			{
				printf("CTOR_RET\n");
				break;
			}
		}
	}
	printf("���ý����˳�\n");
}
void Pick_ConfigFontSizeWork(void)
{	
	TOUCH ctrl_ret=CTOR_NOR;
	u8 index=1,index_temp;
	u8 i;
	u8 *config_buff[5]={(u8 *)"12*12",(u8 *)"16*16",(u8 *)"24*24",(u8 *)"32*32",(u8 *)"48*48"};
	index_temp=index;
	//��ʾ���ý���
	Pick_Gui(index_temp,5);
	gui_6(index_temp,5);
	Show_cStr((lcddev.width-32*4)/2,(50-32)/2,32*4,32,(u8 *)"��������",32,1,WHITE);
	Show_cStr((100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"��",32,1,WHITE);
	Show_cStr(385+(100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"����",32,1,WHITE);
	//��ӡ���ý�����Ϣ
	for(i=0;i<5;i++)
	{
		Show_cStr(10,50+50*i+(50-32)/2,32*12,32,config_buff[i],32,1,BLACK);
	}
	switch (read_config.font_size){
		case 12:
		gui_fill_circle(450,75,5,RED);
		break;
		case 16:
		gui_fill_circle(450,125,5,RED);
		break;
		case 24:
		gui_fill_circle(450,175,5,RED);
		break;
		case 32:
		gui_fill_circle(450,225,5,RED);
		break;
		case 48:
		gui_fill_circle(450,275,5,RED);
		break;
	}
	//�����ж�
	while(1)
	{	
		DelayMs(50);
		Read_Ctrl_Clear();
		ctrl_ret=Read_Ctrl(4);
		if(ctrl_ret!=CTOR_NOR)
		{		
			printf("ctrl_red = %d\n",ctrl_ret);
			if(ctrl_ret>=CTOR_PICK1 && (ctrl_ret<=CTOR_PICK14 && ctrl_ret<=5)) 
			{
				printf("pick%d\n",ctrl_ret);
				gui_4((index_temp-1)%14);
				Show_cStr(10,50+50*((index_temp-1)%14)+(50-32)/2,32*12,32,config_buff[(index_temp-1)%14],32,1,BLACK);
				index_temp=ctrl_ret;
				gui_2(index_temp,5);
				gui_3((index_temp-1)%14);	
				switch (read_config.font_size){
					case 12:
					gui_fill_circle(450,75,5,RED);
					break;
					case 16:
					gui_fill_circle(450,125,5,RED);
					break;
					case 24:
					gui_fill_circle(450,175,5,RED);
					break;
					case 32:
					gui_fill_circle(450,225,5,RED);
					break;
					case 48:
					gui_fill_circle(450,275,5,RED);
					break;
				}
				gui_6(index_temp,5);
				Show_cStr(10,50+50*((index_temp-1)%14)+(50-32)/2,32*12,32,config_buff[(index_temp-1)%14],32,1,BLACK);			
				if(index != index_temp)
				{
					index=index_temp;
				}
				else
				{
					//����
					switch (read_config.font_size){
						case 12:
							gui_fill_circle(450,75,5,WHITE);
						break;
						case 16:
							gui_fill_circle(450,125,5,WHITE);
						break;
						case 24:
							gui_fill_circle(450,175,5,WHITE);
						break;
						case 32:
							gui_fill_circle(450,225,5,WHITE);
						break;
						case 48:
							gui_fill_circle(450,275,5,WHITE);
						break;
					}
					switch (index){
						case 1: read_config.font_size=12;break;
						case 2: read_config.font_size=16;break;
						case 3: read_config.font_size=24;break;
						case 4: read_config.font_size=32;break;
						case 5: read_config.font_size=48;break;
					}
					switch (read_config.font_size){
						case 12:
							gui_fill_circle(450,75,5,RED);
						break;
						case 16:
							gui_fill_circle(450,125,5,RED);
						break;
						case 24:
							gui_fill_circle(450,175,5,RED);
						break;
						case 32:
							gui_fill_circle(450,225,5,RED);
						break;
						case 48:
							gui_fill_circle(450,275,5,RED);
						break;
					}
				}			
			}
			else if(ctrl_ret==CTOR_OK) 
			{
				index=index_temp;
				//���ý����ת
				//����
				switch (read_config.font_size){
					case 12:
						gui_fill_circle(450,75,5,WHITE);
					break;
					case 16:
						gui_fill_circle(450,125,5,WHITE);
					break;
					case 24:
						gui_fill_circle(450,175,5,WHITE);
					break;
					case 32:
						gui_fill_circle(450,225,5,WHITE);
					break;
					case 48:
						gui_fill_circle(450,275,5,WHITE);
					break;
				}
				switch (index){
					case 1: read_config.font_size=12;break;
					case 2: read_config.font_size=16;break;
					case 3: read_config.font_size=24;break;
					case 4: read_config.font_size=32;break;
					case 5: read_config.font_size=48;break;
				}
				switch (read_config.font_size){
					case 12:
						gui_fill_circle(450,75,5,RED);
					break;
					case 16:
						gui_fill_circle(450,125,5,RED);
					break;
					case 24:
						gui_fill_circle(450,175,5,RED);
					break;
					case 32:
						gui_fill_circle(450,225,5,RED);
					break;
					case 48:
						gui_fill_circle(450,275,5,RED);
					break;
				}
				printf("CTOR_OK\n");
			}
			else if(ctrl_ret==CTOR_RET)
			{
				printf("CTOR_RET\n");
				break;
			}
		}
	}
	printf("���ý����˳�\n");
}
void Pick_ConfigFontColorWork(void)
{
	TOUCH ctrl_ret=CTOR_NOR;
	u8 index=1,index_temp;
	u8 i;
	u8 *config_buff[6]={(u8 *)"��ɫ",(u8 *)"��ɫ",(u8 *)"��ɫ",(u8 *)"��ɫ",(u8 *)"��ɫ",(u8 *)"��ɫ"};
	index_temp=index;
	//��ʾ���ý���
	Pick_Gui(index_temp,6);
	gui_6(index_temp,6);
	Show_cStr((lcddev.width-32*6)/2,(50-32)/2,32*6,32,(u8 *)"������ɫ����",32,1,WHITE);
	Show_cStr((100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"��",32,1,WHITE);
	Show_cStr(385+(100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"����",32,1,WHITE);
	//��ӡ���ý�����Ϣ
	for(i=0;i<6;i++)
	{
		Show_cStr(10,50+50*i+(50-32)/2,32*12,32,config_buff[i],32,1,BLACK);
	}					
	switch (read_config.font_color){
		case WHITE:
			gui_fill_circle(450,75,5,RED);
		break;
		case BLACK:
			gui_fill_circle(450,125,5,RED);
		break;
		case RED:
			gui_fill_circle(450,175,5,RED);
		break;
		case YELLOW:
			gui_fill_circle(450,225,5,RED);
		break;
		case GREEN:
			gui_fill_circle(450,275,5,RED);
		break;
		case BLUE:
			gui_fill_circle(450,325,5,RED);
		break;
	}
	//�����ж�
	while(1)
	{	
		DelayMs(50);
		Read_Ctrl_Clear();
		ctrl_ret=Read_Ctrl(4);
		if(ctrl_ret!=CTOR_NOR)
		{		
			printf("ctrl_red = %d\n",ctrl_ret);
			if(ctrl_ret>=CTOR_PICK1 && (ctrl_ret<=CTOR_PICK14 && ctrl_ret<=6)) 
			{
				printf("pick%d\n",ctrl_ret);
				gui_4((index_temp-1)%14);
				Show_cStr(10,50+50*((index_temp-1)%14)+(50-32)/2,32*12,32,config_buff[(index_temp-1)%14],32,1,BLACK);
				index_temp=ctrl_ret;
				gui_2(index_temp,6);
				gui_3((index_temp-1)%14);					
				switch (read_config.font_color){
					case WHITE:
						gui_fill_circle(450,75,5,RED);
					break;
					case BLACK:
						gui_fill_circle(450,125,5,RED);
					break;
					case RED:
						gui_fill_circle(450,175,5,RED);
					break;
					case YELLOW:
						gui_fill_circle(450,225,5,RED);
					break;
					case GREEN:
						gui_fill_circle(450,275,5,RED);
					break;
					case BLUE:
						gui_fill_circle(450,325,5,RED);
					break;
				}
				gui_6(index_temp,6);
				Show_cStr(10,50+50*((index_temp-1)%14)+(50-32)/2,32*12,32,config_buff[(index_temp-1)%14],32,1,BLACK);			
				if(index != index_temp)
				{
					index=index_temp;
				}
				else
				{
					//����
					switch (read_config.font_color){
						case WHITE:
							gui_fill_circle(450,75,5,WHITE);
						break;
						case BLACK:
							gui_fill_circle(450,125,5,WHITE);
						break;
						case RED:
							gui_fill_circle(450,175,5,WHITE);
						break;
						case YELLOW:
							gui_fill_circle(450,225,5,WHITE);
						break;
						case GREEN:
							gui_fill_circle(450,275,5,WHITE);
						break;
						case BLUE:
							gui_fill_circle(450,325,5,WHITE);
						break;
					}
					switch (index){
						case 1: read_config.font_color=WHITE;break;
						case 2: read_config.font_color=BLACK;break;
						case 3: read_config.font_color=RED;break;
						case 4: read_config.font_color=YELLOW;break;
						case 5: read_config.font_color=GREEN;break;
						case 6: read_config.font_color=BLUE;break;
					}
					switch (read_config.font_color){
						case WHITE:
							gui_fill_circle(450,75,5,RED);
						break;
						case BLACK:
							gui_fill_circle(450,125,5,RED);
						break;
						case RED:
							gui_fill_circle(450,175,5,RED);
						break;
						case YELLOW:
							gui_fill_circle(450,225,5,RED);
						break;
						case GREEN:
							gui_fill_circle(450,275,5,RED);
						break;
						case BLUE:
							gui_fill_circle(450,325,5,RED);
						break;
					}
				}			
			}
			else if(ctrl_ret==CTOR_OK) 
			{
				index=index_temp;
				//���ý����ת
				//����
				switch (read_config.font_color){
					case WHITE:
						gui_fill_circle(450,75,5,WHITE);
					break;
					case BLACK:
						gui_fill_circle(450,125,5,WHITE);
					break;
					case RED:
						gui_fill_circle(450,175,5,WHITE);
					break;
					case YELLOW:
						gui_fill_circle(450,225,5,WHITE);
					break;
					case GREEN:
						gui_fill_circle(450,275,5,WHITE);
					break;
					case BLUE:
						gui_fill_circle(450,325,5,WHITE);
					break;
				}
				switch (index){
					case 1: read_config.font_color=WHITE;break;
					case 2: read_config.font_color=BLACK;break;
					case 3: read_config.font_color=RED;break;
					case 4: read_config.font_color=YELLOW;break;
					case 5: read_config.font_color=GREEN;break;
					case 6: read_config.font_color=BLUE;break;
				}
				switch (read_config.font_color){
					case WHITE:
						gui_fill_circle(450,75,5,RED);
					break;
					case BLACK:
						gui_fill_circle(450,125,5,RED);
					break;
					case RED:
						gui_fill_circle(450,175,5,RED);
					break;
					case YELLOW:
						gui_fill_circle(450,225,5,RED);
					break;
					case GREEN:
						gui_fill_circle(450,275,5,RED);
					break;
					case BLUE:
						gui_fill_circle(450,325,5,RED);
					break;
				}
				printf("CTOR_OK\n");
			}
			else if(ctrl_ret==CTOR_RET)
			{
				printf("CTOR_RET\n");
				break;
			}
		}
	}
	printf("���ý����˳�\n");
}
void Pick_ConfigBackColorWork(void)
{
	TOUCH ctrl_ret=CTOR_NOR;
	u8 index=1,index_temp;
	u8 i;
	u8 *config_buff[6]={(u8 *)"��ɫ",(u8 *)"��ɫ",(u8 *)"��ɫ",(u8 *)"��ɫ",(u8 *)"��ɫ",(u8 *)"��ɫ"};
	index_temp=index;
	//��ʾ���ý���
	Pick_Gui(index_temp,6);
	gui_6(index_temp,6);
	Show_cStr((lcddev.width-32*6)/2,(50-32)/2,32*6,32,(u8 *)"������ɫ����",32,1,WHITE);
	Show_cStr((100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"��",32,1,WHITE);
	Show_cStr(385+(100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"����",32,1,WHITE);
	//��ӡ���ý�����Ϣ
	for(i=0;i<6;i++)
	{
		Show_cStr(10,50+50*i+(50-32)/2,32*12,32,config_buff[i],32,1,BLACK);
	}
	switch (read_config.backdrop_color){
		case WHITE:
			gui_fill_circle(450,75,5,RED);
		break;
		case BLACK:
			gui_fill_circle(450,125,5,RED);
		break;
		case RED:
			gui_fill_circle(450,175,5,RED);
		break;
		case YELLOW:
			gui_fill_circle(450,225,5,RED);
		break;
		case GREEN:
			gui_fill_circle(450,275,5,RED);
		break;
		case BLUE:
			gui_fill_circle(450,325,5,RED);
		break;
	}
	//�����ж�
	while(1)
	{	
		DelayMs(50);
		Read_Ctrl_Clear();
		ctrl_ret=Read_Ctrl(4);
		if(ctrl_ret!=CTOR_NOR)
		{		
			printf("ctrl_red = %d\n",ctrl_ret);
			if(ctrl_ret>=CTOR_PICK1 && (ctrl_ret<=CTOR_PICK14 && ctrl_ret<=6)) 
			{
				printf("pick%d\n",ctrl_ret);
				gui_4((index_temp-1)%14);
				Show_cStr(10,50+50*((index_temp-1)%14)+(50-32)/2,32*12,32,config_buff[(index_temp-1)%14],32,1,BLACK);
				index_temp=ctrl_ret;
				gui_2(index_temp,6);
				gui_3((index_temp-1)%14);
				switch (read_config.backdrop_color){
					case WHITE:
						gui_fill_circle(450,75,5,RED);
					break;
					case BLACK:
						gui_fill_circle(450,125,5,RED);
					break;
					case RED:
						gui_fill_circle(450,175,5,RED);
					break;
					case YELLOW:
						gui_fill_circle(450,225,5,RED);
					break;
					case GREEN:
						gui_fill_circle(450,275,5,RED);
					break;
					case BLUE:
						gui_fill_circle(450,325,5,RED);
					break;
				}
				gui_6(index_temp,5);
				Show_cStr(10,50+50*((index_temp-1)%14)+(50-32)/2,32*12,32,config_buff[(index_temp-1)%14],32,1,BLACK);			
				if(index != index_temp)
				{
					index=index_temp;
				}
				else
				{
					//����
					switch (read_config.backdrop_color){
						case WHITE:
							gui_fill_circle(450,75,5,WHITE);
						break;
						case BLACK:
							gui_fill_circle(450,125,5,WHITE);
						break;
						case RED:
							gui_fill_circle(450,175,5,WHITE);
						break;
						case YELLOW:
							gui_fill_circle(450,225,5,WHITE);
						break;
						case GREEN:
							gui_fill_circle(450,275,5,WHITE);
						break;
						case BLUE:
							gui_fill_circle(450,325,5,WHITE);
						break;
					}
					switch (index){
						case 1: read_config.backdrop_color=WHITE;break;
						case 2: read_config.backdrop_color=BLACK;break;
						case 3: read_config.backdrop_color=RED;break;
						case 4: read_config.backdrop_color=YELLOW;break;
						case 5: read_config.backdrop_color=GREEN;break;
						case 6: read_config.backdrop_color=BLUE;break;
					}
					switch (read_config.backdrop_color){
						case WHITE:
							gui_fill_circle(450,75,5,RED);
						break;
						case BLACK:
							gui_fill_circle(450,125,5,RED);
						break;
						case RED:
							gui_fill_circle(450,175,5,RED);
						break;
						case YELLOW:
							gui_fill_circle(450,225,5,RED);
						break;
						case GREEN:
							gui_fill_circle(450,275,5,RED);
						break;
						case BLUE:
							gui_fill_circle(450,325,5,RED);
						break;
					}
				}			
			}
			else if(ctrl_ret==CTOR_OK) 
			{
				index=index_temp;
				//���ý����ת
				//����
				switch (read_config.backdrop_color){
					case WHITE:
						gui_fill_circle(450,75,5,WHITE);
					break;
					case BLACK:
						gui_fill_circle(450,125,5,WHITE);
					break;
					case RED:
						gui_fill_circle(450,175,5,WHITE);
					break;
					case YELLOW:
						gui_fill_circle(450,225,5,WHITE);
					break;
					case GREEN:
						gui_fill_circle(450,275,5,WHITE);
					break;
					case BLUE:
						gui_fill_circle(450,325,5,WHITE);
					break;
				}
				switch (index){
					case 1: read_config.backdrop_color=WHITE;break;
					case 2: read_config.backdrop_color=BLACK;break;
					case 3: read_config.backdrop_color=RED;break;
					case 4: read_config.backdrop_color=YELLOW;break;
					case 5: read_config.backdrop_color=GREEN;break;
					case 6: read_config.backdrop_color=BLUE;break;
				}
				switch (read_config.backdrop_color){
					case WHITE:
						gui_fill_circle(450,75,5,RED);
					break;
					case BLACK:
						gui_fill_circle(450,125,5,RED);
					break;
					case RED:
						gui_fill_circle(450,175,5,RED);
					break;
					case YELLOW:
						gui_fill_circle(450,225,5,RED);
					break;
					case GREEN:
						gui_fill_circle(450,275,5,RED);
					break;
					case BLUE:
						gui_fill_circle(450,325,5,RED);
					break;
				}
				printf("CTOR_OK\n");
			}
			else if(ctrl_ret==CTOR_RET)
			{
				printf("CTOR_RET\n");
				break;
			}
		}
	}
	printf("���ý����˳�\n");
}
void Reading_progress(void)
{
	char num[8];
	u32 rlen;
	TOUCH ctrl_ret=CTOR_NOR;
	FRESULT res;
	u8 ch;
	unsigned long temp_current;
	//����������
	LCD_Clear(WHITE);
	gui_0();
	Show_cStr(385+(100-32*2)/2,750+(50-32)/2,32*2,32,(u8 *)"����",32,1,WHITE);
	Show_cStr((lcddev.width-32*6)/2,(50-32)/2,32*6,32,(u8 *)"�Ķ����ȵ���",32,1,WHITE);
	Show_cStr((lcddev.width-32*6)/2,(50-32)/2+200,32*6,32,(u8 *)"������������",32,1,BLACK);
	//��������
	gui_7(read_config.current,file.fsize);
	//��ʾ���Ȱٷֱ�
	sprintf(num,"%2.2f%%",read_config.current/1.0/file.fsize*100);
	LCD_ShowString(200,450,strlen(num)*24/2,24,24,(u8 *)num);
	while(1)
	{
		DelayMs(50);
		Read_Ctrl_Clear();
		ctrl_ret=Read_Ctrl(5);
		if(ctrl_ret!=CTOR_NOR)
		{
			if(ctrl_ret==CTOR_RET)
			{
				printf("CTOR_RET\n");
				break; 
			}
			else if(ctrl_ret==CTOR_OK)
			{
				temp_current=(tp_dev.x[0]-20)/1.0/440*file.fsize;
				//ָ�뺺���м����
				f_lseek(&file,temp_current);
				res=f_read(&file,&ch,1,&rlen);
				if(res != FR_OK)
				{
					printf("�ļ���ȡʧ��\t������%d\n",res);
					return ;
				}
				if(ch<0x80)
				{
					read_config.current=temp_current+1;
				}
				else
				{
					read_config.current=temp_current;
				}
				
				printf("���� %ld\n",read_config.current);
				printf("�ܹ� %ld\n",file.fsize);
				//��������
				gui_7(read_config.current,file.fsize);
				//��ʾ���Ȱٷֱ�
				LCD_Fill(200,450,400,474,WHITE);
				sprintf(num,"%2.2f%%",read_config.current/1.0/file.fsize*100);
				LCD_ShowString(200,450,strlen(num)*24/2,24,24,(u8 *)num);
			}
		}			
	}
}
