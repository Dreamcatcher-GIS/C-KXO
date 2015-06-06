#include <fxlib.h>
//#include "stdafx.h"
#include "fileLine.h"

#ifdef USE_VCDEMO

int Bfile_DeleteFile(const FONTCHARACTER *filename)
{
	return remove(filename);
}
int Bfile_ReadFile(FHANDLE HANDLE, void *buf, int size, int readpos)
{
	fseek(HANDLE,readpos,SEEK_SET);
	return fread(buf,1,size,HANDLE);
}
int Bfile_WriteFile(FHANDLE HANDLE, const void *buf, int size)
{
	return fwrite(buf,1,size,HANDLE);
}

int Bfile_GetFileSize(FHANDLE HANDLE)
{
	int size;
	int old;
	old=ftell (HANDLE);
	fseek (HANDLE, 0, SEEK_END);   // non-portable
    size=ftell (HANDLE);
	fseek (HANDLE, old, SEEK_SET);   // non-portable
	return size;
}
int Bfile_SeekFile(FHANDLE HANDLE, int pos)
{
	return fseek(HANDLE,pos,SEEK_SET);
}

FHANDLE Bfile_OpenFile(const FONTCHARACTER *filename, int mode)
{
	
	FHANDLE pf;
	if(mode==_OPENMODE_READ)
		pf=fopen(filename,"rb");
	else if(mode==_OPENMODE_WRITE)
		pf=fopen(filename,"wb");
	else //if(mode==_OPENMODE_READWRITE)
		pf=fopen(filename,"rb+");
	return pf;
}
int Bfile_CreateFile(const FONTCHARACTER *filename, int size)
{
	char *pbuf;
	FHANDLE fp;
	if(size<0)
		return -1;
	fp=fopen(filename,"w");
	if(fp==NULL)
		return -1;
	if(size>0)
	{
		pbuf=(char *)malloc(size);
		memset(pbuf,0,size);
		fwrite(pbuf,1,size,fp);
		free(pbuf);
	}
	fclose(fp);
	return 0;
}

#endif


#define LINEBUFFER_LEN	512

//�����ļ�����,�����ڻ򲻳ɹ��򷵻�-1
int GetFileLineNum(const FONTCHARACTER * filename)
{
	char *pbuf;
	int len;
	int rn;
	int mln,sln;
	int fpos;
	char cLastChr;
	int nCurPos,nLinePos;
	int i;
	FHANDLE	hAndle;

	hAndle=Bfile_OpenFile(filename,_OPENMODE_READ);
	if(!IsFileOpenOK(hAndle))
		return -1;
	len=Bfile_GetFileSize(hAndle);
	if(len==0)
	{
		Bfile_CloseFile(hAndle);
		return 0;
	}
	pbuf=(char *)malloc(LINEBUFFER_LEN);
	if(pbuf==NULL)
	{
		Bfile_CloseFile(hAndle);
		return -1;
	}

	rn=0; fpos=0;
	sln=len;
	nCurPos=0; nLinePos=0;
	cLastChr=0;

	for(;sln>0;sln-=LINEBUFFER_LEN,fpos+=LINEBUFFER_LEN)
	{
		mln=sln;
		if(sln>=LINEBUFFER_LEN)
			mln=LINEBUFFER_LEN;
		Bfile_ReadFile(hAndle,pbuf,mln,fpos);	//��һ�����ݵ�����pbuf

		for(i=0;i<mln;i++,nCurPos++)
		{
			if(cLastChr==0x0D && pbuf[i]==0x0A)
			{
				cLastChr=0x0C;	//��0d0a��־
			}
			else
			{
				if(cLastChr==0x0D||cLastChr==0x0A||cLastChr==0x0C)	//0d0a,0d,0a �����Ķ���һ��
				{
					nLinePos=nCurPos;	//��һ�е�λ��
					rn++;				//��1��
				}
				cLastChr=0;				//���־
				if(pbuf[i]==0x0D||pbuf[i]==0x0A)
					cLastChr=pbuf[i];	//�ñ�־
			}
		}
	}
	if(nCurPos!=0)	//���һ�м���
	{
//		nLinePos=nCurPos;	//��һ�е�λ��
		rn++;				//��1��
	}
	free(pbuf);
	return rn;
}

