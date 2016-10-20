// samp.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <stdlib.h>
#include "matrixop.h"

using namespace std;

int main()
{
	//double* a = new double[3 * 3];
	double a[6] = {1,2,3,4,5,6};
	double c[2] = { 2,3 };
	double* b = new double[3];
	m_x_v(a,3,2,c,2,b,3);
	for(int i=0;i<3;i++)
		cout << b[i] << ' ';
	cout << endl;
    return 0;
}

