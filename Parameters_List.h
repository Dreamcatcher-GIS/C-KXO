
typedef struct
{
	double Spacing;				//���
	double BeginStake;			//��׮
	double PegNo;				//׮��
	double Offset;				//ƫ��
	double value[4];			//(xa,ya)(xb,yb)
	double distance,position;		//���롢��λ
}COMMONDATA;

// �����á�������
typedef struct
{
	int point; 	//С����λ��
	char jdfroute_name[13];
	char zdmroute_name[13];
	char route_name[13] ;
	char pmroute_name[13];
}SETUPDATA;


typedef struct
{
	char name[12];
	double jdf_jdzh;
	double jdf_jdx;
	double jdf_jdy;
	double jdf_ls1;
	double jdf_r;
	double jdf_ls2;
	double jdf_qdr;
	double jdf_zdr;

}JDFPOINT;

typedef struct
{
	JDFPOINT *set;
	int size;
}JDFROUTE;



typedef struct
{
	char name[12];
	double qd_zh,zd_zh,xy_x,xy_y,xy_a,xy_v,xy_qr,xy_zr;
}POINT;

typedef struct
{
	double qz;		//����׮
	double zxz;		//ת��ֵ
	double fwj1;
	double jl1;
	POINT *set;		//��Ԫ���ϣ�ʹ���±����(l.set[n])
					//n��0��ʼ��n���ֵΪsize-1
	int size;		//��Ԫ����
}ROUTE;



typedef struct
{
	char name[12];
	double zdm_bbzh;
    double zdm_bbgc;
    double zdm_r;
    double zdm_zdzh;
}ZDMPOINT;


typedef struct
{
	ZDMPOINT *set;
	int size;
}ZDMROUTE;

typedef struct
{
	char name[12];
	double pm_qk;
    double pm_zk;
    double pm_qx;
	double pm_qy;
	double pm_qfj; 
	double pm_p_x;
	double pm_qr;
	double pm_zr;
}PMPOINT;


typedef struct
{
	PMPOINT *set;
	int size;
}PMROUTE;



typedef struct 
{
	double ls1;	    /*ǰ��������*/
	double r;	    /*Բ���߰뾶*/
	double ls2;	    /*�󻺺����� */
	double p1,p2; 
	double q1,q2;
	double t1,t2;
	double l,e,zhz,hyz,yhz,hzz,zhx,zhy,hzx,hzy;
	POINT point;
}TEMP_POINT;



typedef struct
{
	double zhz;		//ֱ����׮��
	double zhx;		//ֱ����X����
	double zhy;		//ֱ����Y����
	double qxz;		//Բ������׮
	double qxx;		//Բ������X����
	double qxy;		//Բ������y����
	double zjz;		//ת��ֵ
	double jdj;		//��������
	double jdfwj;	//����䷽λ��
}POINTRESULT;


void jdfroute_initial		(JDFROUTE *l);				
void jdfroute_push_back		(JDFROUTE *l);				
void jdfroute_insert		(JDFROUTE *l,int index);	
void jdfroute_del			(JDFROUTE *l,int index);	
void jdfroute_clr			(JDFROUTE *l);


void route_initial		(ROUTE *l);				//��ʼ����·
void route_push_back	(ROUTE *l);				//��ĩβ���һ���ڵ�
void route_insert		(ROUTE *l,int index);	//��ָ��λ�ò���һ���ڵ�
void route_del			(ROUTE *l,int index);	//��ָ��λ��ɾ��һ���ڵ�
void route_clr			(ROUTE *l);				//������нڵ�	

void zdmroute_initial		(ZDMROUTE *l);				
void zdmroute_push_back		(ZDMROUTE *l);				
void zdmroute_insert		(ZDMROUTE *l,int index);	
void zdmroute_del			(ZDMROUTE *l,int index);	
void zdmroute_clr			(ZDMROUTE *l);		

void pmroute_initial		(PMROUTE *l);				
void pmroute_push_back		(PMROUTE *l);				
void pmroute_insert		    (PMROUTE *l,int index);	
void pmroute_del			(PMROUTE *l,int index);	
void pmroute_clr			(PMROUTE *l);	
	

	

extern JDFROUTE jdfroute;
extern ROUTE route;
extern ZDMROUTE zdmroute;
extern PMROUTE pmroute;

extern int whether_load;        // point out the file name last load 