#pragma once

void DCT(double * input, int col,double* output);
void IDCT(double* data, int NUM, double* output);
void get_all_sum(double* in, int m, int n, double& summary);
void reshape(double* X1, int m, int n, double* X_temp1);
void my_sorts(double* pInArray, int nLen, int* pOutIndex, double* out);
void getifinal(double energy, double* X_temp, double energy_ratio, int& i_final, int m, int n);
void set_zero(double* X1, double* X_temp, int i_final, int m, int n);
void getrandn(int M, int b, double* R);
void getrealR(double* R, int m, int n, double* RR);