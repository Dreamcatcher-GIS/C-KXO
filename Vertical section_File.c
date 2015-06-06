#include "headfile.h"
#include "MyFont.h"
#include "runmain.h"



FONTCHARACTER * CharToFonts3(const char *cFileName,FONTCHARACTER *fFileName);
char * FontToChars3(const FONTCHARACTER *fFileName,char *cFileName);

//////////////////////////////////////////////////////////////
//					  Char <=> Font
///////////////////////////////////////////////////////////////
FONTCHARACTER * CharToFonts3(const char *cFileName,FONTCHARACTER *fFileName)
{
	int i,len = strlen(cFileName);
	for(i=0; i<len ;++i)
		fFileName[i] = cFileName[i];
	fFileName[i]=0;
	return fFileName;
}
char * FontToChars3(const FONTCHARACTER *fFileName,char *cFileName)
{
	int i = 0;
	while((cFileName[i]=fFileName[i])!=0)++i;
	return cFileName;
}

///////////////////////////////////////////////////////////////
//						File Directory
///////////////////////////////////////////////////////////////
int CreateDirectorys3()
{
	FONTCHARACTER f_path_name[20];
	Bfile_CreateDirectory(CharToFonts3("\\\\crd0\\ZDM",f_path_name));
}

char szSrdDir3[]="\\\\crd0\\ZDM";
//char szSrdDir3[]="\\\\fls0\\ZDM";

///////////////////////////////////////////////////////////////
//							File List
///////////////////////////////////////////////////////////////
int SelectFiles3 (char * file_name)
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
//	CharToFont("\\\\fls0\\ZDM\\*.zdm",find_path);
	sprintf(str,"%s\\*.zdm",szSrdDir3);
	CharToFonts3(str,find_path);
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
			strncpy((files+13*i),FontToChars3(find_name,str),13);
			if(strcmp((files+13*i),setup_data.zdmroute_name)==0) //�ļ�ѡ�к����ڼ���λ��
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
				Bdisp_AllClr_VRAM();
				//PopUpWin(6);
				DispStr(20,8,"�ݶ����ļ�: ");
				if (buttom-top>3)buttom = top + 3;
				if(top>index)
				{
					top = index;
					buttom = index + 3;
				}
				if (index>buttom)
				{
					buttom = index;
					top = buttom - 3;
				}
				if(buttom>size - 1)buttom = size - 1;
				if(top<0) top = 0;
				for (i=top;i<=buttom;++i)
				{
					//sprintf(str,"%13.13s",(files+13*i));
					//y=2+(i-top)*13;   //�����ļ�λ��
					//SetMyFont(&stHz12x12,&stAsc6x12);
					//DispStr(18,y,str); //str[]���ַ���
					//if(index==i)
					//Bdisp_AreaReverseVRAM(15,y,20+13*6,y+11);
				    PrintfXY(20,22+(i-top)*8,index==i,"%13.13s",(files+13*i));
				}
				PrintXY(100,22,top>0?"\xE6\x92":" ",0);
				PrintXY(100,46,buttom<size-1?"\xE6\x93":" ",0);
				r = 0;
			}
			GetKey(&key);
			if (key==KEY_CTRL_UP) 	{if(--index<0) index = size - 1;r = 1;}
			if (key==KEY_CTRL_DOWN)	{if(++index>size - 1) index = 0;r = 1;}
			if (key==KEY_CTRL_EXE)
			{
				strcpy(file_name,(files+13*index));
				free (files);
				return 1;
			}
			if (key==KEY_CTRL_EXIT)
			{
				free (files);
				return 0;
			}
		}
	}
	else
		return -1;
	
}
///////////////////////////////////////////////////////////////
//							SRD
///////////////////////////////////////////////////////////////
int TestSRDFile3 (const char *file_name_c_a)
{
	FONTCHARACTER file_name_f[30];
	char file_name_c[30];
	int file_h;

	sprintf (file_name_c,"\\\\crd0\\ZDM\\%s",file_name_c_a);	

	CharToFonts3(file_name_c,file_name_f);
	file_h = Bfile_OpenFile (file_name_f,_OPENMODE_READ);
	
	if (file_h <0)
		return 0;
	else
	{
		Bfile_CloseFile (file_h);
		return 1;
	}
}

int DelSRDFile3 (const char *file_name_c_a)
{
	FONTCHARACTER file_name_f[30];
	char file_name_c[30];
	int file_h;

	sprintf (file_name_c,"\\\\crd0\\ZDM\\%s",file_name_c_a);	

	CharToFonts3(file_name_c,file_name_f);
	Bfile_DeleteFile (file_name_f);
	return 0;
}