//���ļ�filename��nLineNo(0~)����,����cBuf[],�����볤��nMaxLen,OK�򷵻ض�����г���,�����ڻ򲻳ɹ��򷵻�-1
int ReadFileLine(const FONTCHARACTER * filename,int nLineNo,char cBuf[],int nMaxLen)
{
	char *pbuf;
	int len;
	int rn;
	int mln,sln;
	int fpos;
	char cLastChr;
	int nCurPos,nLinePos;
	int i;
	FHANDLE	hAndle;
	BOOLL bFind=FALSE;

	if(nLineNo<0 || cBuf==NULL || nMaxLen<=0)
		return -1;

	cBuf[0]=0;
	hAndle=Bfile_OpenFile(filename,_OPENMODE_READ);
	if(!IsFileOpenOK(hAndle))
		return -1;
	len=Bfile_GetFileSize(hAndle);
	if(len==0)
	{
		Bfile_CloseFile(hAndle);
		return 0;
	}
	pbuf=(char *)malloc(LINEBUFFER_LEN);
	if(pbuf==NULL)
	{
		Bfile_CloseFile(hAndle);
		return -1;
	}
	
	rn=0; fpos=0;
	sln=len;
	nCurPos=0; nLinePos=0;
	cLastChr=0;

	for(;sln>0;sln-=LINEBUFFER_LEN,fpos+=LINEBUFFER_LEN)
	{
		mln=sln;
		if(sln>=LINEBUFFER_LEN)
			mln=LINEBUFFER_LEN;
		Bfile_ReadFile(hAndle,pbuf,mln,fpos);	//��һ�����ݵ�����pbuf
		
		for(i=0;i<mln;i++,nCurPos++)
		{
			if(cLastChr==0x0D && pbuf[i]==0x0A)
			{
				cLastChr=0x0C;	//��0d0a��־
			}
			else
			{
				if(cLastChr==0x0D||cLastChr==0x0A||cLastChr==0x0C)	//0d0a,0d,0a �����Ķ���һ��
				{
					if(rn==nLineNo)
						bFind=TRUE;
					else
						nLinePos=nCurPos;	//��һ�е�λ��
					rn++;				//��1��
				}
				cLastChr=0;				//���־
				if(pbuf[i]==0x0D||pbuf[i]==0x0A)
					cLastChr=pbuf[i];	//�ñ�־
			}
			if(bFind)
				break;
		}
		if(bFind)
			break;
	}
	if(nCurPos!=0 && nCurPos==len)	//���һ�м���
	{
//		nLinePos=nCurPos;	//��һ�е�λ��
		if(rn==nLineNo)
			bFind=TRUE;
		rn++;				//��1��
	}
	free(pbuf);
	mln=-1;
	if(bFind)
	{
		mln=nCurPos-nLinePos;
		if(mln>nMaxLen)
			mln=nMaxLen;
		Bfile_ReadFile(hAndle,cBuf,mln,nLinePos);	//��һ�����ݵ�����cbuf
		cBuf[mln]=0;
	}
	Bfile_CloseFile(hAndle);
	return mln;
}
//д�ļ�filename��nLineNo(0~)����cBuf[],OK�򷵻�д��������(0~),���ɹ��򷵻�-1
int WriteFileLine(const FONTCHARACTER * filename,int nLineNo,char cBuf[])
{
	char *pbuf;
	int len;
	int rn;
	int mln;
	int i;
	int ln; 
	char lfcr[3]={0x0d,0x0a,0x0};
	BOOLL bCreateNew=FALSE;
	FHANDLE	hAndle;
	char *pp,*plast;
	char *pLinePos;
	int nLineLen;
	BOOLL bEquLenLine;
	int m,npos;

	if(nLineNo<0 || cBuf==NULL)
		return -1;
	ln=strlen(cBuf);

	mln=0;
	if(ln==0 || cBuf[ln-1]!=0x0A)	//��0d0aת�з�
		mln=2;

	hAndle=Bfile_OpenFile(filename,_OPENMODE_READ); //WRITE
	if(!IsFileOpenOK(hAndle))	//����������
	{
		bCreateNew=TRUE;
	}
	else //if(!bCreateNew)
	{
		//����������ڴ洦��
		len=Bfile_GetFileSize(hAndle);
		if(len==0)	//���ļ�
		{
			Bfile_CloseFile(hAndle);
			Bfile_DeleteFile(filename);
			bCreateNew=TRUE;
		}
	}
	if(bCreateNew)	//�������ļ�
	{
		if(Bfile_CreateFile(filename,ln+mln)<0)	//����һ���ļ�����ʧ���򷵻�
			return -1;
		hAndle=Bfile_OpenFile(filename,_OPENMODE_WRITE);
		if(!IsFileOpenOK(hAndle))	//�������򷵻�
			return -1;
		Bfile_SeekFile(hAndle,0);
		Bfile_WriteFile(hAndle,cBuf,ln);
		if(mln==2)
			Bfile_WriteFile(hAndle,lfcr,mln);
		Bfile_CloseFile(hAndle);
		return 0;
	}
	//�ļ����ڣ�����봦���ٽ�����д
	pbuf=(char *)malloc(len+ln+mln+2);	//���ļ����Ⱥ�Ҫд����г����ܳ������ڴ�
	if(pbuf==NULL)
	{
		Bfile_CloseFile(hAndle);
		return -1;
	}
	Bfile_ReadFile(hAndle,pbuf,len,0);

	rn=0;
	pp=pbuf; plast=pbuf;
	bEquLenLine=FALSE;
	while(1)
	{
		if(*pp==0x0d||*pp==0x0a)
		{
			if(*pp==0x0d && pp+1<pbuf+len && pp[1]==0x0a)	//0d,0a ��һ��
			{
				pp++;
			}
			if(rn==nLineNo)
			{
				pLinePos=plast;
				nLineLen=pp+1-plast;
				if(nLineLen==ln+mln)	//�ȳ�����ֱ��д����
				{
					bEquLenLine=TRUE; break;
				}
			}
			plast=pp+1;		//��һ�е�λ��
			rn++;	//0d0a,0d,0a �����Ķ���һ��
		}
		pp++;
		if(pp>=pbuf+len)
			break;
	}
	if(plast<pbuf+len && !bEquLenLine)	//���û���з�����һ��
	{
		if(rn==nLineNo)
		{
			pLinePos=plast;
			nLineLen=pbuf+len-plast;
			if(nLineLen==ln+mln)	//�ȳ�����ֱ��д����
			{
				bEquLenLine=TRUE;
			}
		}
		rn++;
	}
	if(bEquLenLine)
	{
	Bfile_SeekFile(hAndle,pLinePos-pbuf);
		Bfile_WriteFile(hAndle,cBuf,ln);
		if(mln==2)
			Bfile_WriteFile(hAndle,lfcr,2);
		Bfile_CloseFile(hAndle);
		free(pbuf);
		return nLineNo; 
	}
	Bfile_CloseFile(hAndle);
	Bfile_DeleteFile(filename);
	if(nLineNo>=rn)	//������
	{
		if(pbuf[len-1]!=0x0a)
		{
			pbuf[len++]=0x0d; pbuf[len++]=0x0a;
		}
		npos=len; len=len+ln+mln;
		nLineNo=rn;
	}
	else
	{
		npos=pLinePos-pbuf+nLineLen;
		m=ln+mln-nLineLen;
		if(m>=0)	//����
		{
			for (i=len+m-1; m!=0 && i>=npos;i--)
			{
				pbuf[i]=pbuf[i-m];
			}
			len+=m;
		}
		else
		{
			m=-m;
			for (i=npos;i<len;i++)
			{
				pbuf[i-m]=pbuf[i];
			}
			len-=m;
		}
		if(pbuf[len-1]!=0x0a)	//���ûת�з��������
		{
			pbuf[len++]=0x0d; pbuf[len++]=0x0a;
		}
		npos=pLinePos-pbuf;
	}
	for (i=0;i<ln;i++)
	{
		pbuf[npos++]=cBuf[i];
	}
	if(mln==2)
	{
		pbuf[npos++]=0x0d; pbuf[npos++]=0x0a;
	}
	if(Bfile_CreateFile(filename,len)<0)
	{
		free(pbuf);
		return -1;
	}
	hAndle=Bfile_OpenFile(filename,_OPENMODE_WRITE);
	if(!IsFileOpenOK(hAndle))	//�������򷵻�
	{
		free(pbuf);
		return -1;
	}
	Bfile_SeekFile(hAndle,0);
	Bfile_WriteFile(hAndle,pbuf,len);
	Bfile_CloseFile(hAndle);
	free(pbuf);
	return nLineNo;
}

