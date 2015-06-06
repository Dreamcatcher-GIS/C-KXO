#pragma once
#include <math.h>
//#include <iostream>
#include "headfile.h"
#include "cvs.h"
#include "zbzfs.h"

#define N_MAX 16


double ss;	// ���׮��
double es;	// �յ�׮��
double sx;	// �������X
double sy;	// �յ�����Y
double sa;	// ������귽λ��(DEG)
int v;		// ��Ԫת��,��תΪ1,��תΪ-1
double sp;	// ��Ԫ�������,ֱ��Ϊ0,Բ����Ϊ1/R
double ep;	// ��Ԫ�յ�����,ֱ��Ϊ0,Բ����Ϊ1/R

	// ����ȷֵ㴦׮��
	// ����n�ȷִ�ʱnȡ2,4,6..
	// ����2n�ȷִ�ʱnȡ1,3,5...
	double s_function(double s, int n);
	// �������s������
	double p_function(double s);
	// �������s�����߷�λ��(DEG)
	double a_function(double p, double s);

	// �������s������X/Y�����귽λ��A(DEG)
	double zbzs_m_xy(double s, int xy);
	double zbzs_m_a(double s);
	// ��������X/Y��Ӧ����̼�ƫ��
	double zbfs_s_d(double px, double py, int sd);
//
//public:
//	// ���캯�������ݳ�ʼ��
    void intialCZb(double,double ,double ,double,double,int,double,double);
//	
//	~CZb(void);
//	
	// �������stake/ƫ��distance/�Ƕ�angle�������꼰���귽λ��(DEG)
	double zbzs_x(double stake, double distance,double angle);
	double zbzs_y(double stake, double distance,double angle);
	double zbzs_a(double stake, double distance,double angle);
	// ��������Ϊpx/py������̼�ƫ��
	double zbfs_s(double px, double py);
	double zbfs_d(double px, double py);


void intialCZb(double ss_v,double es_v,double sx_v,double sy_v,double sa_v,int v_v,double sp_v,double ep_v)
{	ss=ss_v;   // ���׮��
	es=es_v;   // �յ�׮��
	sx=sx_v;   // �������X
	sy=sy_v;   // �յ�����Y
	sa=sa_v;   // ������귽λ��(DEG)
	v =v_v;    // ��Ԫת��,��תΪ1,��תΪ-1
	sp=sp_v;   // ��Ԫ�������,ֱ��Ϊ0,Բ����Ϊ1/R
	ep=ep_v;   // ��Ԫ�յ�����,ֱ��Ϊ0,Բ����Ϊ1/R
}

// ����ȷֵ㴦���
// ����n�ȷִ�ʱnȡ2,4,6...
// ����2n�ȷִ�ʱnȡ1,3,5...
double s_function(double s, int n)
{	return (s-ss)*n/N_MAX+ss;
}

// �������s������
double p_function(double s)
{	return sp+(ep-sp)*(s-ss)/(es-ss);  //�������+���������-������ʣ�*�����������-�����̣�/(�յ����-�����̣�
}

// �������s�����߷�λ��
double a_function(double p, double s)
{	return sa+90*v*(p+sp)*fabs(s-ss)/PI;  //��㷽λ��+90*��Ԫƫ��*��p��ʲô+������ʣ�*����ֵ����������-�����̣�/��
}

