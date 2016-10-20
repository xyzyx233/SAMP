#include <math.h>
#include "matrixop.h"
/*
*矩阵相乘
矩阵a乘矩阵b,结果为矩阵c
矩阵a大小为m*n，矩阵b大小为n*p由于不能声明两个相同的变量所以将后者声明为nn*p
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
*矩阵转置
*将矩阵a转置为at
*矩阵大小为m*n;
*有问题 需要改！！！
*/
void m_t(double* a, int m, int n, double* at) {
	int i, j;
	for(i=0;i<m;i++)
		for (j = 0;j < n;j++) {
			at[j*m+i] = a[i*n+j];
		}
}
/*
*方阵求逆
*输入：方阵a，大小为n*n
*使用LU分解的方法将矩阵转置
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
	//直接三角分解
	//首先计算矩阵L的第1列，矩阵U的第1行
	for (i = 0; i < n; i++)
	{
		U[0][i] = A[0][i];
		if (i != 0)
		{
			L[i][0] = A[i][0] / U[0][0];
		}
	}
	//递推求矩阵L第r行和矩阵U第r列
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
	/////////////////////求L和U矩阵的逆//////////////////////////////////////////
	for (i = 0; i<n; i++) /*求矩阵U的逆 */
	{
		u[i][i] = 1 / U[i][i];//对角元素的值，直接取倒数
		for (k = i - 1; k >= 0; k--)
		{
			s = 0;
			for (j = k + 1; j <= i; j++)
				s = s + U[k][j] * u[j][i];
			u[k][i] = -s / U[k][k];//迭代计算，按列倒序依次得到每一个值，
		}
	}
	for (i = 0; i<n; i++) //求矩阵L的逆
	{
		r[i][i] = 1; //对角元素的值，直接取倒数，这里为1
		for (k = i + 1; k<n; k++)
		{
			for (j = i; j <= k - 1; j++)
				r[k][i] = r[k][i] - L[k][j] * r[j][i];   //迭代计算，按列顺序依次得到每一个值
		}
	}
	//////////将r和u相乘，得到逆矩阵
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