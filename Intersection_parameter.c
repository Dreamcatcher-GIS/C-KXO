#include "headfile.h"
#include "runmain.h"
#include "zbzfs.h"
#include "MyFont.h"
#include "cvs.h"
#include <stdarg.h>

int PMproduce();

int PrintTables1(short x,short y,char * str,short sel,short lbl)
{
	int len;
	const int width = 28;
	len = strlen(str);
	x *= width;
	y *= 8;
	x += 16;
	if (sel)
	{
		Bdisp_AreaReverseVRAM(x,y,x+width-1,y+7);
		PrintMini(x+1+(lbl ? (width-5*len)/2:0),y+1,str,MINI_REV);
	}
	else
	{

		Bdisp_DrawLineVRAM(x+width-1,y,x+width-1,y+7);
		Bdisp_DrawLineVRAM(x,y+7,x+width-1,y+7);
		PrintMini(x+1+(lbl ? (width-5*len)/2:0),y+1,str,MINI_OVER);
	}
}

int PrintTableFormats1 (short x,short y,short sel,short lbl,char * format,...)
{
	char buffer[128];
	va_list parglist;
	va_start(parglist, format);
	vsprintf(buffer,format, parglist);
	va_end(pArgList);
	buffer[6] = 0;
	PrintTables1(x,y,buffer,sel,lbl);
	return 0;
}
int PrintTableIndexs1 (int y,int index)
{
	char buffer[8];
	if (index<0 || index>999) return 0;
	sprintf(buffer,"%d",index);
	y *= 8;
	Bdisp_AreaReverseVRAM(0,y,15,y+7);
	PrintMini(1,y+2,buffer,MINI_REV);
}
int WhetherCover1()
{
	int enter;
	PopUpWin(5);
	DispStr(48,12,"����?"); //fugai
	locate(7,5);Print("Yes : [F1]");
	locate(7,6);Print("No  : [F6]");
	do{
		GetKey(&key);
		if (key==KEY_CTRL_F6 || key==KEY_CTRL_EXIT){enter =0 ;break;}
		else if (KEY_CTRL_F1) {enter = 1;break;}
	}while(1);
	return enter;
}
int WheterSavaAS1()
{
	char str[13]="";
	int suc,enter,i;
	for (i=0;i<13 && setup_data.jdfroute_name[i] != '.';++i)
		str[i] = setup_data.jdfroute_name[i];
	str[i>=13 ? 12 : i] = 0;
	PopUpWin(4);
	DispStr(24,8,"\xce\xc4\xbc\xfe\xc3\xfb(*.jdf)");//file_name
	do
	{
		suc = InputString(24,24,str,1,8);
		if (suc==KEY_CTRL_EXIT) break;
		if (! str[0]) continue;
	}while(suc!=KEY_CTRL_EXE);
	if(suc!=KEY_CTRL_EXIT)
	{
		enter = 1;
		strcat(str,".jdf");
		if (TestJDFFile(str))
		{
			enter = WhetherCover1();
		}
		if (enter)
		{
			SaveJDFFile(str);
			strcpy(setup_data.jdfroute_name,str);
		}
	}
}

