#ifndef USE_DEMO
  #include "headfile.h"
#endif
#include "MyFont.h"


#ifdef USE_DEMO
//�˵���ʼ��
void MenuInit();
void ListMenu();		//��ʾ�˵��������������ʾ
int DoMyMenu();			//�����˵�,����ִ�еĲ˵�������,���˳���ʱ������-1
#else
#define DelayMs		Sleep
#endif

//ȡ�൱ǰ�����в˵�(nMenuRow,nMenuCol)�����(X,Y)
short GetMenuXY(short nMenuRow,short nMenuCol,short *pMenuY);	
void DrawCenterText(short x,short y,short w,short h,char *str);
//���˵�����
void DrawMenuGrid();
//��ʾ�˵���ʾ,nPos��ָ��λ��,<0����nPromptPos
void ListMenuPrompt(int idx,short nPos);
//
void ListMenuItem(short row,short col,BOOL bSelected);
//���ҿ�ݼ���Ӧ�Ĳ˵�����
short FindKeyMenuItem(uint key);

void ListPageMenu();	//��ʾһҳ�˵�
BOOL DoMenuMoveSel(uint cCurKey);
void DrawMenuSBar();
#ifndef USE_DEMO
void ClearBox(int x1, int y1, int x2, int y2,BOOL bSet);
#endif

//������Ļ�������Ӧ�Ĳ˵���
short GetItemNo(short row,short col);

//Bdisp_DrawLineVRAM

#define MAX_LCD_W		128		//��ʾ����
#define MAX_LCD_H		64		//��ʾ����

#define MAX_MENU_ITEMN	24		//���˵�����
#define MAX_MENU_COLN	3		//���˵�����
#define MAX_MENU_PAGEROWN	3	//һ�����˵�����

#define MENU_LRGAP		0		//�˵������������������
#define MENU_FONT_H		12		//����߶�
#define MENU_SBAR_W		4		//�˵��Ҳ���������

#define MENUPROMPT_DELAYMS		600	//�˵���ʾ��ʱ�ƶ�ʱ�� ms
#ifdef USE_DEMO
  #define MAX_MENUPROMPT_DELAYN	(MENUPROMPT_DELAYMS/100)
#endif

BOOL bMenuTitle=FALSE;//TRUE;		//�в˵�����
BOOL bMenuPrompt=TRUE;		//�в˵���ʾ

short nMenuRowNum=(MAX_MENU_ITEMN+MAX_MENU_COLN-1)/MAX_MENU_COLN;	//�ܵĲ˵�����
short nMenuPageRowN=MAX_MENU_PAGEROWN;		//һ���˵�����
short nMenuTopRowNo=0;		//�����˵������
short nMenuCurItemNo=0;		//��ǰ�˵������

short nMenuCurRow=0;		//��ǰ���������(���nMenuTopRowNo)
short nMenuCurCol=0;		//��ǰ���������
	
BOOL bMenuSBar=FALSE;		//�Ƿ��й�����
short nMenuGridW,nMenuGridH;	//�˵����͸�
short nMenuX0,nMenuY0;			//�˵������Ͻ�����

