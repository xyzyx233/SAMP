#pragma once

void m_x_m(double* a, int m, int n,int realn, double* b, int nn, int p,int realp, double* c);
void m_t(double* a, int m, int n, double* at);
void m_inser(double* a, int n, double* a1);
void my_sort(double *a, int n, int* b);
void norm(double* a, int n, double& norm);
void m_x_v(double* a, int m, int n, double* b, int nn, double* c, int mm);
void s_s(double* a, int n, double* b, int nn, double* c, int nnn);
void s_u_s(int* a, int la, int* b, int lb, int* c,int& clen);
void get_m_col(double* a, int m, int n, int col, double* b);
void set_m_col(double* a, int m, int n, double* b, int col);
void get_s_sub(int* a, int start, int end,int* b);
void get_last_zero(int* a, int len, int* x);
void m_fin(int lenofactiveaetnew, double* Aug_t, int m, int n, double* s, int sl, double* temp7, int ltemp7);