int Selectfile1()
{
	index = 0;
	need_redraw = REDRAW_PART;
	SetMyFont(&stHz12x12,&stAsc6x12);
	
	while(1)
	{
		if (need_redraw == REDRAW_PART)
		{
			PopUpWin(6);
			DrawPicture(32,9,	16,	10,	G_ICON_SAVE);	DispStr(48,8,"����Ϊ ");
			DrawPicture(32,25,	16,	10,	G_ICON_OPEN);	DispStr(48,24,"�� ");
			DrawPicture(32,41,	16,	10,	G_ICON_OTHER);	DispStr(48,40,"�½� ");
			
			Bdisp_AreaReverseVRAM(14,8+16*index,112,20+index*16);
		}
		GetKey(&key);
		if (key==KEY_CTRL_UP) 	{if(--index<0) index = 2;need_redraw = REDRAW_PART;}
		if (key==KEY_CTRL_DOWN)	{if(++index>2) index = 0;need_redraw = REDRAW_PART;}
		if (key==KEY_CTRL_EXIT)	break;
		if (key==KEY_CTRL_EXE)
		{
			char str[13]="";
			int suc,enter,i;
			switch(index)
			{
				case 0:
					
					for (i=0;i<13 && setup_data.jdfroute_name[i] != '.';++i)
						str[i] = setup_data.jdfroute_name[i];
					str[i>=13 ? 12 : i] = 0;
					PopUpWin(4);
					DispStr(24,8,"\xce\xc4\xbc\xfe\xc3\xfb(*.jdf)");//file_name
					do
					{
						suc = InputString(24,24,str,1,8);
						if (suc==KEY_CTRL_EXIT) break;
						if (! str[0]) continue;
					}while(suc!=KEY_CTRL_EXE);
					if(suc!=KEY_CTRL_EXIT)
					{
						enter = 1;
						strcat(str,".jdf");
						if (TestJDFFile(str))
						{
							PopUpWin(5);
							DispStr(48,12,"����?"); //fugai
							locate(7,5);Print("Yes : [F1]");
							locate(7,6);Print("No  : [F6]");
							do{
								GetKey(&key);
								if (key==KEY_CTRL_F6 || key==KEY_CTRL_EXIT){enter =0 ;break;}
								else if (KEY_CTRL_F1) {enter = 1;break;}
							}while(1);
						}
						if (enter)
						{
							SaveJDFFile(str);
							strcpy(setup_data.jdfroute_name,str);
						}
					}     
					break;
				case 1:
					suc = SelectJDFFiles (str);
					if (suc==-1)
					{
						PopUpWin(1);
						locate(7,4);Print("No File!");
						GetKey(&key);
					}
					if(suc==1)
					{
						LoadJDFFile  (str);
						strcpy(setup_data.jdfroute_name,str);
						if(LoadJDFFile  (setup_data.jdfroute_name)==1)
						SaveIniJDFFile();
						goto EXIT;
					}
					
				case 2:
					PopUpWin(5);
				    DispStr(48,12,"�½�?"); 
				    locate(7,5);Print("Yes : [F1]");
					locate(7,6);Print("No  : [F6]");
					GetKey(&key);
					if (key==KEY_CTRL_F1){
						PopUpWin(5);
				        DispStr(40,12,"��������?"); 
				        locate(7,5);Print("Yes : [F1]");
					    locate(7,6);Print("No  : [F6]");
					    GetKey(&key);
						if(key==KEY_CTRL_F1)
						{
							if(setup_data.jdfroute_name[0]==0)
				            {
					            WheterSavaAS1();
				            }
				            else
				            {
					            SaveJDFFile(setup_data.jdfroute_name);
				            }
						}						
						setup_data.jdfroute_name[0]=0;
				        jdfroute_initial(&jdfroute);
	                    jdfroute_push_back(&jdfroute);
						goto EXIT;
			        }   
				    break;
					
				default:break;
			}
			need_redraw = REDRAW_PART;
		}
	}
	EXIT:
	return 0;
}


