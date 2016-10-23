#include <math.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "matrixop.h"

using namespace std;

/*
*矩阵相乘
*矩阵a乘矩阵b,结果为矩阵c
*输入：矩阵a，大小为m*n，矩阵b，大小为n*p	由于不能声明两个相同的变量所以将后者声明为nn*p
*输出：矩阵c，大小为m*p
*全部使用数组表示
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
*输出：方阵at，用数组表示
*使用LU分解的方法将矩阵转置
*/
void m_inser(double* a, int n, double* a1) {
	int i, j, k, x;
	double s;
	double** A = new double*[n];
	double** A_ = new double*[n];
	double** L = new double*[n];
	double** U = new double*[n];
	double** u = new double*[n];
	double** r = new double*[n];
	double** out = new double*[n];

	for (int i = 0;i < n;i++) {
		A[i] = new double[n];
		A_[i] = new double[n];
		L[i] = new double[n];
		U[i] = new double[n];
		u[i] = new double[n];
		r[i] = new double[n];
		out[i] = new double[n];
	}

	for (int i = 0;i < n;i++) {
		for (int j = 0;j < n;j++) {
			A_[i][j] = a[i*n + j];
		}
	}

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			A[i][j] = A_[i][j];
			L[i][j] = 0;
			out[i][j] = 0;
			U[i][j] = 0;
			L[i][j] = 0;
			u[i][j] = 0;
			r[i][j] = 0;
		}
	}

	for (i = 0; i < n; i++)
	{
		L[i][i] = 1;
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
/////////////////////////求L和U矩阵的逆//////////////////////////////////////////
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
//////////////将r和u相乘，得到逆矩阵
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

	for (int i = 0;i < n;i++)
		for (int j = 0;j < n;j++)
			a1[i*n + j] = out[i][j];
	for (int i = 0;i < n;i++) {
		delete[] A[i];
		delete[] A_[i];
		delete[] L[i];
		delete[] U[i];
		delete[] u[i];
		delete[] r[i];
		delete[] out[i];
	}
	delete A;
	delete L;
	delete U;
	delete u;
	delete r;
	delete A_;
	delete out;
}
/*
*向量排序，保留数组索引
*输入：向量pInArray，长度为nLen
*输出：向量pOutIndex
*/
void my_sort(double* pInArray,int nLen, int* pOutIndex) //ascending sequence
{
	int i, j, k;
	for (int i = 0;i < nLen;i++) {
		pInArray[i] = fabs(pInArray[i]);
	}
	for (int i = 0;i < nLen;i++)
		pOutIndex[i] = i;
	for (i = 0;i<nLen - 1;i++)
		for (j = i+1;j<nLen;j++)
			if (pInArray[pOutIndex[i]]<pInArray[pOutIndex[j]])
			{
				k = pOutIndex[i];
				pOutIndex[i] = pOutIndex[j];
				pOutIndex[j] = k;
			}

	for (i = 0;i<nLen;i++)
		std::cout << pInArray[pOutIndex[i]] << '\t';
	std::cout << std::endl;
}
/*
*计算向量的模长
*输入：向量a，长度n
*输出：norm
*/
void norm(double * a, int n, double & norm)
{
	double sum = 0;
	for (int i = 0;i < n;i++) {
		sum += a[i] * a[i];
	}
	norm = sqrt(sum);
}
/*
*矩阵与向量的乘积
*输入：矩阵a，大小为m*n，向量b，长度为n，由于不能声明同名变量，长度声明为nn
*输出：向量c，长度为m，由于不能声明同名变量长度为mm
*/
void m_x_v(double* a, int m, int n, double* b, int nn, double* c, int mm) {
	double  sum = 0;
	for (int i = 0;i < m;i++) {
		sum = 0;
		for (int j = 0;j < n;j++) {
			sum += a[i*n + j] * b[j];
		}
		c[i] = sum;
	}
}
/*
*向量相减
*输入：向量a，大小为n，向量b，大小为n
*输出：向量c，大小为n
*由于变量不能重名，所以取值分别为n，nn，nnn
*/
void s_s(double* a, int n, double* b, int nn, double* c, int nnn) {
	for (int i = 0;i < n;i++)
		c[i] = a[i] - b[i];
}
/*
*向量求并集
*输入：向量a、长度la、向量b、长度lb
*输出：向量c
*/
bool compare(int a, int b)
{
	return a < b;  //从小到大排序
}
void s_u_s(int* a, int la, int* b, int lb,int* c) {
	vector<int> v(la + lb);
	vector<int>::iterator it;
	sort(a,a+ la, compare);
	sort(b, b + lb, compare);
	for (int i = 0;i < la;i++)
		cout << a[i] << ' ';
	cout << endl;
	for (int i = 0;i < lb;i++)
		cout << b[i] << ' ';
	cout <<endl;
	it = set_union(a, a + la, b, b + lb, v.begin());
	for (int i = 0;i < la + lb;i++) {
		c[i] = v[i];
	}
}
/*
*提取矩阵中的一列
*输入：矩阵a，大小为m*n，选取的列col
*输出：向量b，长度为m
*/
void get_m_col(double* a, int m, int n, int col, double* b) {
	for (int i = 0;i < m;i++) {
		b[i] = a[i*n+col];
	}
}
/*
*向量为矩阵的列赋值
*输入：矩阵a，大小为m*n，向量b，大小为m，赋值的列号col
*/
void set_m_col(double* a, int m, int n, double* b, int col) {
	for (int i = 0;i < m;i++) {
		a[i*n + col] = b[i];
	}
}
/*
*获取子串
*输入：向量a，开始位置start，结束位置end
*输出：向量b
*/
void get_s_sub(double * a, int start, int end,double* b)
{
	for (int i = start;i != end;i++)
		b[i] = a[i];
}
