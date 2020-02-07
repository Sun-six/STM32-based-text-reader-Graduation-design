#include "fontupd.h"
#include "ff.h"	  
#include "w25qxx.h"   
#include "lcd.h"  
#include "string.h"
#include "delay.h"
#include "usart.h"
//�ֿ�����ռ�õ�����������С(6���ֿ�+unigbk��+�ֿ���Ϣ=3238700�ֽ�,Լռ3193��W25QXX����)
#define FONTSECSIZE	 	3193
//�ֿ�����ʼ��ַ 
#define FONTINFOADDR 	1024*1024*0 					
														
//���������ֿ������Ϣ����ַ����С��
_font_info ftinfo;

//�ֿ����ڴ����е�·��
u8*const GBK64_PATH=(u8 *)"/FONT/GBK64.FON";		//GBK64�Ĵ��λ��
u8*const GBK56_PATH=(u8 *)"/FONT/GBK56.FON";		//GBK56�Ĵ��λ��
u8*const GBK48_PATH=(u8 *)"/FONT/GBK48.FON";		//GBK48�Ĵ��λ��
u8*const GBK32_PATH=(u8 *)"/FONT/GBK32.FON";		//GBK32�Ĵ��λ��
u8*const GBK24_PATH=(u8 *)"/FONT/GBK24.FON";		//GBK24�Ĵ��λ��
u8*const GBK16_PATH=(u8 *)"/FONT/GBK16.FON";		//GBK16�Ĵ��λ��
u8*const GBK12_PATH=(u8 *)"/FONT/GBK12.FON";		//GBK12�Ĵ��λ��
u8*const UNIGBK_PATH=(u8 *)"/FONT/UNIGBK.BIN";		//UNIGBK.BIN�Ĵ��λ��

