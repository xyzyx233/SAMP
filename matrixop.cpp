#include <math.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "matrixop.h"

using namespace std;

/*
*�������
*����a�˾���b,���Ϊ����c
*���룺����a����СΪm*n������b����СΪn*p	���ڲ�������������ͬ�ı������Խ���������Ϊnn*p
*���������c����СΪm*p
*ȫ��ʹ�������ʾ
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
*���������at���������ʾ
*ʹ��LU�ֽ�ķ���������ת��
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
/////////////////////////��L��U�������//////////////////////////////////////////
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
//////////////��r��u��ˣ��õ������
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
*�������򣬱�����������
*���룺����pInArray������ΪnLen
*���������pOutIndex
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
*����������ģ��
*���룺����a������n
*�����norm
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
*�����������ĳ˻�
*���룺����a����СΪm*n������b������Ϊn�����ڲ�������ͬ����������������Ϊnn
*���������c������Ϊm�����ڲ�������ͬ����������Ϊmm
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
*�������
*���룺����a����СΪn������b����СΪn
*���������c����СΪn
*���ڱ�����������������ȡֵ�ֱ�Ϊn��nn��nnn
*/
void s_s(double* a, int n, double* b, int nn, double* c, int nnn) {
	for (int i = 0;i < n;i++)
		c[i] = a[i] - b[i];
}
/*
*�����󲢼�
*���룺����a������la������b������lb
*���������c
*/
bool compare(int a, int b)
{
	return a < b;  //��С��������
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
*��ȡ�����е�һ��
*���룺����a����СΪm*n��ѡȡ����col
*���������b������Ϊm
*/
void get_m_col(double* a, int m, int n, int col, double* b) {
	for (int i = 0;i < m;i++) {
		b[i] = a[i*n+col];
	}
}
/*
*����Ϊ������и�ֵ
*���룺����a����СΪm*n������b����СΪm����ֵ���к�col
*/
void set_m_col(double* a, int m, int n, double* b, int col) {
	for (int i = 0;i < m;i++) {
		a[i*n + col] = b[i];
	}
}
/*
*��ȡ�Ӵ�
*���룺����a����ʼλ��start������λ��end
*���������b
*/
void get_s_sub(double * a, int start, int end,double* b)
{
	for (int i = start;i != end;i++)
		b[i] = a[i];
}
