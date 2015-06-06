#include <stdio.h>
#include <math.h>
#include <string.h>
#include <math.h>

#define PI 3.14159265358979323846
// ��������stod
// ��  ����double stod(const std::string &s);
// ��  �����ַ����͵�����
// ����ֵ��˫�����͵�����
double stod(const char *s)
{	
	double value;
	value=atof(s);
	return value;
}

// ��������stoi
// ��  ����int stoi(const std::string &s);
// ��  �����ַ����͵�����
// ����ֵ�����͵�����
int stoi(const char *s)
{	int value;
	value = atoi(s);
	return value;
}

// ��������deg
// ��  ����double deg(double dms);
// ��  �����Զȷ���Ϊ��λ�ĽǶ�
// ����ֵ���ԽǶ�Ϊ��λ�ĽǶ�
double deg(double dms)
{	double d=0, m=0, s=0;//��λ,��λ,��λ
	double zs=0, xs=0;
	xs = modf(dms,&zs);		d = zs;		xs = xs * 100.0000000001;
	xs = modf(xs,&zs);		m = zs / 60;
	 s = xs / 36;
	return(d+m+s);
}

// ��������dms  60����С���㻻DMS��ʽ��ʾ
// ��  ����double dms(double deg);
// ��  �����ԽǶ�Ϊ��λ�ĽǶ�
// ����ֵ���Զȷ���Ϊ��λ�ĽǶ�
double dms(double deg)
{	double zs=0,xs=0;
	double d=0, m=0, s=0;

	xs=modf(deg,&zs);		d=zs;
	xs=modf(xs*60.0000000001,&zs);		m=zs;
	s=xs*60;

	return(d+m*0.01+s*0.0001);
}

// ��������rad
// ��  ����double  rad(double deg);
// ��  �����ԽǶ�Ϊ��λ�ĽǶ�
// ����ֵ���Ի���Ϊ��λ�ĽǶ�
double  rad(double deg)
{	return( deg * PI / 180);
}

// ��������arad
// ��  ����double arad(double rad);
// ��  �����Ի���Ϊ��λ�ĽǶ�
// ����ֵ���ԽǶ�Ϊ��λ�ĽǶ�
double arad(double rad)
{	return( rad * 180 / PI);
}

// ��������sign
// ��  ����double sign(double value);
// ��  ��������
// ����ֵ�����ֵķ��ţ����ŷ���+1�����ŷ���-1
double sign(double value)
{	if(value<0)
	{	return -1;
	}
	else
	{	return +1;
	}
}

// �������귽λ�ǣ�����ֵΪ�Ƕȣ�
// ��������azimuth
// ��  ����double azimuth(double x1, double y1, double x2, double y2);
// ��  ����P1������(x1,y1)��P2������(x2,y2)
// ����ֵ��P1�㵽P2�����귽λ��
double azimuth(double x1, double y1, double x2, double y2)
{	double dx = x2 - x1;
	double dy = y2 - y1;

	if( dy>=0 && dx>0 )
	{	return       arad( atan( fabs( dy/dx ) ) );
	}	
	if( dy>=0 && dx<0 )
	{	return 180 - arad( atan( fabs( dy/dx ) ) );
	}
	if( dy<0 && dx<0 )
	{	return 180 + arad( atan( fabs( dy/dx ) ) );
	}
	if( dy<0 && dx>0 )
	{	return 360 - arad( atan( fabs( dy/dx ) ) );
	}
	if( dy>0 && dx==0 )
	{	return 90;
	}
	if( dy<0 && dx==0 )
	{	return 270;
	}
	
	return -1;
}