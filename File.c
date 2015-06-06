#include "headfile.h"
#include "MyFont.h"

static char TITLE[] = "";

FONTCHARACTER * CharToFont(const char *cFileName,FONTCHARACTER *fFileName);
char * FontToChar(const FONTCHARACTER *fFileName,char *cFileName);

///////////////////////////////////////////////////////////////
//					  Char <=> Font
///////////////////////////////////////////////////////////////
FONTCHARACTER * CharToFont(const char *cFileName,FONTCHARACTER *fFileName)
{
	int i,len = strlen(cFileName);
	for(i=0; i<len ;++i)
	fFileName[i] = cFileName[i];
	fFileName[i]=0;
	return fFileName;
}
char * FontToChar(const FONTCHARACTER *fFileName,char *cFileName)
{
	int i = 0;
	while((cFileName[i]=fFileName[i])!=0)++i;
	return cFileName;
}

///////////////////////////////////////////////////////////////
//						File Directory
///////////////////////////////////////////////////////////////
int CreateDirectory()
{
	FONTCHARACTER f_path_name[20];
	Bfile_CreateDirectory(CharToFont("\\\\fls0\\SRD",f_path_name));
}
///////////////////////////////////////////////////////////////
//							File List
///////////////////////////////////////////////////////////////

char szCsvDir[]="\\\\crd0\\YS";
//char szCsvDir[]="\\\\fls0\\YS";

int SelectFile (char * file_name)
// -1 no file , 1 complate ,0 cancel
{
	int key;
	char str[50]; //�ַ����鳤���±��0��ʼ����50��Ԫ��
	FONTCHARACTER find_path[50];   //���·��
	FONTCHARACTER find_name[50];   //�������
	int find_h;
	int size = 0;
	int top = 0;
	int buttom = 0;
	int index,r,y;
	char *files;
	FILE_INFO file_info;
//	CharToFont("\\\\fls0\\SRD\\*.srd",find_path);
	sprintf(str,"%s\\*.csv",szCsvDir);
	CharToFont(str,find_path);
	if(Bfile_FindFirst (find_path,&find_h,find_name,&file_info)==0)  //��������Ŀ¼���ļ������Ʒ���ָ�����ļ���
	{
		int i=0,ret;
		BOOL bFindFirst=TRUE; //����һ�������ͱ�����ʼ��Ϊ��(true),����bool���ͣ�ֵֻҪ����0����Ϊ�棬��true����ֵΪ0ʱΪ�٣���false��
		size ++;  //ʹ��size֮���ټ�һ���ȼ����ٲ�����
		while(Bfile_FindNext(find_h,find_name,&file_info)==0) //Bfile_FindNext ����ʹ����������λƥ��һ���������Ƶ��ļ����ơ�
		size++;
		Bfile_FindClose(find_h);  //Bfile_FindClose �ر�����ָ��������
		files = (char*)malloc(size*13);
		index = 0;
		for(;;)
		{
			if(bFindFirst)  //if ֵΪ��
				ret=Bfile_FindFirst (find_path,&find_h,find_name,&file_info);
			else 
				ret=Bfile_FindNext(find_h,find_name,&file_info);
			if(ret!=0)  //if 
				break;
			strncpy((files+13*i),FontToChar(find_name,str),13);
			if(strcmp((files+13*i),file_name)==0)
			index=i; 
			i++;
			bFindFirst=FALSE;  //��ֵΪ�� ��=0
		}
		Bfile_FindClose(find_h);  //Bfile_FindClose �ر�����ָ��������
		r = 1;
		top = 0;
		buttom = 4;
		while(1)
		{
			if (r)
			{
				Bdisp_AllClr_VRAM();   //ˢ���ػ�
				//PopUpWin(5);
				SetMyFont(&stHz12x12,&stAsc6x12);
				DispStr(2,1,  "ѡ ");    
			    DispStr(2,14, "�� ");
			    DispStr(2,27, "�� ");
			    DispStr(2,41, "�� ");
				ShowTitle(TITLE);
				if (buttom-top>3)buttom = top + 3; //�ײ�-����  �ײ�=����+4
				
				if(top>index)  //if top>0
				{
					top = index;   //if top=0 
					buttom = index + 3;  //buttom= index + 3
				}
				
				if (index>buttom)       //�ļ����ڣ�buttom=4��
				{
					buttom = index;     //�ײ�=�ļ�����
					top = buttom - 3;   //����=�ļ�����-3
				}
				
				if(buttom>size - 1)buttom = size - 1;  //
				
				if(top<0) top = 0;
				
				for (i=top;i<=buttom;++i)
				{
					sprintf(str,"%13.13s",(files+13*i));
					y=2+(i-top)*13;   //�����ļ�λ��
					SetMyFont(&stHz12x12,&stAsc6x12);
					DispStr(18,y,str); //str[]���ַ���
					if(index==i)
					Bdisp_AreaReverseVRAM(15,y,20+13*6,y+11);
					//PrintfXY(20,22+(i-top)*8,index==i,"%13.13s",(files+13*i));
				}
				PrintXY(110,1+1,top>0?"\xE6\x92":" ",0);   //��ҳ���ϼ�ͷ
				PrintXY(110,64-20,buttom<size-1?"\xE6\x93":" ",0);    //��ҳ���¼�ͷ
				r = 0;
			}
			GetKey(&key);
			if (key==KEY_CTRL_UP) 	{if(--index<0) index = size - 1;r = 1;}
			if (key==KEY_CTRL_DOWN)	{if(++index>size - 1) index = 0;r = 1;}
			if (key==KEY_CTRL_EXE)
			{   
				strcpy(file_name,(files+13*index));   //�����ַ��� ��files file_name
				//sprintf(file_name,"%s\\%s",szCsvDir,(files+13*index));
				free (files);
				return 1;
			}
			if (key==KEY_CTRL_EXIT)
			{
				free (files);
				Bdisp_AllClr_VRAM(); 
				return 0;
			}
		}
	}
	else
		return -1;
	
}