const char *strMenuTitle=" �� �� �� �� ";
//�˵�������
const char *strMenuText[MAX_MENU_ITEMN]={
	"\xB7\xC5\xD1\xF9\xC7\xF8","1 \xD5\xFD\xCB\xE3","2 \xB7\xB4\xCB\xE3",   
	"3 \xC7\xC5\xC1\xBA","4 \xCB\xED\xB5\xC0","5 \xC9\xE8\xD6\xC3",    
	"\xB2\xCE\xCA\xFD\xC7\xF8", "A \xBD\xBB\xB5\xE3", "B \xCF\xDF\xD4\xAA", 
	"C \xCA\xFA\xC7\xFA", "D \xC2\xB7\xB7\xF9", "E \xBA\xE1\xC6\xC2", 
	"F \xC6\xC2\xC4\xA3", "G \xCB\xED\xC4\xA3", "H \xBD\xE1\xB9\xB9", 
	"\xB3\xC9\xB9\xFB\xC7\xF8","I \xD6\xB1\xC7\xFA", "J \xCF\xDF\xD4\xAA",
	"K \xCA\xFA\xC7\xFA","L \xD6\xF0\xD7\xAE", "M",
	"\xB9\xA4\xBE\xDF\xC7\xF8",  "N \xD5\xFD\xB7\xB4", "O \xD4\xB2\xBB\xA1", 
};
//�˵���ʾ
const char *strMenuPrompt[MAX_MENU_ITEMN]={
	"\xB7\xC5\xD1\xF9\xC7\xF8","��֪׮�ű߾�н� ������ά���� ","��֪����߳� ����׮�ű߾����ڸ� ",   
	"3 \xBE\xB4\xC7\xEB\xC6\xDA\xB4\xFD","4 \xBE\xB4\xC7\xEB\xC6\xDA\xB4\xFD","5 \xBE\xB4\xC7\xEB\xC6\xDA\xB4\xFD",    
	"\xB2\xCE\xCA\xFD\xC7\xF8", "A \xBE\xB4\xC7\xEB\xC6\xDA\xB4\xFD", "B \xBE\xB4\xC7\xEB\xC6\xDA\xB4\xFD", 
	"C \xBE\xB4\xC7\xEB\xC6\xDA\xB4\xFD", "D \xBE\xB4\xC7\xEB\xC6\xDA\xB4\xFD", "E \xBE\xB4\xC7\xEB\xC6\xDA\xB4\xFD", 
	"F \xBE\xB4\xC7\xEB\xC6\xDA\xB4\xFD", "G \xBE\xB4\xC7\xEB\xC6\xDA\xB4\xFD", "G \xBE\xB4\xC7\xEB\xC6\xDA\xB4\xFD", 
	"\xB3\xC9\xB9\xFB\xC7\xF8","I \xBE\xB4\xC7\xEB\xC6\xDA\xB4\xFD", "J \xBE\xB4\xC7\xEB\xC6\xDA\xB4\xFD",
	"K \xBE\xB4\xC7\xEB\xC6\xDA\xB4\xFD","L \xBE\xB4\xC7\xEB\xC6\xDA\xB4\xFD", "M\xBE\xB4\xC7\xEB\xC6\xDA\xB4\xFD",
	"\xB9\xA4\xBE\xDF\xC7\xF8",  "N \xD6\xB1\xCF\xDF\xCE\xDE\xCF\xDE\xD1\xD3\xB3\xA4\xD1\xD3\xBA\xF3", "O ��֪������������뾶 ����Բ����������� ", 
};
const uint cMenuKey[MAX_MENU_ITEMN]={   
	KEY_CTRL_F1,KEY_CHAR_1,KEY_CHAR_2,
	KEY_CHAR_3,KEY_CHAR_4,KEY_CHAR_5,
	KEY_CTRL_F2,KEY_CTRL_XTT,KEY_CHAR_LOG,
	KEY_CHAR_LN,KEY_CHAR_SIN,KEY_CHAR_COS, 
	KEY_CHAR_TAN,KEY_CHAR_FRAC,KEY_CTRL_FD,
	KEY_CTRL_F3,KEY_CHAR_LPAR,KEY_CHAR_RPAR,
    KEY_CHAR_COMMA,KEY_CHAR_STORE,KEY_CHAR_7,
	KEY_CTRL_F4,KEY_CHAR_8,KEY_CHAR_9,
};
short nPromptPos=0;