int JDF_parameter()
{
	
	char * table_label[]= {"DH","JDZH","X","Y","QHCD","R","HHCD","QDBJ","ZDBJ"};
	//char * icons[]		= {"Set","Add","Ins","DelL","DelA"};
	char * icons[]		= {"","","","",""}; 
	char * ch_text[]	= {"\xb5\xe3\xc3\xfb",
	                        "����׮��",
							"����X",
							"����Y" ,
							"ǰ������ ",
							"�뾶 ",
							"�󻺳��� ",
							"���뾶 ",
							"�յ�뾶 ",
							};
	int x = 0,y = 0,i,j,t;
	int left = 0,top = 0;
	int refresh = REDRAW_ALL;
	int enter,suc;
	char str[13]="";
	char c;
	while(1){
		if (refresh==REDRAW_ALL){
			Bdisp_AllClr_VRAM();
			for (i=0;i<4;++i)
			PrintTables1(i,0,table_label[left+i],1,1);
			t = jdfroute.size<= 1 ? 2 : 5; //����������С�ڵ���1 ��ôT=2 �������5
			if 	(t>=2){
			SetMyFont(&stHz8x8,&stAsc6x12);
			DispStr(2,56,"\xCE\xC4\xBC\xFE");
		    DispStr(23,56,"���");
			}
			if 	(t>2){
			SetMyFont(&stHz8x8,&stAsc6x12);
			DispStr(45,56,"���� ");
		    DispStr(65,56,"ɾ�� ");
			DispStr(86,56,"��� ");
			}
			for (i=0;i< t;++i) // i<2 ��  i<5
			PrintIcon(i,icons[i],0);  //��ӡ�±� 
			PrintIcon(5,"?",0);  //�̶���ӡ�±�
			refresh = REDRAW_PART; // �ػ�����
		}
		if (refresh==REDRAW_PART){  //��������� 
			double * sd;
			for (i=top;i<top+4 && i<jdfroute.size;++i){ 
				sd =  & (jdfroute.set[i].jdf_jdzh);
				for (j=(left==0 ? 1:0);j<4;++j){
					if (i==0 && j+left>3)  //////////////////////////////////////////
					PrintTables1(j,i+1-top,"",y==i && x-left==j,0);///////////////
					else/////////
						PrintTableFormats1(j,i+1-top,y==i && x-left==j,0,"%lf",sd[j+left-1]);
				}
				if (left==0)
					PrintTableFormats1(0,i+1-top,y==i && x-left==0,0,"%s",jdfroute.set[i].name);
				PrintTableIndexs1(i-top+1,i);
			}
			SetMyFont(&stHz12x12,&stAsc6x12);
			if(x>0){
				if (!(y==0 && x>3)){
					sd =  & (jdfroute.set[y].jdf_jdzh); //ȡ��ַ��ֵ
					DispStr(0,42,ch_text[x]);
					PrintFloat(sd[x-1],49,42,0,setup_data.point); //��ӡ 
				}
			}else{
				DispStr(0,42,ch_text[x]);
				PrintXY(51,44,jdfroute.set[y].name,0);
			}
			refresh = 0;
		}
		GetKey(&key);
		switch (key){
		case KEY_CTRL_UP:
			y--;refresh = REDRAW_ALL;break;
		case KEY_CTRL_DOWN:
			y++;refresh = REDRAW_ALL;break;
		case KEY_CTRL_LEFT:
			x--;refresh = REDRAW_ALL;break;
		case KEY_CTRL_RIGHT:
			x++;refresh = REDRAW_ALL;break;
		case KEY_CTRL_EXIT:
		    PopUpWin(5);
			DispStr(48,12,"����?"); 
			locate(7,5);Print("Yes : [F1]");
			locate(7,6);Print("No  : [F6]");
			do{
				GetKey(&key);
				if (key==KEY_CTRL_F6 || key==KEY_CTRL_EXIT){enter =0 ;break;}
				else if (KEY_CTRL_F1) {enter = 1;break;}
			}while(1);
			if(enter)
			{
				if(setup_data.jdfroute_name[0]==0)
				{
					WheterSavaAS1();
				}
				else
				{
					SaveJDFFile(setup_data.jdfroute_name);
				}
			}
			return 0 ;
		case KEY_CTRL_F1:					/* Set */
			refresh = REDRAW_ALL;
			Selectfile1();
			break;
		case KEY_CTRL_F2:					/* Add */
			jdfroute_push_back (&jdfroute);
			y = jdfroute.size - 1;
			refresh = REDRAW_ALL;
			break;
		case KEY_CTRL_F3:					/* ins */
			if (y==0) break;
			jdfroute_insert (&jdfroute,y);
			y++;
			refresh = REDRAW_ALL;
			break;
		case KEY_CTRL_F4:					/* Del a point */
			if (y==0) break;
			refresh = REDRAW_ALL;
			PopUpWin(4);DispStr(25,10,"ɾ �� һ ��?");
			locate(3,5);Print("[F1]Yes   [F6]No");
			GetKey(&key);
			if (key==KEY_CTRL_F1){
				jdfroute_del(&jdfroute,y);y--;
			}
			break;
			
		 case KEY_CTRL_F5:					/* Del All */
			refresh = REDRAW_ALL;
			PopUpWin(4);DispStr(25,10,"�� �� �� �� ?");
			locate(3,5);Print("[F1]Yes   [F6]No");
			GetKey(&key);
			if (key==KEY_CTRL_F1){
				jdfroute_initial(&jdfroute);
	            jdfroute_push_back(&jdfroute);
			}
			break;

         case KEY_CTRL_F6:
		    refresh = REDRAW_ALL;
			//PopUpWin(6);
			PMproduce();
			//GetListq();
			
			//PrintfXY (15,10,0,"%.3f",jdfroute.set[1].jdf_jdzh);  
			//GetKey(&key);
			//PrintfXY (15,20,0,"%.3f",jdfroute.set[2].jdf_jdzh);  
			//PrintfXY (15,30,0,"%.3f",jdfroute.set[3].jdf_jdzh);  
			//GetKey(&key);
			//PrintfXY (70,30,0,"%.3f",route.set[0].xy_zr);  
			//DispStr(15,10,"��·����");
			//if(setup_data.jdfroute_name[0]==0)
			//{
			 //DispStr(25,27,"�뱣����·�ļ� ");
			//}
			 //else
			//{
             //DispStr(25,27,setup_data.zdmroute_name);
			//}
          	break;	
		default:							/*other things*/
			if (x>0 && (c=GetPressNumChar(key))!=0){
				char buffer[14];
				int r;
				if (y==0 && x > 3)break;
				buffer[0] = c;
				buffer[1] = '\0';
				r = InputString(49,42,buffer,0,12);
				if (r!=KEY_CTRL_EXIT){
					double * sd;
					sd =  (double*)&(jdfroute.set[y].jdf_jdzh);
					sd[x-1] = atof(buffer);
					switch(r){
					case KEY_CTRL_UP:	y--;break;
					case KEY_CTRL_DOWN:	y++;break;
					case KEY_CTRL_EXE:	x++;break;
					}
				}
				refresh = REDRAW_ALL;
			}
			else if (x==0 && (c=GetPressChar(key))!=0){
				char buffer[14];
				int r;
				buffer[0] = c;
				buffer[1] = '\0';
				r = InputString(49,42,buffer,1,12);
				if (r!=KEY_CTRL_EXIT){
					strncpy(jdfroute.set[y].name,buffer,12);
					switch(r){
					case KEY_CTRL_UP:	y--;break;
					case KEY_CTRL_DOWN:	y++;break;
					case KEY_CTRL_EXE:	x++;break;
					}
				}
				refresh = REDRAW_ALL;
			}
			break;
		}
		if (x<0) x = 8,y--;
		if (x>8) x = 0,y++;
		if (y<0) y = 0;
		if (y>=jdfroute.size) y = jdfroute.size - 1;
		if (y>top + 3) top = y - 3;
		if (y<top) top = y;
		if (x>left +3) left = x - 3;
		if (x<left) left = x;
	}
}