//��ʾ��ǰ������½���
//x,y:����
//size:�����С
//fsize:�����ļ���С
//pos:��ǰ�ļ�ָ��λ��
u32 fupd_prog(u16 x,u16 y,u8 size,u32 fsize,u32 pos)
{
	float prog;
	u8 t=0XFF;
	prog=(float)pos/fsize;
	prog*=100;
	if(t!=prog)
	{
		LCD_ShowString(x+3*size/2,y,240,320,size,(u8 *)"%");		
		t=prog;
		if(t>100)t=100;
		LCD_ShowNum(x,y,t,3,size);//��ʾ��ֵ
	}
	return 0;					    
} 
//����ĳһ��
//x,y:����
//size:�����С
//fxpath:·��
//fx:���µ����� 0,ungbk;1,gbk12;2,gbk16;3,gbk24;
//����ֵ:0,�ɹ�;����,ʧ��.
u8 updata_fontx(u16 x,u16 y,u8 size,u8 *fxpath,u8 fx)
{
	u32 flashaddr=0;								    
	FIL * fftemp;
	u8 *tempbuf;
 	u8 res;	
	u16 bread;
	u32 offx=0;
	u8 rval=0;	     
	fftemp=(FIL*)malloc(sizeof(FIL));	//�����ڴ�	
	if(fftemp==NULL)rval=1;
	tempbuf=malloc(4096);				//����4096���ֽڿռ�
	if(tempbuf==NULL)rval=1;
 	res=f_open(fftemp,(const TCHAR*)fxpath,FA_READ); 
 	if(res)rval=2;//���ļ�ʧ��  
 	if(rval==0)	 
	{
		switch(fx)
		{
			case 0:												//����UNIGBK.BIN
				ftinfo.ugbkaddr=FONTINFOADDR+sizeof(ftinfo);	//��Ϣͷ֮�󣬽���UNIGBKת�����
				ftinfo.ugbksize=fftemp->fsize;					//UNIGBK��С
				flashaddr=ftinfo.ugbkaddr;
				break;
			case 1:
				ftinfo.f12addr=ftinfo.ugbkaddr+ftinfo.ugbksize;	//UNIGBK֮�󣬽���GBK12�ֿ�
				ftinfo.gbk12size=fftemp->fsize;					//GBK12�ֿ��С
				flashaddr=ftinfo.f12addr;						//GBK12����ʼ��ַ
				break;
			case 2:
				ftinfo.f16addr=ftinfo.f12addr+ftinfo.gbk12size;	//GBK12֮�󣬽���GBK16�ֿ�
				ftinfo.gbk16size=fftemp->fsize;					//GBK16�ֿ��С
				flashaddr=ftinfo.f16addr;						//GBK16����ʼ��ַ
				break;
			case 3:
				ftinfo.f24addr=ftinfo.f16addr+ftinfo.gbk16size;	//GBK16֮�󣬽���GBK24�ֿ�
				ftinfo.gkb24size=fftemp->fsize;					//GBK24�ֿ��С
				flashaddr=ftinfo.f24addr;						//GBK24����ʼ��ַ
				break;
			case 4:
				ftinfo.f32addr=ftinfo.f24addr+ftinfo.gkb24size;	//GBK24֮�󣬽���GBK32�ֿ�
				ftinfo.gkb32size=fftemp->fsize;					//GBK32�ֿ��С
				flashaddr=ftinfo.f32addr;						//GBK32����ʼ��ַ
				break;
			case 5:
				ftinfo.f48addr=ftinfo.f32addr+ftinfo.gkb32size;	//GBK32֮�󣬽���GBK48�ֿ�
				ftinfo.gkb48size=fftemp->fsize;					//GBK48�ֿ��С
				flashaddr=ftinfo.f48addr;						//GBK48����ʼ��ַ
				break;
//			case 6:
//				ftinfo.f56addr=ftinfo.f48addr+ftinfo.gkb48size;	//GBK48֮�󣬽���GBK56�ֿ�
//				ftinfo.gkb56size=fftemp->fsize;					//GBK56�ֿ��С
//				flashaddr=ftinfo.f56addr;						//GBK56����ʼ��ַ
//				break;
//			case 8:
//				ftinfo.f64addr=ftinfo.f56addr+ftinfo.gkb56size;	//GBK56֮�󣬽���GBK64�ֿ�
//				ftinfo.gkb64size=fftemp->fsize;					//GBK64�ֿ��С
//				flashaddr=ftinfo.f64addr;						//GBK64����ʼ��ַ
//				break;
		} 
			
		while(res==FR_OK)//��ѭ��ִ��
		{
	 		res=f_read(fftemp,tempbuf,4096,(UINT *)&bread);		//��ȡ����	 
			if(res!=FR_OK)break;								//ִ�д���
			W25QXX_Write(tempbuf,offx+flashaddr,4096);		//��0��ʼд��4096������  
	  		offx+=bread;	  
			fupd_prog(x,y,size,fftemp->fsize,offx);	 			//������ʾ
			if(bread!=4096)break;								//������.
	 	} 	
		f_close(fftemp);		
	}			 
	free(fftemp);	//�ͷ��ڴ�
	free(tempbuf);	//�ͷ��ڴ�
	return res;
} 
//���������ļ�,UNIGBK,GBK12,GBK16,GBK24һ�����
//x,y:��ʾ��Ϣ����ʾ��ַ
//size:�����С
//src:�ֿ���Դ����."0:",SD��;"1:",FLASH��,"2:",U��.
//��ʾ��Ϣ�����С										  
//����ֵ:0,���³ɹ�;
//		 ����,�������.	  
u8 update_font(u16 x,u16 y,u8 size,u8* src)
{	
	u8 *pname;
	u32 *buf;
	u8 res=0;		   
 	u16 i,j;
	FIL *fftemp;
	u8 rval=0; 
	res=0XFF;		
	ftinfo.fontok=0XFF;
	pname=malloc(100);	//����100�ֽ��ڴ�  
	buf=malloc(4096);	//����4K�ֽ��ڴ�  
	fftemp=(FIL*)malloc(sizeof(FIL));	//�����ڴ�	
	if(buf==NULL||pname==NULL||fftemp==NULL)
	{
		free(fftemp);
		free(pname);
		free(buf);
		return 5;	//�ڴ�����ʧ��
	}
	//�Ȳ����ļ��Ƿ����� 
	strcpy((char*)pname,(char*)src);	//copy src���ݵ�pname
	strcat((char*)pname,(char*)UNIGBK_PATH); 
 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ); 
 	if(res)rval|=1<<4;//���ļ�ʧ��  
	strcpy((char*)pname,(char*)src);	//copy src���ݵ�pname
	strcat((char*)pname,(char*)GBK12_PATH); 
 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ); 
 	if(res)rval|=1<<5;//���ļ�ʧ��  
	strcpy((char*)pname,(char*)src);	//copy src���ݵ�pname
	strcat((char*)pname,(char*)GBK16_PATH); 
 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ); 
 	if(res)rval|=1<<6;//���ļ�ʧ��  
	strcpy((char*)pname,(char*)src);	//copy src���ݵ�pname
	strcat((char*)pname,(char*)GBK24_PATH); 
 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ); 
 	if(res)rval|=1<<7;//���ļ�ʧ�� 
	strcpy((char*)pname,(char*)src);	//copy src���ݵ�pname
	strcat((char*)pname,(char*)GBK32_PATH); 
 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ); 
 	if(res)rval|=1<<8;//���ļ�ʧ��    
	strcpy((char*)pname,(char*)src);	//copy src���ݵ�pname
	strcat((char*)pname,(char*)GBK48_PATH); 
 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ); 
 	if(res)rval|=1<<9;//���ļ�ʧ�� 