//�˵���ʼ��
void MenuInit()
{
	short rn;
	short w=MAX_LCD_W-2*MENU_LRGAP;
	short h=MAX_LCD_H;
	if(nMenuPageRowN>nMenuRowNum)    //��һ�������������ڲ˵��˵�����
		nMenuPageRowN=nMenuRowNum;  
	if(nMenuTopRowNo<0)   //�����˵������
		nMenuTopRowNo=0;
	nMenuCurItemNo=GetItemNo(nMenuCurRow,nMenuCurCol);
	if(nMenuCurItemNo>=MAX_MENU_ITEMN)	//�����˵�����������ǰ�˵�����ŵ����һ��
	{
		nMenuCurItemNo=MAX_MENU_ITEMN-1;   //���˵�����-1
		nMenuCurCol=nMenuCurItemNo%MAX_MENU_COLN;
		//������ʵ�nMenuCurRow(��ǰ�˵�����ţ����nMenuTopRowNo),nMenuTopRowNo(�����˵������)
		rn=nMenuCurItemNo/MAX_MENU_COLN;
		if(rn<nMenuTopRowNo||rn>=nMenuTopRowNo+nMenuPageRowN)
		{
			nMenuTopRowNo=rn;
		}
		if(nMenuTopRowNo+nMenuPageRowN>nMenuRowNum)	
			nMenuTopRowNo=nMenuRowNum-nMenuPageRowN;
		if(nMenuTopRowNo<0)
			nMenuTopRowNo=0;
		nMenuCurRow=rn-nMenuPageRowN;
	}
	bMenuSBar=nMenuRowNum>nMenuPageRowN;	//�Ƿ��й�����
	if(bMenuSBar)
		w-=(MENU_SBAR_W+1);
	nMenuGridW=(w-1)/MAX_MENU_COLN-1;
	nMenuX0=(w-((nMenuGridW+1)*MAX_MENU_COLN+1))/2;		//�����˵������X
	nMenuY0=0;
	if(bMenuTitle)
		nMenuY0+=MENU_FONT_H;
	if(bMenuPrompt)
	{
		h-=MENU_FONT_H; 
		if(!bMenuTitle) h--;
	}
	h=h-nMenuY0;
	nMenuGridH=(h-1)/nMenuPageRowN-1;
	nMenuY0=nMenuY0+(h-((nMenuGridH+1)*nMenuPageRowN+1))/2;	//�����˵�������Y
}
//ȡ�൱ǰ�����в˵�(nMenuRow,nMenuCol)�����(X,Y)
short GetMenuXY(short nMenuRow,short nMenuCol,short *pMenuY)	
{
	if(pMenuY)
		*pMenuY=nMenuY0+nMenuRow*(nMenuGridH+1)+1;
	return nMenuX0+nMenuCol*(nMenuGridW+1)+1;
}
//��ʾ�ı�str[]������(x,y,w,h)�м�
void DrawCenterText(short x,short y,short w,short h,char *str)
{
	int ln=strlen(str);
	x=x+(w-ln*MENU_FONT_H/2)/2;
	y=y+(h-MENU_FONT_H)/2;
	DispStr(x,y,str);
}
//��ʾ�˵������������ʾ
void ListMenu()
{
	SetMyFont(&stHz12x12,&stAsc6x12);
	if(bMenuTitle)
		DrawCenterText(0,0,MAX_LCD_W,MENU_FONT_H,(char *)strMenuTitle);
//	PrintMsgInfo("%d,%d,%s",nMenuGridW,nMenuGridH,strMenuTitle);
	ListPageMenu();
	//���˵�����
	DrawMenuGrid();

	ListMenuPrompt(nMenuCurItemNo,0);
}
//��ʾ�˵���ʾ,nPos��ָ��λ��,<0����nPromptPos
void ListMenuPrompt(int idx,short nPos)
{
	static int nDelayCount=0;
	short y,ln,cn,x;
	char *p;
	uchar c1,c2;
	if(bMenuPrompt)
	{
		if(nPos>=0)
		{
			nPromptPos=nPos;
#ifdef USE_DEMO
			nDelayCount=MAX_MENUPROMPT_DELAYN;
#endif
		}
		y=MAX_LCD_H-MENU_FONT_H;
		p=(char *)strMenuPrompt[idx];
		if(p==NULL)
			return;
		cn=MAX_LCD_W/(MENU_FONT_H/2);
		ln=strlen(p);
		if(ln<=cn)
		{
			if(nPos>=0)
			{
				ClearBox(0,y,MAX_LCD_W,y+MENU_FONT_H,FALSE);
				DispStrN(0,y,p,ln);
			}
			return;
		}
#ifdef USE_DEMO
		//��ʱ����
		DelayMs(100);
		nDelayCount++;
		if(nDelayCount<MAX_MENUPROMPT_DELAYN)	//û�������򷵻�
			return;
		nDelayCount=0;
#endif
		//��ʾ
		if(nPromptPos>=ln)
			nPromptPos=0;
		p+=nPromptPos;
		ln=strlen(p);
		if(ln>cn)
			ln=cn;
		DispStrN(0,y,p,ln);
		c1=p[0]; c2=p[1];
		nPromptPos++;
		if(c1>=128 && c2>=128)
			nPromptPos++;
		x=ln*MENU_FONT_H/2;
		if(x<MAX_LCD_W)
		{
			//ClearBox(x,y,MAX_LCD_W,y+MENU_FONT_H,FALSE);
			for(cn=0;cn<(MAX_LCD_W-x)/MENU_FONT_H*2+1;cn++) 
				DispStr(x,y,"  "); x+=(MENU_FONT_H/2);
		}
	}
}

