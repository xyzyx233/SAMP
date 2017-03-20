
#include <iostream>
#include <math.h>
#include "matrixop.h"

#define PI 3.1415926

// DCT - Discrete Cosine Transform  

void DCT(double* data,int NUM,double* output)
{
	double ALPHA, BETA;
	short u = 0;
	short v = 0;
	short i = 0;
	short j = 0;
	for (u = 0; u < NUM; u++)
	{
		for (v = 0; v < NUM; v++)
		{
			if (u == 0)
			{
				ALPHA = sqrt(1.0 / NUM);
			}
			else
			{
				ALPHA = sqrt(2.0 / NUM);
			}
			if (v == 0)
			{
				BETA = sqrt(1.0 / NUM);
			}
			else
			{
				BETA = sqrt(2.0 / NUM);
			}
			double tmp = 0.0;
			for (i = 0; i < NUM; i++)
			{
				for (j = 0; j < NUM; j++)
				{
					tmp += data[i*NUM+j] * cos((2 * i + 1)*u*PI / (2.0 * NUM)) * cos((2 * j + 1)*v*PI / (2.0 * NUM));
				}
			}
			output[u*NUM+v] = round(ALPHA * BETA * tmp);
		}
		std::cout << u << std::endl;
	}
}
// Inverse DCT
void IDCT(double* data,int NUM,double* output)
{
	double ALPHA, BETA;
	short u = 0;
	short v = 0;
	short i = 0;
	short j = 0;
	for (i = 0; i < NUM; i++)
	{
		for (short j = 0; j < NUM; j++)
		{
			double tmp = 0.0;
			for (short u = 0; u < NUM; u++)
			{
				for (v = 0; v < NUM; v++)
				{
					if (u == 0)
					{
						ALPHA = sqrt(1.0 / NUM);
					}
					else
					{
						ALPHA = sqrt(2.0 / NUM);
					}
					if (v == 0)
					{
						BETA = sqrt(1.0 / NUM);
					}
					else
					{
						BETA = sqrt(2.0 / NUM);
					}
					tmp += ALPHA * BETA * data[u*NUM+v] * cos((2 * i + 1)*u*PI / (2.0 * NUM)) * cos((2 * j + 1)*v*PI / (2.0 * NUM));
				}
			}
			output[i*NUM+j] = round(tmp);
		}
		std::cout << i << std::endl;
	}
}
void get_all_sum(double* in, int m,int n,double& summary ) {
	double* temp = new double[m*n];
	double sum=0;
	for (int i = 0;i < m*n;i++) {
		temp[i] = in[i] * in[i];
	}
	for (int i = 0;i < m*n;i++) {
		sum += temp[i];
	}
	summary = sum;
}

void reshape(double* X1, int m, int n, double* X_temp1) {
	int k = 0;
	for (int i = 0;i < m;i++) {
		for (int j = 0;j < n;j++) {
			X_temp1[k] = X1[j*n+i];
			k++;
		}
	}
}
/*
*向量排序，保留数组索引
*输入：向量pInArray，长度为nLen
*输出：向量pOutIndex,向量out为排序后的结果
*/
void my_sorts(double* pInArray, int nLen, int* pOutIndex,double* out) //ascending sequence
{
	int i, j, k;
	double kk;
	double* tout = new double[nLen];
	for (int i = 0;i < nLen;i++) {
		pInArray[i] = fabs(pInArray[i]);
	}
	for (int i = 0;i < nLen;i++)
		tout[i] = pInArray[i];
	for (int i = 0;i < nLen;i++)
		pOutIndex[i] = i;
	for (i = 0;i<nLen - 1;i++)
		for (j = i + 1;j<nLen;j++)
			if (pInArray[pOutIndex[i]]<pInArray[pOutIndex[j]])
			{
				k = pOutIndex[i];
				kk = tout[i];
				pOutIndex[i] = pOutIndex[j];
				tout[i] = tout[j];
				pOutIndex[j] = k;
				tout[j] = kk;
			}
	for (int i = 0;i < nLen;i++) {
		out[i] = tout[i];
	}
	delete(tout);
}
void getifinal(double energy,double* X_temp,double energy_ratio,int& i_final,int m,int n) {
	double energy_temp = 0;
	i_final = 0;
	for (int i = 0;i < m*n; i++) {
		if (energy_temp <= energy*energy_ratio) {
			energy_temp = energy_temp + X_temp[i] * X_temp[i];
		}
		else {
			i_final = i;
			break;
		}
	}
}
void set_zero(double* X1,double* X_temp,int i_final,int m,int n) {
	for (int i = 0;i < m*n;i++) {
		if (fabs(X1[i]) < fabs(X_temp[i_final])) {
			X1[i] = 0;
		}
	}
}

double gaussrand() {
	static double U, V;
	static int phase = 0;
	double z;

	if (phase == 0)
	{
		U = (rand() + 1.1) / (RAND_MAX + 2.);
		V = rand() / (RAND_MAX + 1.);
		z = sqrt(-1 * log(U))* sin(2 * 3.141592654 * V);
	}
	else
	{
		z = sqrt(-2 * log(U)) * cos(2 * 3.141592654 * V);
	}

	phase = 1 - phase;
	return z;
}
void getrandn(int M,int b,double* R) {
	for (int i = 0;i < M*b;i++) {
		R[i] = gaussrand();
	}
}
void getrealR(double* R, int m, int n, double* RR) {
	double* tempcol = new double[m];
	double nor;
	for (int i = 0;i < n;i++) {
		get_m_col(RR, m, n, i, tempcol);
		norm(tempcol,m,nor);
		for (int j = 0;j < m;j++) {
			R[j*n + i] = RR[j*n + i] / nor;
		}
	}
}