// �������s����X/Y
double zbzs_m_xy(double s, int xy)
{	// N_MAX�ȷֵ㴦���
    double s_array[N_MAX];
    // N_MAX�ȷֵ㴦����
    double p_array[N_MAX];
    // N_MAX�ȷֵ㴦���߷�λ��
    double a_array[N_MAX];
	double sum_n_cos = 0;
	double sum_2n_cos = 0;
	double sum_n_sin = 0;
	double sum_2n_sin = 0;
	double s_p , s_a;
	double H = fabs(ss-s)/(N_MAX/2);
	// �����ʼ��
	int iTmp;
	for (iTmp=0; iTmp<N_MAX; iTmp++)
	{	s_array[iTmp] = 0;
        p_array[iTmp] = 0;
        a_array[iTmp] = 0;
	}
	// ����N_MAX(n&2n)�ȷֵ㴦��̵�s_array(N_MAX)
	// ����N_MAX(n&2n)�ȷֵ㴦���ʵ�p_array(N_MAX)
	// ����N_MAX(n&2n)�ȷֵ㴦���߷�λ�ǵ�A_array(N_MAX)
	for (iTmp=0; iTmp<(N_MAX-1); iTmp++)
	{	s_array[iTmp] = s_function(s, iTmp+1);
		p_array[iTmp] = p_function(s_array[iTmp]);
		a_array[iTmp] = a_function(p_array[iTmp], s_array[iTmp]);
	}
	// ����N_MAX/2(n)�ȷֵ㴦���߷�λ�����Һ�sum_n_cos

    for (iTmp=1; iTmp<(N_MAX-1); iTmp=iTmp+2)
	{	
		sum_n_cos = sum_n_cos+cos(rad(a_array[iTmp]));
	}
	// ����N_MAX(2n)�ȷֵ㴦���߷�λ�����Һ�sum_2n_cos
	
	for (iTmp=0; iTmp<(N_MAX-1); iTmp=iTmp+2)
	{	sum_2n_cos = sum_2n_cos+cos(rad(a_array[iTmp]));
	}
	// ����N_MAX/2(n)�ȷֵ㴦���߷�λ�����Һ�sum_n_sin
	
    for (iTmp=1; iTmp<(N_MAX-1); iTmp=iTmp+2)
	{	sum_n_sin = sum_n_sin+sin(rad(a_array[iTmp]));
	}
	// ����N_MAX(2n)�ȷֵ㴦���߷�λ�����Һ�sum_2n_sin
	
	for (iTmp=0; iTmp<(N_MAX-1); iTmp=iTmp+2)
	{	sum_2n_sin = sum_2n_sin+sin(rad(a_array[iTmp]));
	}
	// ����s������s_p�����߷�λ��s_a
	
	s_p = p_function(s);
	s_a = a_function(s_p, s);

	// ��������ֵ
	
	if( xy==0 )
	{	return (sx + H / 6 * (cos(rad(sa)) + 4 * sum_2n_cos + 2 * sum_n_cos + cos(rad(s_a))));
	}
	else
	{	return (sy + H / 6 * (sin(rad(sa)) + 4 * sum_2n_sin + 2 * sum_n_sin + sin(rad(s_a))));
	}
}

// �������s���귽λ��A
double zbzs_m_a(double s)
{	double ma;
	ma = a_function(p_function(s), s);
	if (ma>360)
	{	ma = ma-360;
	}
	if (ma<0)
	{	ma = ma+360;
	}
	return ma;
}

