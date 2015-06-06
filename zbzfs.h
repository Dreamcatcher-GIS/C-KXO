#ifndef ZBZS_H
#define ZBZS_H

#define N_MAX 16

extern double ss;	// ���׮��
extern double es;	// �յ�׮��
extern double sx;	// �������X
extern double sy;	// �յ�����Y
extern double sa;	// ������귽λ��(DEG)
extern int v;		// ��Ԫת��,��תΪ1,��תΪ-1
extern double sp;	// ��Ԫ�������,ֱ��Ϊ0,Բ����Ϊ1/R
extern double ep;	// ��Ԫ�յ�����,ֱ��Ϊ0,Բ����Ϊ1/R

	// ����ȷֵ㴦׮��
	// ����n�ȷִ�ʱnȡ2,4,6..
	// ����2n�ȷִ�ʱnȡ1,3,5...
	extern double s_function(double s, int n);
	// �������s������
	extern double p_function(double s);
	// �������s�����߷�λ��(DEG)
	extern double a_function(double p, double s);

	// �������s������X/Y�����귽λ��A(DEG)
	extern double zbzs_m_xy(double s, int xy);
	extern double zbzs_m_a(double s);
	// ��������X/Y��Ӧ����̼�ƫ��
	extern double zbfs_s_d(double px, double py, int sd);
//
//public:
//	// ���캯�������ݳ�ʼ��
   extern  void intialCZb(double,double ,double ,double,double,int,double,double);
//	
//	~CZb(void);
//	
	// �������stake/ƫ��distance/�Ƕ�angle�������꼰���귽λ��(DEG)
	extern double zbzs_x(double stake, double distance,double angle);
	extern double zbzs_y(double stake, double distance,double angle);
	extern double zbzs_a(double stake, double distance,double angle);
	// ��������Ϊpx/py������̼�ƫ��
	extern double zbfs_s(double px, double py);
	extern double zbfs_d(double px, double py);
#endif // ZBZS_H
