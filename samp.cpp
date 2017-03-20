// samp.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include "BmpRot.h"
#include "matrixop.h"
#include "sampop.h"
#include "prepare.h"

using namespace std;

BITMAPFILEHEADER strHead;
RGBQUAD strPla[256];//256色调色板
BITMAPINFOHEADER strInfo;

//显示位图文件头信息
void showBmpHead(BITMAPFILEHEADER pBmpHead) {
	cout << "位图文件头:" << endl;
	cout << "文件大小:" << pBmpHead.bfSize << endl;
	cout << "保留字_1:" << pBmpHead.bfReserved1 << endl;
	cout << "保留字_2:" << pBmpHead.bfReserved2 << endl;
	cout << "实际位图数据的偏移字节数:" << pBmpHead.bfOffBits << endl << endl;
}

void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead) {
	cout << "位图信息头:" << endl;
	cout << "结构体的长度:" << pBmpInforHead.biSize << endl;
	cout << "位图宽:" << pBmpInforHead.biWidth << endl;
	cout << "位图高:" << pBmpInforHead.biHeight << endl;
	cout << "biPlanes平面数:" << pBmpInforHead.biPlanes << endl;
	cout << "biBitCount采用颜色位数:" << pBmpInforHead.biBitCount << endl;
	cout << "压缩方式:" << pBmpInforHead.biCompression << endl;
	cout << "biSizeImage实际位图数据占用的字节数:" << pBmpInforHead.biSizeImage << endl;
	cout << "X方向分辨率:" << pBmpInforHead.biXPelsPerMeter << endl;
	cout << "Y方向分辨率:" << pBmpInforHead.biYPelsPerMeter << endl;
	cout << "使用的颜色数:" << pBmpInforHead.biClrUsed << endl;
	cout << "重要颜色数:" << pBmpInforHead.biClrImportant << endl;
}


