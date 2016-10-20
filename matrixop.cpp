#include <math.h>
#include "matrixop.h"
/*
*�������
����a�˾���b,���Ϊ����c
����a��СΪm*n������b��СΪn*p���ڲ�������������ͬ�ı������Խ���������Ϊnn*p
*/
void m_x_m(double* a,int m,int n,double* b, int nn,int p,double* c) {
	int i, j, k;
	double sum;
	for (i = 0;i < m;i++) {
		for (j = 0;j < p;j++) {
			sum = 0;
			for (k = 0;k < n;k++) {
				sum += a[i*n+k] * b[k*p+j];
			}
			c[i*p + j] = sum;
		}
	}
}
/*
*����ת��
*������aת��Ϊat
*�����СΪm*n;
*������ ��Ҫ�ģ�����
*/
void m_t(double* a, int m, int n, double* at) {
	int i, j;
	for(i=0;i<m;i++)
		for (j = 0;j < n;j++) {
			at[j*m+i] = a[i*n+j];
		}
}
/*
*��������
*���룺����a����СΪn*n
*ʹ��LU�ֽ�ķ���������ת��
*/
void m_inser(double* a, int n, double* a1) {

	//double **out;
	int i, j, k, x;
	double s;
	//out = (double**)malloc(n*sizeof(double*));
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			A[i][j] = A_[i][j];
			L[i][j] = 0;
		}
	}
	for (i = 0; i < n; i++)
	{
		//out[i] = (double*)malloc(n*sizeof(double));
		L[i][i] = 1;
	}
	for (i = 0;i<n;i++)
		for (j = 0;j < n;j++) {
			U[i][j] = 0;
			L[i][j] = 0;
			u[i][j] = 0;
			r[i][j] = 0;
		}
	//ֱ�����Ƿֽ�
	//���ȼ������L�ĵ�1�У�����U�ĵ�1��
	for (i = 0; i < n; i++)
	{
		U[0][i] = A[0][i];
		if (i != 0)
		{
			L[i][0] = A[i][0] / U[0][0];
		}
	}
	//���������L��r�к;���U��r��
	for (x = 1; x < n; x++)
	{
		for (int i = x; i < n; i++)
		{
			double tmp1 = 0;
			double tmp2 = 0;
			for (int k = 0; k < x; k++)
			{
				tmp1 += L[x][k] * U[k][i];
				if (i > x)
					tmp2 += L[i][k] * U[k][x];
			}
			U[x][i] = A[x][i] - tmp1;
			if (i > x)
				L[i][x] = (A[i][x] - tmp2) / U[x][x];
		}
	}
	/////////////////////��L��U�������//////////////////////////////////////////
	for (i = 0; i<n; i++) /*�����U���� */
	{
		u[i][i] = 1 / U[i][i];//�Խ�Ԫ�ص�ֵ��ֱ��ȡ����
		for (k = i - 1; k >= 0; k--)
		{
			s = 0;
			for (j = k + 1; j <= i; j++)
				s = s + U[k][j] * u[j][i];
			u[k][i] = -s / U[k][k];//�������㣬���е������εõ�ÿһ��ֵ��
		}
	}
	for (i = 0; i<n; i++) //�����L����
	{
		r[i][i] = 1; //�Խ�Ԫ�ص�ֵ��ֱ��ȡ����������Ϊ1
		for (k = i + 1; k<n; k++)
		{
			for (j = i; j <= k - 1; j++)
				r[k][i] = r[k][i] - L[k][j] * r[j][i];   //�������㣬����˳�����εõ�ÿһ��ֵ
		}
	}
	//////////��r��u��ˣ��õ������
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n; j++)
		{
			out[i][j] = 0;
		}
	}
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n; j++)
		{
			for (k = 0; k<n; k++)
			{
				out[i][j] += u[i][k] * r[k][j];
			}
		}
	}
}