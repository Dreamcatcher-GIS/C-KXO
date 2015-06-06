#include "headfile.h"
#include "xy.h"
#include "MyFont.h"

static char TITLE[] = "";

int computeFS ()
{
	uint refresh = 2,key;
	int  index = 0,i,hcs;
	char ch;
	double ht,hz,v,bh,hd;
	double value[4] = {0.0,0.0,0.0,0.0};
	const char * context[] = {"","","",""};
	while(1){
			if (refresh == REFRESH_ALL){
			refresh = REFRESH_PART;
			Bdisp_AllClr_VRAM();
			SetMyFont(&stHz12x12,&stAsc6x12);  //���������С
			DispStr(2,1, "�� ");
			DispStr(2,12,"· ");
			DispStr(2,25,"�� ");
			DispStr(2,38,"�� ");
			ShowTitle(TITLE);
			DispStr(17,0, "����X: ");
		    DispStr(17,13,"����Y: ");
			DispStr(17,26,"����Z: ");
			//DispStr(17,39,"����Z: ");

			for (i=0;i<4;++i){
				PrintXY(50,2+i*12,(uchar*)context[i],0);
			}
			PrintIcon (0,"Rou",0);
			PrintIcon (1,"Set",0);
			PrintIcon (2,"Disp",0);
			PrintIcon (3,"=0=",1);
			PrintIcon (4,"Save",1);
			PrintIcon (5,"?",0);
		}
		if (refresh == REFRESH_PART){
			for (i=0;i<4;++i){
		     PrintFloat(value[i],48,0+i*12,index==i,4);	//�����λ��
			}
			refresh = 0;
		}
		GetKey(&key);
		switch(key){
			case KEY_CTRL_F1	:
				SET();	refresh=REFRESH_ALL;	break;
			case KEY_CTRL_F2	:
				ZSSET();	refresh=REFRESH_ALL;	break;
			case KEY_CTRL_F3	:
				RestoreDisp(SAVEDISP_PAGE1);refresh=REFRESH_ALL;WaitKey();break;
			case KEY_CTRL_F4	:
			case KEY_CTRL_F5	:
				Nodata();	refresh=REFRESH_ALL;	break;
			case KEY_CTRL_F6	:
				About();	refresh=REFRESH_ALL;	break;
			case KEY_CTRL_UP	:
				refresh=REFRESH_PART;	--index;	break;
			case KEY_CTRL_DOWN	:
				refresh=REFRESH_PART;	++index;	break;
			case KEY_CTRL_EXE:
			if (index<3){
				refresh = REFRESH_PART;	index++;
			}else{
				yscs.x1 = value[0];	yscs.y1 = value[1];
				hz = value[2];	ht = value[3];
				refresh=REFRESH_ALL;
				//yscs.k=0.0;
				yscs.k=xy_to_sdSrd( yscs.x1, yscs.y1, 0);
				yscs.d=xy_to_sdSrd( yscs.x1, yscs.y1, 1);
				//yscs.k=xy_to_sd( yscs.x1, yscs.y1, 0);
				//yscs.d=xy_to_sd( yscs.x1, yscs.y1, 1);
				
				//FS();
				GC();
				HP();
				if	(yscs.d<0)	hcs=-1;	//ƫ�����
		 		if	(yscs.d>=0)	hcs=1; //ƫ���Ҳ�
		 		if	(yscs.d*hcs<=zfset.zsp)	hd=0;  //
	 			if	(zfset.zsh==0)	hd=0;
	 			if	(yscs.d*hcs>zfset.zsp && yscs.d*hcs-zfset.zsp<zfset.zsh) hd=(yscs.d*hcs-zfset.zsp)*hcs;
				if	(yscs.d*hcs-zfset.zsp>=zfset.zsh)	hd=zfset.zsh*hcs;
				bh=yscs.gch+(yscs.hpd*hd)-ht;
				Bdisp_AllClr_VRAM();
				DispStr(2,0, "�� ");
			    DispStr(2,12,"�� ");
			    DispStr(2,26,"�� ");
			    DispStr(2,39,"�� ");
				ShowTitle(TITLE);
				DispStr(17,0,"׮�� "); 
			    DispStr(17,13,"ƫ�� ");
			    DispStr(17,26,"����: ");
				DispStr(17,39,"����: ");
				PrintfXY (48,2.5,0,"%.4f",yscs.k); 
				PrintfXY (48,15,0,"%.4f",yscs.d);
				PrintfXY (48,28,0,"%.4f",yscs.gch);
				PrintfXY (48,42,0,"%.4f",yscs.hpd);
				//PrintfXY (22,40,0," H= %.3f",bh);
				//PrintfXY (22,48,0," H-Z= %.3f",hz-bh);
				SaveDisp(SAVEDISP_PAGE1);
				WaitKey();
				++index;
			}break;
			case KEY_CTRL_EXIT:
				return 0;
			default:
				if ((ch=GetPressNumChar(key))!=0){
					char buf[14];
					int        r;
					refresh = REFRESH_PART;
					buf[0] = ch;   buf[1] = '\0';
					r = InputString (48,0+index*12,buf,0,12);  //�����λ��
					if (r != KEY_CTRL_EXIT){

						value[index] = atof(buf);
						if	(r==KEY_CTRL_DOWN)	++index;
						if	(r==KEY_CTRL_UP)	--index;
						if (r==KEY_CTRL_EXE && index<3)	++index;
					}
					break;
				}
		}
		if (index >=4) index = 2;
		if (index < 0) index = 3;
	}
}