void CalZDM()
{
	int i;
	double I1,I2,bad,cave_vex;
	if(zdmroute.size<3)
	{
		return ;
	}
	else{
		for(i=1;i<zdmroute.size-1;i++)
		{
			I1=(zdmroute.set[i].zdm_bbgc-zdmroute.set[i-1].zdm_bbgc)/(zdmroute.set[i].zdm_bbzh-zdmroute.set[i-1].zdm_bbzh);
			I2=(zdmroute.set[i+1].zdm_bbgc-zdmroute.set[i].zdm_bbgc)/(zdmroute.set[i+1].zdm_bbzh-zdmroute.set[i].zdm_bbzh);
			bad = I2 - I1;
			cave_vex = zdmroute.set[i].zdm_r * (bad>=0?1:-1);
			zdmroute.set[i].zdm_zdzh = zdmroute.set[i].zdm_bbzh + bad * cave_vex/2;
		}
	}
}

int SaveSRDFile3 (const char *file_name_c_a)
{
	FONTCHARACTER file_name_f[30];
	char file_name_c[30];
	int file_h;
	CalZDM();
	
	sprintf (file_name_c,"\\\\crd0\\ZDM\\%s",file_name_c_a);

	CharToFonts3(file_name_c,file_name_f);
	
	Bfile_DeleteFile (file_name_f);
	Bfile_CreateFile (file_name_f,
	zdmroute.size * sizeof(ZDMPOINT) + sizeof (ZDMROUTE));
	file_h = Bfile_OpenFile (file_name_f,_OPENMODE_WRITE);
	
	Bfile_SeekFile (file_h,0);
	Bfile_WriteFile (file_h,&zdmroute,sizeof(ZDMROUTE));
	
	Bfile_SeekFile (file_h,sizeof(ZDMROUTE));
	Bfile_WriteFile (file_h,zdmroute.set,zdmroute.size * sizeof(ZDMPOINT));

	Bfile_CloseFile (file_h);
	return 0;	
}

int LoadSRDFile3 (const char *file_name_c_a)    
{
	FONTCHARACTER file_name_f[30];
	char file_name_c[30];
	int file_h;
	
	sprintf (file_name_c,"\\\\crd0\\ZDM\\%s",file_name_c_a);	

	CharToFonts3(file_name_c,file_name_f);

	file_h = Bfile_OpenFile (file_name_f,_OPENMODE_READ);
	if(file_h>=0){
	    zdmroute_clr (&zdmroute);
	    Bfile_ReadFile(file_h,&zdmroute,sizeof(ZDMROUTE),0);
	    zdmroute.set = (ZDMPOINT*)calloc(zdmroute.size,sizeof(ZDMPOINT));
	    Bfile_ReadFile(file_h,zdmroute.set,sizeof(ZDMPOINT)*zdmroute.size,sizeof(ZDMROUTE));
	
	    Bfile_CloseFile (file_h);
	    return 1;
	}
	return 0;
}


///////////////////////////////////////
/// ini
///////////////////////////////////////


//static FONTCHARACTER IniFile[] = { '\\','\\','f','l','s','0','\\','s','r','d','.','i','n','i' ,'\0'} ;
static FONTCHARACTER IniSRDFile[] = { '\\','\\','c','r','d','0','\\','z','d','m','.','i','n','i' ,'\0'} ;


void SaveIniSRDFile3()
{
	int ptr = -1;
	int ln;
	Bfile_DeleteFile(IniSRDFile);
	ln=strlen(setup_data.zdmroute_name);
	if(ln==0)
		return;
	if (Bfile_CreateFile(IniSRDFile, ln) < 0) //Error("�����ļ�ʧ��") ;
		return;
	//	else Error("�����ļ�OK") ;
	ptr = Bfile_OpenFile(IniSRDFile, _OPENMODE_WRITE) ;  // Bfile_OpenFile : ��һ�����е��ļ���
	if (ptr >= 0) {
		Bfile_WriteFile(ptr, setup_data.zdmroute_name, ln) ;
		Bfile_CloseFile(ptr) ;
	}
}

int LoadLastDataSRDFile3()
{
	char strFileName[50]; 
	int reg_ptr = -1;
	int ln;
	if(setup_data.zdmroute_name[0]==0)
	{
		reg_ptr = Bfile_OpenFile(IniSRDFile, _OPENMODE_READ) ;
		if (reg_ptr >= 0) {
			ln=Bfile_GetFileSize(reg_ptr);
			if(ln>0)
			{   
				if(ln>=sizeof(setup_data.zdmroute_name)-1)
					ln=sizeof(setup_data.zdmroute_name)-1;
				Bfile_ReadFile(reg_ptr, setup_data.zdmroute_name,ln,0) ;
				setup_data.zdmroute_name[ln]=0;
			}
			Bfile_CloseFile(reg_ptr) ;
			//PopUpWin(1);
	        //locate(7,4);Print(setup_data.zdmroute_name);
	        //GetKey(&key);
			//sprintf(strFileName,"\\\\crd0\\SRD\\%s",setup_data.zdmroute_name);
			LoadSRDFile3(setup_data.zdmroute_name);
			return 1;
		}		
	}
	return 0;
}