void ListPageMenu()
{
	short row,col,idx;
	SetMyFont(&stHz12x12,&stAsc6x12);
	for (row=0;row<nMenuPageRowN;row++)
	{
		for (col=0;col<MAX_MENU_COLN;col++)
		{
			idx=GetItemNo(row,col); // (nMenuTopRowNo+row)*MAX_MENU_COLN+col;
			ListMenuItem(row,col,idx==nMenuCurItemNo);
		}
	}
	if(bMenuSBar)
		DrawMenuSBar();
}
//��ʾĳһ��˵�,bSelected:�Ƿ�ѡ��
void ListMenuItem(short row,short col,BOOL bSelected)
{
	short idx;
	short x,y;
	char *p=NULL;
//	SetMyFont(&stHz12x12,&stAsc6x12);

	idx=GetItemNo(row,col); //(nMenuTopRowNo+row)*MAX_MENU_COLN+col;
	x=GetMenuXY(row,col,&y);
	if(idx<MAX_MENU_ITEMN)
		p=(char *)strMenuText[idx];
	if(p==NULL)
	{
		ClearBox(x,y,x+nMenuGridW,y+nMenuGridH,FALSE);
		return;
	}
	if(bSelected) //idx==nMenuCurItemNo)
	{
		ClearBox(x,y,x+nMenuGridW,y+nMenuGridH,TRUE);
		SetFontReverce(TRUE);
	}
	else
	{
		ClearBox(x,y,x+nMenuGridW,y+nMenuGridH,FALSE);
	}
	if(p)
		DrawCenterText(x,y,nMenuGridW,nMenuGridH,p);
	if(bSelected) //idx==nMenuCurItemNo)
		SetFontReverce(FALSE);
}
void DrawMenuSBar()
{
	short x1,y1,x2,y2,h,n;
	x1=GetMenuXY(0,0,&y1);
	x1=x1+MAX_MENU_COLN*(nMenuGridW+1);
	x2=x1+MENU_SBAR_W;
	y1--;
	y2=y1+nMenuPageRowN*(nMenuGridH+1);
#ifdef USE_DEMO
	DrawHLine(x1,y1,x2-x1,TRUE);
	DrawHLine(x1,y2,x2-x1+1,TRUE);
	DrawVLine(x2,y1,y2-y1,TRUE);
#else
	Bdisp_DrawLineVRAM(x1,y1,x2,y1);
	Bdisp_DrawLineVRAM(x1,y2,x2,y2);
	Bdisp_DrawLineVRAM(x2,y1,x2,y2);
#endif
	ClearBox(x1,y1+1,x2,y2,FALSE);
	y1++; y2--;
	h=y2-y1+1;
	n=h*nMenuPageRowN/nMenuRowNum;
	if(n>h)
		n=h;
	if(nMenuRowNum<=nMenuPageRowN)
		return;
	y1=y1+(h-n)*nMenuTopRowNo/(nMenuRowNum-nMenuPageRowN);
	ClearBox(x1,y1,x2,y1+n,TRUE);
//	nMenuTopRowNo+nMenuPageRowN
}
//���˵�����
void DrawMenuGrid()
{
	short x0,y0,ln,i,m;
	x0=GetMenuXY(0,0,&y0);
	x0--; y0--;	//�˵�����λ��
	//����ˮƽ��
	ln=MAX_MENU_COLN*(nMenuGridW+1)+1;
	for (i=0;i<=nMenuPageRowN;i++)
	{
		m=y0+i*(nMenuGridH+1);
#ifdef USE_DEMO
		DrawHLine(x0,m,ln,TRUE);
#else
		Bdisp_DrawLineVRAM(x0,m,x0+ln-1,m);
#endif
	}
	//����ֱ��
	ln=nMenuPageRowN*(nMenuGridH+1)+1;
	for (i=0;i<=MAX_MENU_COLN;i++)
	{
		m=x0+i*(nMenuGridW+1);
#ifdef USE_DEMO
		DrawVLine(m,y0,ln,TRUE);
#else
		Bdisp_DrawLineVRAM(m,y0,m,y0+ln-1);
#endif
	}
}
#ifndef USE_DEMO
void ClearBox(int x1, int y1, int x2, int y2,BOOL bSet)
{
	DISPBOX stDB;
	stDB.left=x1;
	stDB.top=y1;
	stDB.right=x2-1;
	stDB.bottom=y2-1;
	Bdisp_AreaClr_VRAM(&stDB);
	if(bSet)
	{
		Bdisp_AreaReverseVRAM(x1, y1, x2-1, y2-1);
	}
}
#endif

