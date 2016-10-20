// samp.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <stdlib.h>
#include "matrixop.h"

using namespace std;

int main()
{
	double* a = new double[3 * 2];
	double* c = new double[3 * 2];
	for (int i = 0;i < 6;i++) {
		a[i] = i + 1;
	}
	m_t(a,2,3,c);
	for (int i = 0;i < 6;i++)
		cout << c[i] << " ";
	cout << endl;
    return 0;
}