class Pre {
public:
	double* Y;
	double* R;
	int m, n,an;
	double* pixel;
	double* after;
	FILE *fpi, *fpw;
	int width, height;
	Pre(int m, int n) {
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
	void writeP() {
		ofstream outfile;
		outfile.open("F:\ppp.txt");
		for (int i = 0;i < m;i++) {
			for (int j = 0;j < n;j++) {
				outfile << pixel[i * n + j] << '\t';
			}
			outfile << endl;
		}
	}
	void writeX1(double* X1) {
		ofstream outfile;
		outfile.open("F:\outX1.txt");
		for (int i = 0;i < m;i++) {
			for (int j = 0;j < n;j++) {
				outfile << X1[i * n + j] << '\t';
			}
			outfile << endl;
		}
	}
	void readtu(string len= "lena256.bmp") {
		IMAGEDATA *imagedata = NULL;//动态分配存储原图片的像素信息的二维数组
		int width, height;//图片的宽度和高度
		fpi = fopen(len.data(), "rb");
		if (fpi != NULL) {
			//先读取文件类型
			WORD bfType;
			fread(&bfType, 1, sizeof(WORD), fpi);
			if (0x4d42 != bfType)
			{
				cout << "the file is not a bmp file!" << endl;
				return ;
			}
			//读取bmp文件的文件头和信息头
			fread(&strHead, 1, sizeof(tagBITMAPFILEHEADER), fpi);
			//showBmpHead(strHead);//显示文件头
			fread(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), fpi);
			//showBmpInforHead(strInfo);//显示文件信息头

			//读取调色板
			for (unsigned int nCounti = 0;nCounti<strInfo.biClrUsed;nCounti++)
			{
				fread((char *)&(strPla[nCounti].rgbBlue), 1, sizeof(BYTE), fpi);
				fread((char *)&(strPla[nCounti].rgbGreen), 1, sizeof(BYTE), fpi);
				fread((char *)&(strPla[nCounti].rgbRed), 1, sizeof(BYTE), fpi);
				fread((char *)&(strPla[nCounti].rgbReserved), 1, sizeof(BYTE), fpi);
			}

			width = strInfo.biWidth;
			height = strInfo.biHeight;
			//图像每一行的字节数必须是4的整数倍
			width = (width * sizeof(IMAGEDATA) + 3) / 4 * 4;
			imagedata = (IMAGEDATA*)malloc(width * height);
			//初始化原始图片的像素数组
			for (int i = 0;i < height;++i)
			{
				for (int j = 0;j < width;++j)
				{
					(*(imagedata + i * width + j)).blue = 0;
				}
			}
			//读出图片的像素数据
			fread(imagedata, sizeof(struct tagIMAGEDATA) * width, height, fpi);
			fclose(fpi);
		}
		else
		{
			cout << "file open error!" << endl;
			return ;
		}
		this->width = width;
		this->height = height;
		n = height;
		after = new double[width*height];
		Y = new double[m*n];
		R = new double[m*n];
		pixel = new double[n*n];
		an = width;
		for (int i = 0;i < width * height;i++) {
			pixel[i] = (double)imagedata[i].blue;
		}
		//释放内存
		delete[] imagedata;
	}
	void writepic(double* X3) {
		IMAGEDATA *imagedataRot = NULL;//动态分配存储旋转后的图片的像素信息的二维数组
		imagedataRot = (IMAGEDATA*)malloc(width * height);
		//for (int i = 0;i < width;i++) {
		//	for (int j = 0;j < height;j++) {
		//		if (X3[i*width+j] > 255) {
		//			X3[i*width + j] = 255;
		//		}
		//		else if (X3[i*width+j] < 0) {
		//			X3[i*width + j] = 0;
		//		}
		//		else {
		//			ans1[i][j] = anss[i][j];
		//		}
		//	}
		//}
		for (int i = 0;i < width * height;i++) {
			imagedataRot[i].blue = (BYTE)X3[i];
		}
		//保存bmp图片
		if ((fpw = fopen("out.bmp", "wb")) == NULL)
		{
			cout << "create the bmp file error!" << endl;
			return ;
		}
		WORD bfType_w = 0x4d42;
		fwrite(&bfType_w, 1, sizeof(WORD), fpw);
		//fpw +=2;
		fwrite(&strHead, 1, sizeof(tagBITMAPFILEHEADER), fpw);
		strInfo.biWidth = width;
		strInfo.biHeight = height;
		fwrite(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), fpw);
		//保存调色板数据
		for (unsigned int nCounti = 0;nCounti<strInfo.biClrUsed;nCounti++)
		{
			fwrite(&strPla[nCounti].rgbBlue, 1, sizeof(BYTE), fpw);
			fwrite(&strPla[nCounti].rgbGreen, 1, sizeof(BYTE), fpw);
			fwrite(&strPla[nCounti].rgbRed, 1, sizeof(BYTE), fpw);
			fwrite(&strPla[nCounti].rgbReserved, 1, sizeof(BYTE), fpw);
		}
		//保存像素数据
		for (int i = 0;i < height;++i)
		{
			for (int j = 0;j < width;++j)
			{
				fwrite(&((*(imagedataRot + i * width + j)).blue), 1, sizeof(BYTE), fpw);
			}
		}
		fclose(fpw);
		//释放内存
		delete[] imagedataRot;
	}
	void writeafter() {
		ofstream outfile;
		outfile.open("F:\outresult.txt");
		for (int i = n-1;i >=0 ;i--) {
			for (int j = 0;j < n;j++) {
				outfile << after[i * n + j] << '\t';
			}
			outfile << endl;
		}
	}
	void writeX3(double* X3) {
		ofstream outfile;
		outfile.open("F:\Pix.txt");
		for (int i = n - 1;i >= 0;i--) {
			for (int j = 0;j < n;j++) {
				outfile << X3[i * n + j] << '\t';
			}
			outfile << endl;
		}
	}
};
int main()
{
	Pre p(190,256);
	p.readtu();
	cout << "read is over" << endl;
	double* X1 = new double[p.width*p.height];
	double* X_temp1 = new double[p.width*p.height];
	double* X_temp = new double[p.width*p.height];
	int* o = new int[p.width*p.height];
	DCT(p.pixel,256, X1);
	cout << "DTC is over" << endl;
	double energy;
	get_all_sum(X1, p.width, p.height, energy);
	reshape(X1, p.width, p.height, X_temp1);
	my_sorts(X_temp1, p.width*p.height,o , X_temp);
	int i_final = 0;
	double energy_ratio = 0.995;
	getifinal(energy, X_temp, energy_ratio, i_final, p.width, p.height);
	cout << "get i_final is over" << endl;
	set_zero(X1, X_temp, i_final, p.width, p.height);
	double* RR = new double[190 * p.width];
	getrandn(190, p.width, RR);
	getrealR(p.R, 190, p.width, RR);
	cout << "get real R is over " << endl;
	//p.writeR();
	m_x_m(p.R, 190, p.width, p.width, X1, p.height, p.width, p.width, p.Y);
	//p.writeY();
	real_samp(p.Y, p.after, p.R, p.m, p.n, p.an);
	//p.writeafter();
	cout << "samp is OK" << endl;
	double* X3 = new double[p.width * p.height];
	IDCT(p.after, 256, X3);
	//p.writeX3(X3);
	cout << "all over" << endl;
	cout << "printing picture" << endl;
	p.writepic(X3);
	cout << "all done!" << endl;
    return 0;
}

