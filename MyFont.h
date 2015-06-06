#ifndef MYFONT_H_1234
#define	MYFONT_H_1234

#ifndef USE_DEMO
  #include "fxlib.h"
#endif

#include "math.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"

#ifndef uchar 
   #define uchar	unsigned char
#endif

typedef struct tagCHRDOT_INFO {
	short nChrW;			//�ַ����(����)
	short nChrH;			//�ַ��߶�(����)
	short nChrNum;			//�ַ�����
	const uchar *pChrTab;			//�ַ���ָ��
	uchar cFirstCode;		//pChrTab=NULLʱ,����Ӣ�����������ַ���
	const uchar *pChrDotData;		//�ַ���������ָ��
} CHRDOT_INFO;

extern CHRDOT_INFO stHz14x14;
extern CHRDOT_INFO stHz12x12;
extern CHRDOT_INFO stHz8x8;
extern CHRDOT_INFO stAsc6x12;

//�����巴����ʾģʽ
void SetFontReverce(BOOL bReverce);

//���ú��ֺ�Ӣ�ĵ����ֿ�,pHzDotInfo:ָ���ֵ�(û�õ�����NULL),pAscDotInfo:ָ��Ӣ�ĵ�(û�õ�����NULL)
void SetMyFont(CHRDOT_INFO *pHzDotInfo,CHRDOT_INFO *pAscDotInfo);

//��ʾ�ַ���str,������SetMyFont�趨
void DispStr(short x,short y,char str[]);
//��ʾ�ַ���str,��󳤶�Ϊlen(<0����ʵ�ʳ���),������SetMyFont�趨
void DispStrN(short x,short y,char str[],short len);

#ifdef USE_DEMO
  #define DrawDot	DispAnyDot
#else
  void DrawDot(int x,int y,int width,int height,unsigned char * pGraph);
#endif


#endif