//	strcpy((char*)pname,(char*)src);	//copy src���ݵ�pname
//	strcat((char*)pname,(char*)GBK56_PATH); 
// 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ); 
// 	if(res)rval|=1<<10;//���ļ�ʧ�� 
//	strcpy((char*)pname,(char*)src);	//copy src���ݵ�pname
//	strcat((char*)pname,(char*)GBK64_PATH); 
// 	res=f_open(fftemp,(const TCHAR*)pname,FA_READ); 
// 	if(res)rval|=1<<11;//���ļ�ʧ��
	free(fftemp);//�ͷ��ڴ�
	if(rval==0)//�ֿ��ļ�������.
	{  
		LCD_ShowString(x,y,240,320,size,(u8 *)"Erasing sectors... ");//��ʾ���ڲ�������	
		for(i=0;i<FONTSECSIZE;i++)	//�Ȳ����ֿ�����,���д���ٶ�
		{
			fupd_prog(x+20*size/2,y,size,FONTSECSIZE,i);//������ʾ
			W25QXX_Read((u8*)buf,((FONTINFOADDR/4096)+i)*4096,4096);//������������������
			for(j=0;j<1024;j++)//У������
			{
				if(buf[j]!=0XFFFFFFFF)break;//��Ҫ����  	  
			}
			if(j!=1024)W25QXX_Erase_Sector((FONTINFOADDR/4096)+i);	//��Ҫ����������
		}
		free(buf);
		LCD_ShowString(x,y,240,320,size,(u8 *)"Updating UNIGBK.BIN");		
		strcpy((char*)pname,(char*)src);				//copy src���ݵ�pname
		strcat((char*)pname,(char*)UNIGBK_PATH); 
		res=updata_fontx(x+20*size/2,y,size,pname,0);	//����UNIGBK.BIN
		if(res){free(pname);return 1;}
		LCD_ShowString(x,y,240,320,size,(u8 *)"Updating GBK12.BIN  ");
		strcpy((char*)pname,(char*)src);				//copy src���ݵ�pname
		strcat((char*)pname,(char*)GBK12_PATH); 
		res=updata_fontx(x+20*size/2,y,size,pname,1);	//����GBK12.FON
		if(res){free(pname);return 2;}
		LCD_ShowString(x,y,240,320,size,(u8 *)"Updating GBK16.BIN  ");
		strcpy((char*)pname,(char*)src);				//copy src���ݵ�pname
		strcat((char*)pname,(char*)GBK16_PATH); 
		res=updata_fontx(x+20*size/2,y,size,pname,2);	//����GBK16.FON
		if(res){free(pname);return 3;}
		LCD_ShowString(x,y,240,320,size,(u8 *)"Updating GBK24.BIN  ");
		strcpy((char*)pname,(char*)src);				//copy src���ݵ�pname
		strcat((char*)pname,(char*)GBK24_PATH); 
		res=updata_fontx(x+20*size/2,y,size,pname,3);	//����GBK24.FON
		if(res){free(pname);return 4;}
		LCD_ShowString(x,y,240,320,size,(u8 *)"Updating GBK32.BIN  ");
		strcpy((char*)pname,(char*)src);				//copy src���ݵ�pname
		strcat((char*)pname,(char*)GBK32_PATH); 
		res=updata_fontx(x+20*size/2,y,size,pname,4);	//����GBK32.FON
		if(res){free(pname);return 5;}
		LCD_ShowString(x,y,240,320,size,(u8 *)"Updating GBK48.BIN  ");
		strcpy((char*)pname,(char*)src);				//copy src���ݵ�pname
		strcat((char*)pname,(char*)GBK48_PATH); 
		res=updata_fontx(x+20*size/2,y,size,pname,5);	//����GBK48.FON
		if(res){free(pname);return 6;}
//		LCD_ShowString(x,y,240,320,size,(u8 *)"Updating GBK56.BIN  ");
//		strcpy((char*)pname,(char*)src);				//copy src���ݵ�pname
//		strcat((char*)pname,(char*)GBK56_PATH); 
//		res=updata_fontx(x+20*size/2,y,size,pname,6);	//����GBK56.FON
//		if(res){free(pname);return 7;}
//		LCD_ShowString(x,y,240,320,size,(u8 *)"Updating GBK64.BIN  ");
//		strcpy((char*)pname,(char*)src);				//copy src���ݵ�pname
//		strcat((char*)pname,(char*)GBK64_PATH); 
//		res=updata_fontx(x+20*size/2,y,size,pname,7);	//����GBK64.FON
//		if(res){free(pname);return 8;}
		//ȫ�����º���
		ftinfo.fontok=0XAA;
		W25QXX_Write((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));	//�����ֿ���Ϣ
	}
	free(pname);//�ͷ��ڴ� 
	free(buf);
	return rval;//�޴���.			 
} 
//��ʼ������
//����ֵ:0,�ֿ����.
//		 ����,�ֿⶪʧ
u8 font_init(void)
{		
	u8 t=0;
	W25QXX_Init();  
	while(t<10)//������ȡ10��,���Ǵ���,˵��ȷʵ��������,�ø����ֿ���
	{
		t++;
		W25QXX_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//����ftinfo�ṹ������
		if(ftinfo.fontok==0XAA)break;
		DelayMs(20);
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;		    
}





























