// samp.cpp : �������̨Ӧ�ó������ڵ㡣
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
RGBQUAD strPla[256];//256ɫ��ɫ��
BITMAPINFOHEADER strInfo;

//��ʾλͼ�ļ�ͷ��Ϣ
void showBmpHead(BITMAPFILEHEADER pBmpHead) {
	cout << "λͼ�ļ�ͷ:" << endl;
	cout << "�ļ���С:" << pBmpHead.bfSize << endl;
	cout << "������_1:" << pBmpHead.bfReserved1 << endl;
	cout << "������_2:" << pBmpHead.bfReserved2 << endl;
	cout << "ʵ��λͼ���ݵ�ƫ���ֽ���:" << pBmpHead.bfOffBits << endl << endl;
}

void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead) {
	cout << "λͼ��Ϣͷ:" << endl;
	cout << "�ṹ��ĳ���:" << pBmpInforHead.biSize << endl;
	cout << "λͼ��:" << pBmpInforHead.biWidth << endl;
	cout << "λͼ��:" << pBmpInforHead.biHeight << endl;
	cout << "biPlanesƽ����:" << pBmpInforHead.biPlanes << endl;
	cout << "biBitCount������ɫλ��:" << pBmpInforHead.biBitCount << endl;
	cout << "ѹ����ʽ:" << pBmpInforHead.biCompression << endl;
	cout << "biSizeImageʵ��λͼ����ռ�õ��ֽ���:" << pBmpInforHead.biSizeImage << endl;
	cout << "X����ֱ���:" << pBmpInforHead.biXPelsPerMeter << endl;
	cout << "Y����ֱ���:" << pBmpInforHead.biYPelsPerMeter << endl;
	cout << "ʹ�õ���ɫ��:" << pBmpInforHead.biClrUsed << endl;
	cout << "��Ҫ��ɫ��:" << pBmpInforHead.biClrImportant << endl;
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
		IMAGEDATA *imagedata = NULL;//��̬����洢ԭͼƬ��������Ϣ�Ķ�ά����
		int width, height;//ͼƬ�Ŀ�Ⱥ͸߶�
		fpi = fopen(len.data(), "rb");
		if (fpi != NULL) {
			//�ȶ�ȡ�ļ�����
			WORD bfType;
			fread(&bfType, 1, sizeof(WORD), fpi);
			if (0x4d42 != bfType)
			{
				cout << "the file is not a bmp file!" << endl;
				return ;
			}
			//��ȡbmp�ļ����ļ�ͷ����Ϣͷ
			fread(&strHead, 1, sizeof(tagBITMAPFILEHEADER), fpi);
			//showBmpHead(strHead);//��ʾ�ļ�ͷ
			fread(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), fpi);
			//showBmpInforHead(strInfo);//��ʾ�ļ���Ϣͷ

			//��ȡ��ɫ��
			for (unsigned int nCounti = 0;nCounti<strInfo.biClrUsed;nCounti++)
			{
				fread((char *)&(strPla[nCounti].rgbBlue), 1, sizeof(BYTE), fpi);
				fread((char *)&(strPla[nCounti].rgbGreen), 1, sizeof(BYTE), fpi);
				fread((char *)&(strPla[nCounti].rgbRed), 1, sizeof(BYTE), fpi);
				fread((char *)&(strPla[nCounti].rgbReserved), 1, sizeof(BYTE), fpi);
			}

			width = strInfo.biWidth;
			height = strInfo.biHeight;
			//ͼ��ÿһ�е��ֽ���������4��������
			width = (width * sizeof(IMAGEDATA) + 3) / 4 * 4;
			imagedata = (IMAGEDATA*)malloc(width * height);
			//��ʼ��ԭʼͼƬ����������
			for (int i = 0;i < height;++i)
			{
				for (int j = 0;j < width;++j)
				{
					(*(imagedata + i * width + j)).blue = 0;
				}
			}
			//����ͼƬ����������
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
		//�ͷ��ڴ�
		delete[] imagedata;
	}
	void writepic(double* X3) {
		IMAGEDATA *imagedataRot = NULL;//��̬����洢��ת���ͼƬ��������Ϣ�Ķ�ά����
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
		//����bmpͼƬ
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
		//�����ɫ������
		for (unsigned int nCounti = 0;nCounti<strInfo.biClrUsed;nCounti++)
		{
			fwrite(&strPla[nCounti].rgbBlue, 1, sizeof(BYTE), fpw);
			fwrite(&strPla[nCounti].rgbGreen, 1, sizeof(BYTE), fpw);
			fwrite(&strPla[nCounti].rgbRed, 1, sizeof(BYTE), fpw);
			fwrite(&strPla[nCounti].rgbReserved, 1, sizeof(BYTE), fpw);
		}
		//������������
		for (int i = 0;i < height;++i)
		{
			for (int j = 0;j < width;++j)
			{
				fwrite(&((*(imagedataRot + i * width + j)).blue), 1, sizeof(BYTE), fpw);
			}
		}
		fclose(fpw);
		//�ͷ��ڴ�
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

