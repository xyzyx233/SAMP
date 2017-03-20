#include "matrixop.h"
#include <math.h>
#include<iostream>
/*
*SAMP
*SAMP算法
*输入：原始压缩的矩阵 orginmatrix 恢复后的矩阵 aftermatrix 观察矩阵 R 观测矩阵大小 m*n 190*256
*全部使用数组表示
*/
void real_samp(double* orginmatrix, double* aftermatrix, double* R, int m, int n, int om) {
	double* Aug_t = new double[n*n];
	double* temp3 = new double[4 * m*m];
	double* temp4 = new double[n*n];
	double* temp5 = new double[4 * m*m];
	double* temp6 = new double[2 * m*m];
	double* temp7 = new double[2 * m];
	for (int i = 0;i < n;i++)
	{
		std::cout << "第 " << i + 1 << " 次samp还原" << std::endl;
		double* s = new double[m];
		int step = 3;
		int step_whole = step;
		int k = 1;
		int stage = 1;

		int* candidate_set_formal = new int[m];
		for (int i = 0;i < m;i++)
			candidate_set_formal[i] = -1;
		int* candidate_set_final = new int[m];
		for (int i = 0;i < m;i++)
			candidate_set_final[i] = -1;
		double* R_t = new double[m*n];
		m_t(R, m, n, R_t);
		double* res_formal = new double[m];
		get_m_col(orginmatrix, m, n, i, s);
		get_m_col(orginmatrix, m, n, i, res_formal);
		double* xr_final = new double[n*n];
		for (int i = 0;i < n*n;i++) {
			xr_final[i] = 0;
		}
		double norm1 = 0;
		double norm2 = 0;
		while (((norm1 - norm2) <= 0) || ((norm1 - norm2) >= 0.001) || stage <= 2)
		{
			//Aug_t=zeros(N,N);
			//double* Aug_t = new double[n*n];
			for (int i = 0;i < n*n;i++) {
				Aug_t[i] = 0;
			}
			//[~, idx1]=sort(abs(T'*res_formal),'descend');
			double* temp1 = new double[n];
			m_x_v(R_t, n, m, res_formal, m, temp1, m);
			for (int i = 0;i < n;i++)
				temp1[i] = fabs(temp1[i]);
			int* idx1 = new int[n];
			my_sort(temp1, n, idx1);
			delete(temp1);
			//idx2=idx1(1:step_whole);
			int* idx2 = new int[step_whole];
			get_s_sub(idx1, 0, step_whole - 1, idx2);
			delete(idx1);
			//active_set_new = union(idx2, candidate_set_formal);
			int* active_set_new = new int[2 * step_whole];
			int lenofactiveaetnew = 0;
			s_u_s(idx2, step_whole, candidate_set_formal, step_whole, active_set_new, lenofactiveaetnew);
			/*get_last_zero(active_set_new, 2 * step_whole, &lenofactiveaetnew);*/
			delete(idx2);
			//Aug_t = T(:, active_set_new);
			double* temp2 = new double[n];
			for (int i = 0;i < n;i++) {
				temp2[i] = 0;
			}
			for (int i = 0;i < lenofactiveaetnew;i++) {
				get_m_col(R, m, n, active_set_new[i], temp2);
				set_m_col(Aug_t, n, n, temp2, i);
			}
			delete(temp2);
			//Xp = zeros(N, 1);
			double* Xp = new double[n];
			for (int i = 0;i < n;i++)
				Xp[i] = 0;
			//Xp(active_set_new) = (Aug_t'*Aug_t)^(-1)*Aug_t'*s;
			//double* temp3 = new double[lenofactiveaetnew*lenofactiveaetnew];//temp3=Aug_t'*Aug_t
			//double* temp4 = new double[n*n];
			m_t(Aug_t, n, n, temp4);//temp4=Aug_t'
			m_x_m(temp4, lenofactiveaetnew, m, n, Aug_t, m, lenofactiveaetnew, n, temp3);
			//double* temp5 = new double[lenofactiveaetnew*lenofactiveaetnew];//temp5=(Aug_t'*Aug_t)^(-1)
			m_inser(temp3, lenofactiveaetnew, temp5);
			//double* temp6 = new double[lenofactiveaetnew*m];//temp6=(Aug_t'*Aug_t)^(-1)*Aug_t'
			m_x_m(temp5, lenofactiveaetnew, lenofactiveaetnew, lenofactiveaetnew, temp4, lenofactiveaetnew, m, n, temp6);
			//double* temp7 = new double[lenofactiveaetnew];
			m_x_v(temp6, lenofactiveaetnew, m, s, m, temp7, lenofactiveaetnew);
			for (int i = 0;i < lenofactiveaetnew;i++) {
				Xp[active_set_new[i]] = temp7[i];
			}
			//delete(temp3);
			//delete(temp4);
			//delete(temp5);
			//delete(temp6);
			//delete(temp7);
			delete(active_set_new);
			//[~, idx3] = sort(abs(Xp), 'descend');
			double* temp8 = new double[n];
			for (int i = 0;i < n;i++)
				temp8[i] = fabs(Xp[i]);
			int* idx3 = new int[n];
			my_sort(temp8, n, idx3);
			delete(temp8);
			//candidate_set_final = idx3(1:step_whole);
			get_s_sub(idx3, 0, step_whole - 1, candidate_set_final);
			delete(idx3);
			//Xq = zeros(N, 1);
			double* Xq = new double[n];
			for (int i = 0;i < n;i++)
				Xq[i] = 0;
			//Xq(candidate_set_final) = Xp(candidate_set_final);    % recovered x with s sparsity
			for (int i = 0;i < step_whole;i++) {
				Xq[candidate_set_final[i]] = Xp[candidate_set_final[i]];
			}
			delete(Xp);
			//	res_later = s - T*Xq;
			double* temp9 = new double[m];
			m_x_v(R, m, n, Xq, n, temp9, m);
			double* res_later = new double[m];
			s_s(s, m, temp9, m, res_later, m);
			delete(temp9);
			double res_1, res_2;
			//res_1 = norm(res_later);
			norm(res_later, m, res_1);
			//res_2 = norm(res_formal);
			norm(res_formal, m, res_2);
			//if res_1 >= res_2
			if (res_1 >= res_2) {
				//	xr_final(:, stage) = Xq;
				set_m_col(xr_final, n, n, Xq, stage);
				delete(Xq);
				//if stage >= 2
				if (stage >= 2) {
					//	norm1 = norm(xr_final(:, stage));
					double* temp10 = new double[n];
					get_m_col(xr_final, n, n, stage, temp10);
					norm(temp10, n, norm1);
					//norm2 = norm(xr_final(:, stage - 1));
					double* temp11 = new double[n];
					get_m_col(xr_final, n, n, stage - 1, temp11);
					norm(temp11, n, norm2);
					//end
					delete(temp10);
					delete(temp11);
				}
				//	stage = stage + 1;
				stage++;
				//step_whole = stage*step;
				step_whole = stage*step;
				//if (step_whole)>M;
				if (step_whole > m) {
					//break;
					break;
					//end
				}
			}
			//else
			else {
				//	candidate_set_formal = candidate_set_final;
				for (int i = 0;i < m;i++) {
					candidate_set_formal[i] = candidate_set_final[i];
				}
				//res_formal = res_later;
				for (int i = 0;i < m;i++) {
					res_formal[i] = res_later[i];
				}
				delete(res_later);
				//end
			}
			//	end
			//k = k + 1;
			k = k + 1;
		}
		//	xr = xr_final(:, stage - 1);
		double* xr = new double[n];
		get_m_col(xr_final, n, n, stage - 1, xr);
		set_m_col(aftermatrix, om, n, xr, i);
		delete(s);
		delete(candidate_set_final);
		delete(candidate_set_formal);
		delete(R_t);
		delete(res_formal);
		delete(xr_final);
	}
}