//д�ļ�filename����szSect[]=����cBuf[],OK�򷵻�д��������(0~),���ɹ��򷵻�-1,�� "A=123" ,szSect[]="A", cBuf[]="123"
int WriteIniLine(const FONTCHARACTER * filename,char szSect[],char cBuf[])
{
	char *pbuf;
	int len;
	int rn;
	int mln;//,sln;
//	int fpos;
//	char cLastChr;
	int wlen;
	int i;
	BOOLL bCreateNew;
	char equchr[]="=";
	char lfcr[3]={0x0d,0x0a,0x0};
	FHANDLE	hAndle;
	int nLineNo=-1;
	char *pp,*plast;
	char *pLinePos;
	int nLineLen;
	BOOLL bEquLenLine;
	int m,npos;
	int l1,ln;

	if(filename==NULL || szSect==NULL || cBuf==NULL)
		return -1;

	l1=strlen(szSect); ln=strlen(cBuf);
	mln=0;
	if(ln==0 || cBuf[ln-1]!=0x0A)	//��0d0aת�з�
		mln=2;
	wlen=l1+1+ln+mln;	//Ҫд����г���

	bCreateNew=FALSE;

	hAndle=Bfile_OpenFile(filename,_OPENMODE_READ);//WRITE);
	if(!IsFileOpenOK(hAndle))	//����������
	{
		bCreateNew=TRUE;
	}
	else //if(!bCreateNew)
	{
		//����������ڴ洦��
		len=Bfile_GetFileSize(hAndle);
		if(len==0)	//���ļ�
		{
			Bfile_CloseFile(hAndle);
			Bfile_DeleteFile(filename);
			bCreateNew=TRUE;
		}
	}
	if(bCreateNew)	//�������ļ�
	{
		if(Bfile_CreateFile(filename,wlen)<0)	//����һ���ļ�����ʧ���򷵻�
			return -1;
		hAndle=Bfile_OpenFile(filename,_OPENMODE_WRITE);
		if(!IsFileOpenOK(hAndle))	//�������򷵻�
			return -1;
		Bfile_SeekFile(hAndle,0);
		Bfile_WriteFile(hAndle,szSect,l1);
		Bfile_WriteFile(hAndle,equchr,1);	//"="
		Bfile_WriteFile(hAndle,cBuf,ln);
		if(mln==2)
			Bfile_WriteFile(hAndle,lfcr,mln);
		Bfile_CloseFile(hAndle);
		return 0;
	}
	//�ļ����ڣ�����봦���ٽ�����д
	pbuf=(char *)malloc(len+wlen+2);	//���ļ����Ⱥ�Ҫд����г����ܳ������ڴ�
	if(pbuf==NULL)
	{
		Bfile_CloseFile(hAndle);
		return -1;
	}
	Bfile_ReadFile(hAndle,pbuf,len,0);

	rn=0;
	pp=pbuf; plast=pbuf;
	bEquLenLine=FALSE;
	while(1)
	{
		if(*pp==0x0d||*pp==0x0a)
		{
			if(*pp==0x0d && pp+1<pbuf+len && pp[1]==0x0a)	//0d,0a ��һ��
			{
				pp++;
			}
			if(pp+1-plast>=l1+1 && strncmp(szSect,plast,l1)==0 && plast[l1]=='=') //rn==nLineNo)
			{
				nLineNo=rn;
				pLinePos=plast;
				nLineLen=pp+1-plast;
				if(nLineLen==wlen)	//�ȳ�����ֱ��д����
				{
					bEquLenLine=TRUE; break;
				}
			}
			plast=pp+1;		//��һ�е�λ��
			rn++;	//0d0a,0d,0a �����Ķ���һ��
		}
		pp++;
		if(pp>=pbuf+len)
			break;
	}
	if(plast<pbuf+len && !bEquLenLine)	//���û���з�����һ��
	{
		//if(rn==nLineNo)
		if(pp+1-plast>=l1+1 && strncmp(szSect,plast,l1)==0 && plast[l1]=='=') //rn==nLineNo)
		{
			nLineNo=rn;
			pLinePos=plast;
			nLineLen=pbuf+len-plast;
			if(nLineLen==wlen)	//�ȳ�����ֱ��д����
			{
				bEquLenLine=TRUE;
			}
		}
		rn++;
	}
	if(bEquLenLine)
	{
/*		Bfile_SeekFile(hAndle,pLinePos-pbuf);
		Bfile_WriteFile(hAndle,szSect,l1);
		Bfile_WriteFile(hAndle,equchr,1);	//"="
		Bfile_WriteFile(hAndle,cBuf,ln);
		if(mln==2)
			Bfile_WriteFile(hAndle,lfcr,2);
		Bfile_CloseFile(hAndle);
		free(pbuf);
		return nLineNo;
*/
	}
	Bfile_CloseFile(hAndle);
	Bfile_DeleteFile(filename);
	if(nLineNo==-1)	//û�ҵ���Ҳ��������
	{
		if(pbuf[len-1]!=0x0a)
		{
			pbuf[len++]=0x0d; pbuf[len++]=0x0a;
		}
		npos=len; len=len+wlen;
		nLineNo=rn;
	}
	else
	{
		npos=pLinePos-pbuf+nLineLen;
		m=wlen-nLineLen;
		if(m>=0)	//����
		{
			for (i=len+m-1; m!=0 && i>=npos;i--)
			{
				pbuf[i]=pbuf[i-m];
			}
			len+=m;
		}
		else
		{
			m=-m;
			for (i=npos;i<len;i++)
			{
				pbuf[i-m]=pbuf[i];
			}
			len-=m;
		}
		npos=pLinePos-pbuf;
	}
	for (i=0;i<l1;i++)
	{
		pbuf[npos++]=szSect[i];
	}
	pbuf[npos++]=equchr[0];
	for (i=0;i<ln;i++)
	{
		pbuf[npos++]=cBuf[i];
	}
	if(mln==2)
	{
		pbuf[npos++]=0x0d; pbuf[npos++]=0x0a;
	}
	if(len>0 && pbuf[len-1]!=0x0a)	//���ûת�з��������
	{
		pbuf[len++]=0x0d; pbuf[len++]=0x0a;
		// 			CString ss;	ss.Format("%d,%d",len,m); MessageBox(NULL,ss,NULL,MB_OK);
	}

	if(Bfile_CreateFile(filename,len)<0)
	{
		free(pbuf);
		return -1;
	}
	hAndle=Bfile_OpenFile(filename,_OPENMODE_WRITE);
	if(!IsFileOpenOK(hAndle))	//�������򷵻�
	{
		free(pbuf);
		return -1;
	}
	Bfile_SeekFile(hAndle,0);
	Bfile_WriteFile(hAndle,pbuf,len);
	Bfile_CloseFile(hAndle);
	free(pbuf);
	return nLineNo;
}

