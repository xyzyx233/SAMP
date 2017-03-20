// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "prepare.h"

using namespace std;

class p {
public:
	double* Y;
	double* R;
	int m, n;
	double* pixel;
	double* X2;
	int width=256, height=256;
	p(int m, int n) {
		Y = new double[m*n];
		R = new double[m*n];
		pixel = new double[n*n];
		X2 = new double[n*n];
		this->m = m;
		this->n = n;
	}

	void writeY() {
		ofstream outfile;
		outfile.open("F:\outresultY.txt");
		for (int i = 0;i < m;i++) {
			for (int j = 0;j < n;j++) {
				outfile << Y[i * n + j] << '\t';
			}
			outfile << endl;
		}
	}
	void readtu() {
		string file = "tu.txt";
		ifstream infile(file);
		int i = 0;
		while (!infile.eof())
		{
			infile >> pixel[i];
			i++;
		}
		infile.close();             //关闭文件输入流 
	}

	void readX2() {
		string file = "X2.txt";
		ifstream infile(file);
		int i = 0;
		while (!infile.eof())
		{
			infile >> X2[i];
			i++;
		}
		infile.close();             //关闭文件输入流 
	}
	void writeR() {
		ofstream outfile;
		outfile.open("F:\outresultR.txt");
		for (int i = 0;i < m;i++) {
			for (int j = 0;j < n;j++) {
				outfile << R[i * n + j] << '\t';
			}
			outfile << endl;
		}
	}

	void writeX3(double* X3) {
		ofstream outfile;
		outfile.open("F:\X3.txt");
		for (int i = 0;i < n;i++) {
			for (int j = 0;j < n;j++) {
				outfile << X3[i * n + j] << '\t';
			}
			outfile << endl;
		}
	}
	void writeDCT(double* XX) {
		ofstream outfile;
		outfile.open("F:\DCT.txt");
		for (int i = 0;i < n;i++) {
			for (int j = 0;j < n;j++) {
				outfile << XX[i * n + j] << '\t';
			}
			outfile << endl;
		}
	}
};
int main()
{
	p p(190, 256);
	//p.readtu();
	//cout << "read is over" << endl;
	//double* X1 = new double[p.width*p.height];
	//double* X_temp1 = new double[p.width*p.height];
	//double* X_temp = new double[p.width*p.height];
	//int* o = new int[p.width*p.height];
	//DCT(p.pixel,256, X1);
	//cout << "DTC is over" << endl;
	//p.writeDCT(X1);
	//double energy;
	//get_all_sum(X1, p.width, p.height, energy);
	//reshape(X1, p.width, p.height, X_temp1);
	//my_sort(X_temp1, p.width*p.height,o , X_temp);
	//int i_final = 0;
	//double energy_ratio = 0.995;
	//getifinal(energy, X_temp, energy_ratio, i_final, p.width, p.height);
	//cout << "get i_final is over" << endl;
	//set_zero(X1, X_temp, i_final, p.width, p.height);
	//double* RR = new double[190 * p.width];
	//getrandn(190, p.width, RR);
	//getrealR(p.R, 190, p.width, RR);
	//cout << "get real R is over " << endl;
	//m_x_m(p.R, 190, p.width, p.width, X1, p.width, p.height, p.height, p.Y);
	//p.writeR();
	//p.writeY();
	//cout << "OK" << endl;
	p.readX2();
	cout << "read X2 is over" << endl;
	double* X3 = new double[256 * 256];
	IDCT(p.X2, 256, X3);
	p.writeX3(X3);
	cout << "all over" << endl;
    return 0;
}