// ����P������px/py��Ӧ����̼�ƫ��
// ��Pλ����������Ԫ���������������Բ����˵����
// 1. λ����Ԫ�ڲ���λ��Բ���ڲ� �� λ����Ԫ���
//    ��d1(����Ԫ��㷨�ߴ���)С����Ԫ���Ȳ����㷨A
// 2. λ����Ԫ�ڲ���λ��Բ���������㷨B
// 3. λ����Ԫ�����d1����Ԫ������Ԫ���Ȳ����㷨B
double zbfs_s_d(double px, double py, int sd)
{	// ������Ԫ�յ�����ex/ey����λ��ea
	double ex = zbzs_m_xy(es, 0);
	double ey = zbzs_m_xy(es, 1);
	double ea = zbzs_m_a (es);

	// ������Ԫ���յ���P�㷽λ��֮�н�
	double sa_spa_b = fabs(sa - azimuth(sx, sy, px, py) );
	double ea_epa_b = fabs(ea - azimuth(ex, ey, px, py) );

	double d;
	d = fabs( (py-sy) * cos(rad(sa-90)) - (px-sx) * sin(rad(sa-90)) );

	// 1. λ����Ԫ�ڲ���λ��Բ���ڲ� �� λ����Ԫ���
	//    ��d1(����Ԫ��㷨�ߴ���)С����Ԫ���� �㷨A
	if( sa_spa_b<90 && ea_epa_b>90 && d<=fabs(es-ss) )
	{	double di=0; //������ʱ����di
		double pix=0, piy=0; //������ʱ����pix,piy

		// �㷨A
		///////////////////////////////////////////
		//std::cout << "�����㷨A" << std::endl;
		///////////////////////////////////////////
		//(1)������ʱ����di,��ʱ����pix,piy
        //(2)��ss+d��Ϊ������̼�������pix,piy
        //(3)����ss+d�����귽λ��a_ssd
        //(4)����di=(py-piy)*cos(a_ssd-90)-(px-pix)*sin(a_ssd-90)
        //(5)d=d+di
        //(6)����ss+d��������pix,piy
        //(7)��abs(di)��0.001���������stake=ss+d/ƫ��distance=(piy-py)/sin(a_ssd-90)�������ظ�(3)��(7)
		
		pix = zbzs_m_xy( ss+d, 0); //������Ԫ+D ����x
		piy = zbzs_m_xy( ss+d, 1); //������Ԫ+D ����y

		do
		{	di = (py-piy) * cos(rad( a_function(p_function(ss+d), ss+d )-90 ))  
			   - (px-pix) * sin(rad( a_function(p_function(ss+d), ss+d )-90 ));  //�𲽱ƽ�ֵ��ֱ��diС��0.001���Ƿ�����Ӧ�����

			d = d + di; //d+�𲽱ƽ�ֵ
			pix = zbzs_m_xy( ss+d, 0);  //�𲽱ƽ�����X 
			piy = zbzs_m_xy( ss+d, 1);  //�𲽱ƽ�����y
			//// DEBUG
			//std::cout << "di = " << di << std::endl;
			//std::cout << "da = " << a_function(p_function(ss+d), ss+d )-90 << std::endl;
			//getchar();
			//// END DEBUG
		}while( fabs(di)>0.001 );

		if( sd==0 )
		{	return ( ss+d );   //���
		}
		else
		{	return (piy-py) / sin(rad( a_function(p_function(ss+d), ss+d )-90 ));   //ƫ��
		}

	}
	
	
	// 2. λ����Ԫ�ڲ���λ��Բ����� �㷨B
	// 3. λ����Ԫ�����d1����Ԫ������Ԫ���� �㷨B

	else
	{
     //��������㷨B
     //(1)�������s1/s2/s3
     //����ȷִ����꼰��λ��s2x/s2y/s2a
     //����P����s1/s2/s3���ߴ���d1/d2/d3
     //(2)��P������㷨�ߴ���d1<0.001 ����ss
     //(3)��P�����յ㷨�ߴ���d3<0.001 ����es
     //(4)�����̸�ֵ��s1: s1=ss
     //(5)�յ���̸�ֵ��s3: s3=es
     //(6)����s1��s3�зֵ㴦���s2: s2=0.5*(s3-s1)+s1
     //(7)����s2������ֵ����λ�ǣ� s2x/s2y/s2a
     //(8)����P����s2���ߴ���d2:  ��d2<0.001 ����s2����stake
     //(9)��d1*d2<0 --> s3=s2����s1=s2
     //(10)�ظ�(6)��(9)
	 
		double s1, s2, s3;
		double s2x, s2y, s2a;
		double d1, d2, d3;
		//// DEBUG
		//int n=1;
		//std::cout << "�����㷨B" << std::endl;
		//// END DEBUG

		d1 = (py-sy) * cos(rad(sa-90)) - (px-sx) * sin(rad(sa-90));

		if( fabs(d1) < 0.001 )
		{	return ss;
		}
		d3 = (py-ey) * cos(rad(ea-90)) - (px-ex) * sin(rad(ea-90));
		if( fabs(d3) < 0.001 )
		{	return es;
		}

		s1 = ss;
		s3 = es;

		do
		{	s2 = 0.5 * (s3 - s1) + s1; //������Ԫ�������
			s2x = zbzs_m_xy( s2, 0 ); //������Ԫ����x
			s2y = zbzs_m_xy( s2, 1 ); //������Ԫ����y
			s2a = zbzs_m_a ( s2 );  ////������Ԫ���е�������߷�λ��

			d2 = (py-s2y) * cos(rad( s2a-90 )) - (px-s2x) * sin(rad( s2a-90 ));  

			if( d1*d2 < 0 )
			{	s3 = s2;
			}
			else
			{	s1 = s2;
			}
			//n++;
		}while( fabs(d2)>0.001 );
		//// DEBUG
		//std::cout << "���д���="<< n <<"\n";
		//// END DEBUG
		if( sd==0 )
		{	return s2;
		}
		else
		{	return (s2y-py) / sin(rad( s2a-90 ));
		}
	}

}

/////////////////
// �ⲿ���ú���
/////////////////
double zbzs_x(double stake, double distance, double angle)
{	return (zbzs_m_xy(stake, 0)+fabs(distance)*cos(rad(zbzs_m_a(stake)+sign(distance)*fabs(angle))));
}

double zbzs_y(double stake, double distance, double angle)
{	return (zbzs_m_xy(stake, 1)+fabs(distance)*sin(rad(zbzs_m_a(stake)+sign(distance)*fabs(angle))));
}

double zbzs_a(double stake, double distance, double angle)
{	double a;
	a = zbzs_m_a(stake)+sign(distance)*fabs(angle);
	if (a>360)
	{	a = a-360;
	}
	if (a<0)
	{	a = a+360;
	}
	return a;
}

double zbfs_s(double px, double py)
{	return zbfs_s_d(px, py, 0);
}

double zbfs_d(double px, double py)
{	return zbfs_s_d(px, py, 1);
}