//���ļ�filename����szSect[]=���ݵ�cBuf[],OK�򷵻ض���ʵ�ʳ���,���ɹ��򷵻�-1,�� "A=123" ,szSect[]="A", cBuf[]="123"
int ReadIniLine(const FONTCHARACTER * filename,char szSect[],char cBuf[],int nMaxLen)
{
	char *pbuf;
	char *pp,*plast;
	int i,len;
	int rn=0;
	FHANDLE	hAndle;

	int l1;
	char equchr[]="=";
	BOOLL bFindLine=FALSE;
	int nLineNo=-1;
	int nLineLen;
	char *pLinePos=NULL;

	if(filename==NULL || szSect==NULL || cBuf==NULL|| nMaxLen<=0)
		return -1;

	hAndle=Bfile_OpenFile(filename,_OPENMODE_READ);
	if(!IsFileOpenOK(hAndle))
		return -1;

	l1=strlen(szSect);
	cBuf[0]=0;
	len=Bfile_GetFileSize(hAndle);
	if(len==0)
	{
		Bfile_CloseFile(hAndle);
		return 0;
	}
	pbuf=(char *)malloc(len);
	if(pbuf==NULL)
	{
		Bfile_CloseFile(hAndle);
		return -1;
	}
	Bfile_ReadFile(hAndle,pbuf,len,0);

	pp=pbuf; plast=pbuf;
	while(1)
	{
		if(*pp==0x0d||*pp==0x0a)
		{
			if(*pp==0x0d && pp+1<pbuf+len && pp[1]==0x0a)	//0d,0a ��һ��
			{
				pp++;
			}
			if(pp+1-plast>=l1+1 && strncmp(szSect,plast,l1)==0 && plast[l1]=='=') //rn==nLineNo)
			{
				nLineNo=rn;
				pLinePos=plast;
				nLineLen=pp+1-plast;
				bFindLine=TRUE; break;
			}
			rn++;	//0d0a,0d,0a �����Ķ���һ��
			plast=pp+1;		//��һ�е�λ��
		}
		pp++;
		if(pp>=pbuf+len)
			break;
	}

	if(plast<pbuf+len && !bFindLine)	//���û���з�����һ��
	{
		if(pp+1-plast>=l1+1 && strncmp(szSect,plast,l1)==0 && plast[l1]=='=') //rn==nLineNo)
		{
			nLineNo=rn;
			pLinePos=plast;
			nLineLen=pp+1-plast;
			bFindLine=TRUE; 
		}
		rn++;
	}
	if(bFindLine)
	{
		if(nLineLen>=1 && (pLinePos[nLineLen-1]==0x0a||pLinePos[nLineLen-1]==0x0d))
			nLineLen--;
		if(nLineLen>=1 && (pLinePos[nLineLen-1]==0x0a||pLinePos[nLineLen-1]==0x0d))
			nLineLen--;
		nLineLen-=l1; nLineLen--;
		if(nMaxLen>nLineLen)
			nMaxLen=nLineLen;
		if(nMaxLen<0)
			nMaxLen=0;
		for (i=0;i<nMaxLen;i++)
		{
			cBuf[i]=pLinePos[l1+1+i];
		}
		cBuf[nMaxLen]=0;
		return nMaxLen;
	}
	else
		nMaxLen=-1;
	free(pbuf);
	Bfile_CloseFile(hAndle);
	return nMaxLen;
}

