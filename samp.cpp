// samp.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <stdlib.h>
#include "matrixop.h"

using namespace std;

int main()
{
	//double* a = new double[3 * 3];
	double a[4] = {1,2,3,4};
	double c[3] = { 1,4 };
	double* b = new double[7];
	set_m_col(a, 2, 2, c, 0);
	for (int i = 0;i < 4;i++) {
		cout << a[i] << ' ';
		if ((i + 1) % 2 == 0)
			cout << endl;
	}
	cout << endl;
    return 0;
}

