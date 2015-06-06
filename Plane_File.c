#include "headfile.h"
#include "MyFont.h"
#include "runmain.h"



FONTCHARACTER * PMCharToFont(const char *cFileName,FONTCHARACTER *fFileName); //*PMCharToFont*
char * PMFontToChar(const FONTCHARACTER *fFileName,char *cFileName); //*PMFontToChar*

//////////////////////////////////////////////////////////////
//					  Char <=> Font
///////////////////////////////////////////////////////////////
FONTCHARACTER * PMCharToFont(const char *cFileName,FONTCHARACTER *fFileName)
{
	int i,len = strlen(cFileName);
	for(i=0; i<len ;++i)
		fFileName[i] = cFileName[i];
	fFileName[i]=0;
	return fFileName;
}
char * PMFontToChar(const FONTCHARACTER *fFileName,char *cFileName)
{
	int i = 0;
	while((cFileName[i]=fFileName[i])!=0)++i;
	return cFileName;
}

///////////////////////////////////////////////////////////////
//						File Directory
///////////////////////////////////////////////////////////////
int PMCreateDirectory()   //*PMCreateDirectory*
{
	FONTCHARACTER f_path_name[20];
	Bfile_CreateDirectory(PMCharToFont("\\\\crd0\\PM",f_path_name));
}

char szPMDir[]="\\\\crd0\\PM";  //*szPMDir[]*
//char szPMDir[]="\\\\fls0\\PM";

///////////////////////////////////////////////////////////////
//							File List
///////////////////////////////////////////////////////////////
int SelectPMFile (char * file_name)  //*SelectPMFile*
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
//	CharToFont("\\\\fls0\\JDF\\*.pm",find_path); //*pm*
	sprintf(str,"%s\\*.pm",szPMDir); //*pm*
	PMCharToFont(str,find_path);
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
			strncpy((files+13*i),PMFontToChar(find_name,str),13);
			if(strcmp((files+13*i),setup_data.pmroute_name)==0) //�ļ�ѡ�к����ڼ���λ��
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
				DispStr(20,8,"��Ԫ�����ļ�: ");
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
//							PM
///////////////////////////////////////////////////////////////
int TestPMFile (const char *file_name_c_a)
{
	FONTCHARACTER file_name_f[30];
	char file_name_c[30];
	int file_h;

	sprintf (file_name_c,"\\\\crd0\\PM\\%s",file_name_c_a);	

	PMCharToFont(file_name_c,file_name_f);
	file_h = Bfile_OpenFile (file_name_f,_OPENMODE_READ);
	
	if (file_h <0)
		return 0;
	else
	{
		Bfile_CloseFile (file_h);
		return 1;
	}
}

int DelPMFile (const char *file_name_c_a)
{
	FONTCHARACTER file_name_f[30];
	char file_name_c[30];
	int file_h;

	sprintf (file_name_c,"\\\\crd0\\PM\\%s",file_name_c_a);	

	PMCharToFont(file_name_c,file_name_f);
	Bfile_DeleteFile (file_name_f);
	return 0;
}



int SavePMFile (const char *file_name_c_a)
{
	FONTCHARACTER file_name_f[30];
	char file_name_c[30];
	int file_h;
	sprintf (file_name_c,"\\\\crd0\\PM\\%s",file_name_c_a);

	PMCharToFont(file_name_c,file_name_f);
	
	Bfile_DeleteFile (file_name_f);
	Bfile_CreateFile (file_name_f,
	pmroute.size * sizeof(PMPOINT) + sizeof (PMROUTE));
	file_h = Bfile_OpenFile (file_name_f,_OPENMODE_WRITE);
	
	Bfile_SeekFile (file_h,0);
	Bfile_WriteFile (file_h,&pmroute,sizeof(PMROUTE));
	
	Bfile_SeekFile (file_h,sizeof(JDFROUTE));
	Bfile_WriteFile (file_h,pmroute.set,pmroute.size * sizeof(JDFPOINT));

	Bfile_CloseFile (file_h);
	return 0;	
}

int LoadPMFile (const char *file_name_c_a)    
{
	FONTCHARACTER file_name_f[30];
	char file_name_c[30];
	int file_h;
	
	sprintf (file_name_c,"\\\\crd0\\PM\\%s",file_name_c_a);	

	PMCharToFont(file_name_c,file_name_f);

	file_h = Bfile_OpenFile (file_name_f,_OPENMODE_READ);
	if(file_h>=0){
	    //jdfroute_clr (&jdfroute);
	    Bfile_ReadFile(file_h,&pmroute,sizeof(PMROUTE),0);
	    pmroute.set = (PMPOINT*)calloc(pmroute.size,sizeof(PMPOINT));
	    Bfile_ReadFile(file_h,pmroute.set,sizeof(PMPOINT)* pmroute.size,sizeof(PMROUTE));
	    Bfile_CloseFile (file_h);
	    return 1;
	}
	return 0;
}

///////////////////////////////////////
/// ini
///////////////////////////////////////


//static FONTCHARACTER IniFile[] = { '\\','\\','f','l','s','0','\\','s','r','d','.','i','n','i' ,'\0'} ;
static FONTCHARACTER IniPMFile[] = { '\\','\\','c','r','d','0','\\','j','d','f','.','i','n','i' ,'\0'} ;


void SaveIniPMFile()
{
	int ptr = -1;
	int ln;
	Bfile_DeleteFile(IniPMFile);
	ln=strlen(setup_data.jdfroute_name);
	if(ln==0)
		return;
	if (Bfile_CreateFile(IniPMFile, ln) < 0) //Error("�����ļ�ʧ��") ;
		return;
	//	else Error("�����ļ�OK") ;
	ptr = Bfile_OpenFile(IniPMFile, _OPENMODE_WRITE) ;  // Bfile_OpenFile : ��һ�����е��ļ���
	if (ptr >= 0) {
		Bfile_WriteFile(ptr, setup_data.pmroute_name, ln) ;
		Bfile_CloseFile(ptr) ;
	}
}

int LoadLastDataPMFile()
{
	char strFileName[50]; 
	int reg_ptr = -1;
	int ln;
	if(setup_data.pmroute_name[0]==0)
	{
		reg_ptr = Bfile_OpenFile(IniPMFile, _OPENMODE_READ) ;
		if (reg_ptr >= 0) {
			ln=Bfile_GetFileSize(reg_ptr);
			if(ln>0)
			{   
				if(ln>=sizeof(setup_data.pmroute_name)-1)
				   ln=sizeof(setup_data.pmroute_name)-1;
				Bfile_ReadFile(reg_ptr, setup_data.pmroute_name,ln,0) ;
				setup_data.pmroute_name[ln]=0;
			}
			Bfile_CloseFile(reg_ptr) ;
			//PopUpWin(1);
	        //locate(7,4);Print(setup_data.JDFroute_name);
	        //GetKey(&key);
			//sprintf(strFileName,"\\\\crd0\\SRD\\%s",setup_data.JDFroute_name);
			LoadPMFile(setup_data.pmroute_name);
			return 1;
		}		
	}
	return 0;
}