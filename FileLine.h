


#include "stdio.h"
#include "string.h"
#include "stdlib.h"

//#define USE_VCDEMO

typedef unsigned char	BOOLL ;

#define TRUE	1
#define FALSE	0


#ifdef USE_VCDEMO
  typedef FILE * FHANDLE;
  #define IsFileOpenOK(fp) (fp!=NULL)
#else
  typedef int FHANDLE;
  #define IsFileOpenOK(handle) (handle>=0)
#endif


#ifdef USE_VCDEMO

//#define FONTCHARACTER               unsigned short
  #define FONTCHARACTER               char

  #define _OPENMODE_READ              0x01
  #define _OPENMODE_READ_SHARE        0x80
  #define _OPENMODE_WRITE             0x02
  #define _OPENMODE_READWRITE         0x03
  #define _OPENMODE_READWRITE_SHARE   0x83

  #define _CREATEMODE_BINARY          1
  #define _CREATEMODE_DIRECTORY       5

	FHANDLE Bfile_OpenFile(const FONTCHARACTER *filename, int mode);
	int Bfile_CreateFile(const FONTCHARACTER *filename, int size);
	
	int Bfile_SeekFile(FHANDLE HANDLE, int pos);
	int Bfile_GetFileSize(FHANDLE HANDLE);
	#define Bfile_CloseFile(handle) fclose(handle)
	
	int Bfile_ReadFile(FHANDLE HANDLE, void *buf, int size, int readpos);
	int Bfile_WriteFile(FHANDLE HANDLE, const void *buf, int size);
	int Bfile_DeleteFile(const FONTCHARACTER *filename);
#endif


//�����ļ�����,�����ڻ򲻳ɹ��򷵻�-1
int GetFileLineNum(const FONTCHARACTER * filename);
//���ļ�filename��nLineNo(0~)����,����cBuf[],�����볤��nMaxLen,OK�򷵻ض�����г���,�����ڻ򲻳ɹ��򷵻�-1
int ReadFileLine(const FONTCHARACTER * filename,int nLineNo,char cBuf[],int nMaxLen);
//д�ļ�filename��nLineNo(0~)����cBuf[],OK�򷵻�д��������(0~),���ɹ��򷵻�-1
int WriteFileLine(const FONTCHARACTER * filename,int nLineNo,char cBuf[]);
//д�ļ�filename����szSect[]=����cBuf[],OK�򷵻�д��������(0~),���ɹ��򷵻�-1,�� "A=123" ,szSect[]="A", cBuf[]="123"
int WriteIniLine(const FONTCHARACTER * filename,char szSect[],char cBuf[]);
//���ļ�filename����szSect[]=���ݵ�cBuf[],OK�򷵻ض���ʵ�ʳ���,���ɹ��򷵻�-1,�� "A=123" ,szSect[]="A", cBuf[]="123"
int ReadIniLine(const FONTCHARACTER * filename,char szSect[],char cBuf[],int nMaxLen);

