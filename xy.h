////////////////////////////
// �ļ�����xy.h
//
// ���ܣ��������
///////////////////////////

#ifndef XY_H
#define XY_H

// ��֪���/ƫ��/�Ƕȼ�������
// xya=0����x;xya=1����y;xya=n����a;�������㷶Χ����-1e10
double sd_to_xya( double stake, double distance, double angle, int xy );

// ��֪���귴����̼�ƫ��
// sd=0����ֵΪ���,sd=1����ֵΪƫ��(��ֵΪ��ƫ/��ֵΪ��ƫ),�������㷶Χ����1e10
double xy_to_sd( double PX, double PY, int sd );

#endif	// XY_H