void ListPromptTimer()
{
	ListMenuPrompt(nMenuCurItemNo,-1);
}
int DoMyMenu()
{
	short idx=nMenuCurItemNo; //cCurMMenuIdx;
	BOOL bb=FALSE; //TRUE;
#ifndef USE_DEMO
	uint cCurKey=0;
	SetTimer(1,MENUPROMPT_DELAYMS,ListPromptTimer);
#endif
	while(1)
	{
		if(bb)
		{
			ListPageMenu(); bb=FALSE; //PrintMessage("%d",cCurKey);
		}
		else
		{
#ifdef USE_DEMO
			ListMenuPrompt(nMenuCurItemNo,-1);
#endif
		}
#ifndef USE_DEMO
		cCurKey=WaitKey();
#else
		DelayMs(80);//Sleep(50);
		keyboard();
		if(cKeyNo==KEY_UNKNOW)
			continue;
		cCurKey=cKeyNo; cKeyNo=KEY_UNKNOW;
#endif
		if(cCurKey==KEY_CTRL_EXE)
			break;
		if(cCurKey==KEY_CTRL_EXIT)	//ֱ���˳�
		{
			KillTimer(1);
			return -1;
		}
		if(cCurKey==KEY_CTRL_LEFT||cCurKey==KEY_CTRL_RIGHT||cCurKey==KEY_CTRL_UP||cCurKey==KEY_CTRL_DOWN)
		{
			bb=DoMenuMoveSel(cCurKey);
		}
		else
		{
			short k=FindKeyMenuItem(cCurKey);
			if(k>=0)
			{
				nMenuCurItemNo=k;
				break;
			}
		}
		if(idx!=nMenuCurItemNo)
		{
			ListMenuPrompt(nMenuCurItemNo,0);
			idx=nMenuCurItemNo;
		}
	}
	//�����ͣ��ѡ�������
	if(cCurKey!=KEY_CTRL_EXE)
	{
		ListMenuItem(nMenuCurRow,nMenuCurCol,FALSE);
		nMenuCurCol=nMenuCurItemNo%MAX_MENU_COLN; 
		if(nMenuCurItemNo>=GetItemNo(0,0)&&nMenuCurItemNo<=GetItemNo(nMenuPageRowN-1,MAX_MENU_COLN-1))	//�ڵ�ǰ���˵�����
		{
//			ListMenuItem(row,col,FALSE);
			nMenuCurRow=nMenuCurItemNo/MAX_MENU_COLN-nMenuTopRowNo;
		}
		else
		{
			nMenuCurRow=nMenuCurItemNo/MAX_MENU_COLN;
			if(nMenuCurRow<=nMenuTopRowNo)	//�ڵ�ǰ��ǰ
			{
				nMenuCurRow=0;
				nMenuTopRowNo=nMenuCurItemNo/MAX_MENU_COLN;
			}
			else	//�ں�
			{
				nMenuCurRow=0;
				nMenuTopRowNo=nMenuCurItemNo/MAX_MENU_COLN;
				if(nMenuTopRowNo+nMenuPageRowN>nMenuRowNum)
				{
					nMenuTopRowNo=nMenuRowNum-nMenuPageRowN;
					nMenuCurRow=nMenuCurItemNo/MAX_MENU_COLN-nMenuTopRowNo;
				}
			}
		}
		ListMenuItem(nMenuCurRow,nMenuCurCol,TRUE);
		ListMenuPrompt(nMenuCurItemNo,0);
	}
#ifdef USE_DEMO
	ClearLcd(0);
	DrawCenterText(0,0,MAX_LCD_W,MAX_LCD_H,(char *)strMenuText[nMenuCurItemNo]);
#else
	KillTimer(1);
#endif
	return nMenuCurItemNo;
}
short GetItemNo(short row,short col)
{
	return (nMenuTopRowNo+row)*MAX_MENU_COLN+col;
}
BOOL DoMenuMoveSel(uint cCurKey)
{
//	BOOL bb=FALSE;
	short row,col,top;
	int idx=nMenuCurItemNo;
	row=nMenuCurRow; col=nMenuCurCol;
	top=nMenuTopRowNo;

	switch(cCurKey){
	case KEY_CTRL_UP:
		if(nMenuCurRow>0)
		{	nMenuCurRow--; } //bb=TRUE; 
		else if(nMenuCurRow==0 && nMenuTopRowNo>0)
		{
			nMenuTopRowNo--; //bb=TRUE;
		}
		else if(nMenuCurRow==0)
		{
			nMenuTopRowNo=nMenuRowNum-nMenuPageRowN;
			if(nMenuTopRowNo<0)
				nMenuTopRowNo=0;
			nMenuCurRow=nMenuPageRowN-1;
			while(nMenuCurRow>0 && GetItemNo(nMenuCurRow,nMenuCurCol)>=MAX_MENU_ITEMN)
			{
				nMenuCurRow--; 
			}
		}
		break;
	case KEY_CTRL_DOWN:
// 		if(GetItemNo(nMenuCurRow+1,nMenuCurCol)>=MAX_MENU_ITEMN)	//�����Ƴ����ܲ˵�������������
// 			break;
		if(GetItemNo(nMenuCurRow+1,nMenuCurCol)>=MAX_MENU_ITEMN)	//�����Ƴ����ܲ˵��������򵽵�1ҳ��
		{
			nMenuCurRow=0; nMenuTopRowNo=0;
			break;
		}
		if(nMenuCurRow>=nMenuPageRowN-1)	//���ײ�ʱ������������1
		{
			nMenuCurRow=nMenuPageRowN-1;
			nMenuTopRowNo++;
		}
		else
			nMenuCurRow++;
		//bb=TRUE;
		break;
	case KEY_CTRL_LEFT:
		if(nMenuCurCol>0)
		{	nMenuCurCol--;}
		else if(nMenuCurCol==0)
		{
			nMenuCurCol=MAX_MENU_COLN-1;
			if(GetItemNo(nMenuCurRow,nMenuCurCol)>=MAX_MENU_ITEMN)	//�����Ƴ����ܲ˵�����
				nMenuCurCol=(MAX_MENU_ITEMN%MAX_MENU_COLN)-1;
		}
		if(nMenuCurCol<0)
			nMenuCurCol;
//		bb=TRUE; 
		break;
	case KEY_CTRL_RIGHT:
		if(GetItemNo(nMenuCurRow,nMenuCurCol+1)>=MAX_MENU_ITEMN)	//�����Ƴ����ܲ˵�����
			nMenuCurCol=0;
		else
			nMenuCurCol=(nMenuCurCol+1)%MAX_MENU_COLN;
		break;
	}
	nMenuCurItemNo=GetItemNo(nMenuCurRow,nMenuCurCol);
	
	if(nMenuTopRowNo!=top)	//��ҳ idx!=nMenuCurItemNo && 
		return TRUE;
	if(idx!=nMenuCurItemNo)
	{
		ListMenuItem(row,col,FALSE);
		ListMenuItem(nMenuCurRow,nMenuCurCol,TRUE);
	}
	return FALSE;
}
//���ҿ�ݼ���Ӧ�Ĳ˵�����
short FindKeyMenuItem(uint key)
{
	int i;
	for(i=0;i<MAX_MENU_ITEMN;i++)
	{
		if(key==cMenuKey[i])
			return i;
	}
	return -1;
}