int PMproduce()
{        
    int  luhao[3];   
	int  px,i,n;       
	int  inluxian;  
    double jdk[3]; 
	double jdx[3]; 
	double jdy[3];  
    double ls1[3]; 
    double r[3];    
	double ls2[3];	
	double qdr[3]; 
	double zdr[3];   
	double fwj[3];   
	double jdjl[3];  
	double t[3];   
	double zhejao;  
	double jdjj;     
	double qdk;     
	double p[2];     
	double q[2];    
	double l;        
	double zhk,hyk,yhk,hzk,zhx,zhy,hzx,hzy,hyfj,yhfj,hyx,hyy,yhx,yhy; 
	double hzkq,hzxq,hzyq;  
	qdk=jdfroute.set[0].jdf_jdzh; 
    for (i=0;i<jdfroute.size-2;++i){ 
		for (n=0;n<3;++n){
		    jdk[n]=jdfroute.set[i+n].jdf_jdzh; 
			jdx[n]=jdfroute.set[i+n].jdf_jdx;   
			jdy[n]=jdfroute.set[i+n].jdf_jdy;
			ls1[n]=jdfroute.set[i+n].jdf_ls1;
			r[n]=jdfroute.set[i+n].jdf_r;
			ls2[n]=jdfroute.set[i+n].jdf_ls2;
			qdr[n]=jdfroute.set[i+n].jdf_qdr;
			zdr[n]=jdfroute.set[i+n].jdf_zdr;		 
		}

		     //if (jdk[1]==0) { 
        	 jdk[0]=qdk ;
		     jdjj=sqrt(pow((jdx[0]-jdx[1]),2)+pow((jdy[0]-jdy[1]),2)); //�������������֮��ľ���
		     jdk[1] = jdk[0]+ jdjj;  //��һ������׮��,�������һ������
			 jdfroute.set[i+1].jdf_jdzh=jdk[1];
			 PrintTableFormats1(15,15,0,0,"%lf", jdfroute.set[i+1].jdf_jdzh);
		     fwj[0] = azimuth(jdx[0],jdy[0],jdx[1],jdy[1]); //���㽻��ǰ����֮��ķ�λ��
		     fwj[1] = azimuth(jdx[1],jdy[1],jdx[2],jdy[2]); //���㽻�������֮��ķ�λ��
		     zhejao = (fwj[1] - fwj[0]);  //�ǶȲ�
			 
			 
            if (zhejao > 0) {      //��ת��ֵ���� 
			    zhejao = fwj[1]-fwj[0]; 
                if (zhejao < 0)  zhejao=zhejao+360;
                px=1;  
		    } 
		
	        else {  
	             zhejao = fwj[0]-fwj[1];    //��תֵ����
                if (zhejao < 0)  zhejao=zhejao+360;
	             px=-1;  
		    }  
		//} 	
		
             zhejao = fabs(zhejao);  
		     p[0]=pow(ls1[1],2)/(24*r[1])-pow(ls1[1],4)/2688/pow(r[1],3)+pow(ls1[1],6)/506880/pow(r[1],5)-pow(ls1[1],8)/154828800/pow(r[1],7);  //ƫ��ֵ3.3333
		     q[0]=ls1[1]/2-pow(ls1[1],3)/(240*pow(r[1],2))+pow(ls1[1],5)/34560/pow(r[1],4)-pow(ls1[1],7)/8386560/pow(r[1],6)+pow(ls1[1],9)/3158507520
             /pow(r[1],8);//ԭ������ֱ��ƫ��ֵ99.8666666
			 p[1]=pow(ls2[1],2)/(24*r[1])-pow(ls2[1],4)/2688/pow(r[1],3)+pow(ls2[1],6)/506880/pow(r[1],5)-pow(ls2[1],8)/154828800/pow(r[1],7);  //ƫ��ֵ3.3333
			 q[1]=ls2[1]/2-pow(ls2[1],3)/(240*pow(r[1],2))+pow(ls2[1],5)/34560/pow(r[1],4)-pow(ls2[1],7)/8386560/pow(r[1],6)+pow(ls2[1],9)/3158507520
             /pow(r[1],8);//ԭ������ֱ��ƫ��ֵ99.8666666
			 t[0]=fabs((r[1] + p[0]) * (tan(zhejao / 2*PI/180.0)) + q[0] + (p[0] - p[1]) / (sin(zhejao*PI/180.0)));  //��һ���߳� *PI/180.0 �Ƕ�ת������
			 t[1]=fabs((r[1] + p[1]) * (tan(zhejao / 2*PI/180.0)) + q[1] + (p[0] - p[1]) / (sin(zhejao*PI/180.0)));  //�ڶ����߳� *PI/180.0 �Ƕ�ת������
			 l=fabs (zhejao*PI/180)*r[1]+(ls1[1]+ls2[1])/2;  //�����ܳ�
			 zhk = jdk[1] - t[0];      //ֱ����׮�� ֱԲ��                    
             hyk = zhk + ls1[1];       //��Բ��׮�� Բֱ��                     
             yhk = zhk + l - ls2[1];   //Բ����׮��                  
             hzk = zhk + l;            //��ֱ��׮��                  
             zhx = jdx[1] + t[0] * cos(fwj[0]*PI/180.0 + PI);    //ֱ��������X 			 
             zhy = jdy[1] + t[1] * sin(fwj[0]*PI/180.0 + PI);    //ֱ��������Y 
             hzx = jdx[1] + cos(fwj[1]*PI/180.0) * t[1];         //��ֱ������X
             hzy = jdy[1] + sin(fwj[1]*PI/180.0) * t[1];         //��ֱ������Y 
			 
        if (ls1[1] > 0) {       //�����һ�������ߴ���0�����㻺Բ�� 
			 intialCZb(zhk,hyk,zhx,zhy,fwj[0],px,0,1/r[1]);  //���ݺ��� 
			 hyfj=zbzs_a(hyk, 0, 0);  //���㻺Բ�㷽λ��     
             hyx=zbzs_x(hyk, 0, 0);   //���㻺Բ��X����       
		     hyy=zbzs_y(hyk, 0, 0);   //���㻺Բ��Y����           
	    } 
				
		if (ls2[1] > 0) {          //����ڶ��������ߴ���0 ������Բ���� 
			 intialCZb(0,ls2[1],hzx,hzy,fwj[1]+180,px*(-1),0,1/r[1]);  //���ݺ���  �������
			 yhfj=zbzs_a(ls2[1], 0, 0)-180;   //����Բ���㷽λ�� �������λ��-180
			if(yhfj<0) yhfj=yhfj+360;     //����λ��ת��Ϊ����
			 yhx=zbzs_x(ls2[1], 0, 0);    //����Բ����X���� 
			 yhy=zbzs_y(ls2[1], 0, 0);    //����Բ����Y����
        } 
			
		if (i<1) {     //д��ֱ�߶�
			 read_in(jdk[0],zhk,jdx[0],jdy[0],fwj[0],px,0,0);  
		}
		else {
				read_in(hzkq,zhk,hzxq,hzyq,fwj[0],px,0,0);       
		}
			
		if (ls1[1]==0 && ls2[1]==0) {                    //�������߶�Ϊ0��ֻд��Բ����
			 read_in(zhk,hzk,zhx,zhy,fwj[0],px,r[1],r[1]); 
        }			
		else if  (ls1[1]==0 && ls2[1]!=0) {              //�ڶ��������߲�Ϊ0                    
			 read_in(zhk,yhk,zhx,zhy,fwj[0],px,r[1],r[1]);   //��д��Բ����
			 read_in(yhk,hzk,yhx,yhy,yhfj,px,r[1],0);        //��д��ڶ���������
        }		
		else if	 (ls1[1]!=0 && ls2[1]==0) {                //��һ�������߲�Ϊ0 �ڶ���Ϊ0                  
			 read_in(zhk,hyk,zhx,zhy,fwj[0],px,0,r[1]);     //��д���һ��������    
			 read_in(hyk,hzk,hyx,hyy,yhfj,px,r[1],r[1]);    //��д��Բ����
		}
        else {                                                  
			 read_in(zhk,hyk,zhx,zhy,fwj[0],px,0,r[1]);    //д���һ��������    
			 read_in(hyk,yhk,hyx,hyy,hyfj,px,r[1],r[1]);   //д��Բ����
			 read_in(yhk,hzk,yhx,yhy,hyfj,px,r[1],0);      //д��ڶ���������
        }
		     hzkq=hzk;
			 hzxq=hzx;
			 hzyq=hzy;
			 qdk=hzk-t[1];
			 
		if  (i==jdfroute.size-3)  {  
			 jdjj=sqrt(pow((jdx[2]-hzx),2)+pow((jdy[2]-hzy),2));  //д���ֱ�߶�
		     jdx[2]= hzk + jdjj; 
			 read_in(hzk,jdx[2],hzx,hzy,fwj[0],px,0,0);
		}     
		
		     jdfroute.set[jdfroute.size].jdf_jdzh=jdk[2]; 
		     PrintTableFormats1(1,3,5,0,"%lf",jdfroute.set[jdfroute.size].jdf_jdzh);
	}
} 


int read_in(double qk,double zk,double qx,double qy,double qfj,int p_x,double  qr,double zr)
{

	s_pqx newNode[1];
	newNode[0].ss=qk;
	newNode[0].es=zk;
    newNode[0].sx=qx;
	newNode[0].sy=qy;
	newNode[0].sa=qfj;
	newNode[0].v=p_x;
	newNode[0].sp=qr;
	newNode[0].ep=zr;
	PushList(&l_pqx,newNode[0]);
}


/*int GetListq()
{
	int iter;
	s_pqx temp;
	for(iter=0; iter<l_pqx.length; iter++)
	{	
        temp = GetList(&l_pqx,iter);
		PrintfXY (48,2,0,"%.4f", temp.ss); 
		PrintfXY (48,10,0,"%.4f",temp.es); 
		PrintfXY (48,18,0,"%.4f",temp.sx); 
		PrintfXY (48,28,0,"%.4f",temp.sy); 
		PrintfXY (48,36,0,"%.4f",temp.sa);
		PrintfXY (48,44,0,"%.4f",temp.v);
		PrintfXY (48,52,0,"%.4f",temp.sp);
		PrintfXY (48,60,0,"%.4f",temp.ep);

		GetKey(&key);
	}
} */

