#include "matlab.h"
# include "sys.h"
#include "arm_math.h"

//#define   PI  3.14159265358979
#define   M_FIR   2     //����FIR�˲�������
#define   lamda   0.99  //������������
#define   c       0.01  //С���� ��֤����P������
#define   fs   256.4

u8 I[M_FIR][M_FIR] = {1,0,1,0};    //  ���ɶ�Ӧ�ĵ�λ����
float Eta_out[2048] = {0};

void  Romove_powerline_interference(float in_data[2048],float out_data[2048],u16 peak_50)
{
	 
	  u16 len = 0,t = 0,stmp = 0;
	  u16 k, k2, k4;
		
		
		float  P_last[M_FIR][M_FIR] = {0}; 
		float  w_last[M_FIR][1] = {0}; 

	  float d = 0,res = 0,Eta = 0,y = 0; 
	  float Interference[M_FIR][1] = {0};
		float Interference1[1][M_FIR] = {0};
		
		float  M[M_FIR][1] = {0};
		float  N[1][M_FIR] = {0};
		
		float  K[M_FIR][1] = {0};
		float  K1[M_FIR][1] = {0};
		
		float  W[M_FIR][1] = {0};
		
		float  P1[M_FIR][M_FIR] = {0};
		float  P2[M_FIR][M_FIR] = {0};
		float  P3[M_FIR][M_FIR] = {0};
		float  P[M_FIR][M_FIR] = {0};
		

		
	  for(len=0;len<2048;len++)
	  {
				 if(len==0)    //����ǵ�һ�ν���
				 {
							for(t=0;t<1;t++)
							{
								 P_last[t][t] = 1/c;  //100
								 P_last[t][t+1] = 0;  
								 P_last[t+1][t] = 1/c;  //100
								 P_last[t+1][t+1] = 0;  
							}
				 }
				 d =  in_data[len];    //�����µ������ź�  ԭʼ��ֵ�˲�����źŸ���d
				 Interference[0][0] = sin(2*PI*peak_50*(len)/fs);  //fs�ǲ�����
				 Interference[1][0] = cos(2*PI*peak_50*(len)/fs);  //��������һ�еľ���
				 
				 Interference1[0][0]   = Interference[0][0];
				 Interference1[0][1]   = Interference[1][0];
				 //�㷨����
				 for (k = 0; k <M_FIR; k++)     //P_last * Interference
				 {
							for (k2 = 0; k2 < 1; k2++)   
							{
									stmp = 0.0;
									for (k4 = 0; k4 < M_FIR; k4++)  
									{
										stmp += P_last[k][k4] * Interference[k4][k2];
									}
									M[k][k2] = stmp;
							}
				 }
				 
				 for (k = 0; k <1; k++)     //Interference'* P_last * Interferenc
				 {
							for (k2 = 0; k2 < M_FIR; k2++)   
							{
									stmp = 0.0;
									for (k4 = 0; k4 < M_FIR; k4++)  
									{
										stmp += Interference1[k][k4] * P_last[k4][k2];
									}
									N[k][k2] = stmp;
							}
				 }
				 
				 for (k = 0; k <1; k++)     //lamda + Interference'* P_last * Interference
				 {
							for (k2 = 0; k2 < 1; k2++)   
							{
									stmp = 0.0;
									for (k4 = 0; k4 < M_FIR; k4++)  
									{
										stmp += N[k][k4] * Interference[k4][k2];
									}
									res = stmp +lamda;
							}
				 }
				 K[0][0] =  (M[0][0])/res;  //K = (P_last * Interference) / (lamda + Interference'* P_last * Interference)
				 K[1][0] =  (M[1][0])/res;
				 
				 for (k = 0; k <1; k++)     //y = Interference' * w_last  ����FIR�˲������
				 {
							for (k2 = 0; k2 < 1; k2++)   
							{
									stmp = 0.0;
									for (k4 = 0; k4 < M_FIR; k4++)  
									{
										stmp += Interference1[k][k4] * w_last[k4][k2];
									}
									y= stmp ;
							}
				 }
				 Eta = d - y;  // ������Ƶ����
				 
				 K1[0][0] = K[0][0] * Eta;  //�����˲���ϵ��ʸ��// K * Eta
				 K1[1][0] = K[1][0] * Eta;
				 
				 W[0][0] = w_last[0][0] +  K1[0][0]; //  w = w_last + K * Eta;
				 W[1][0] = w_last[1][0] +  K1[1][0];
				 
				 for (k = 0; k < M_FIR; k++)     //���������ؾ���// K * Interference'
				 {
							for (k2 = 0; k2 <  M_FIR; k2++)   
							{
									stmp = 0.0;
									for (k4 = 0; k4 < 1; k4++)  
									{
										stmp += N[k][k4] * Interference[k4][k2];
									}
									P1[k][k2] = stmp;
							}
				 }
				 
				 for (k = 0; k < M_FIR; k++)  //I - K * Interference'
				 {
							for(k2 = 0; k2 < M_FIR; k2++)
							{
								P2[k][k2] =I[k][k2] - P1[k][k2]; 
							}
				 }
				 
				 for (k = 0; k < M_FIR; k++)     //(I - K * Interference') * P_last
				 {
							for (k2 = 0; k2 <  M_FIR; k2++)   
							{
									stmp = 0.0;
									for (k4 = 0; k4 < 2; k4++)  
									{
										stmp += P2[k][k4] * P_last[k4][k2];
									}
									P3[k][k2] = stmp;
							}
				 }
				 
				 for (k = 0; k < M_FIR; k++)     // P = (I - K * Interference') * P_last/lamda
				 {
							for (k2 = 0; k2 <  M_FIR; k2++)   
							{
									stmp = 0.0;
									for (k4 = 0; k4 < 2; k4++)  
									{
										stmp += P3[k][k4] / lamda;
									}
									P[k][k2] = stmp;
							}
				 }
				 
				 for (k = 0; k < M_FIR; k++)  // P_last = P
				 {
							for(k2 = 0; k2 < M_FIR; k2++)
							{
								P_last[k][k2] = P[k][k2]; 
							}
				 }
					
				 for (k = 0; k < M_FIR; k++)  //  w_last = w
				 {
							for(k2 = 0; k2 < 1; k2++)
							{
								w_last[k][k2] = W[k][k2]; 
							}
				 }
				 
				 /*�˲�����洢*/
				 Eta_out[len] = Eta;
		}
		
		for(len=0;len<2048;len++)
		{
				 out_data[len] = Eta_out[len]; //�������ź�
		}
	   
}

//	a_matrix=b_matrix*c_matrix
//	krow  :����
//	kline :����
//	ktrl  :	����0:��������������� ������0:����������Ը�������
//void matrix_m(float **a_matrix,  float **b_matrix,  float **c_matrix,
//				int krow, int kline, int kmiddle)
//{
//	u16 k, k2, k4;
//	float stmp;
// 
//	for (k = 0; k < krow; k++)     
//	{
//			for (k2 = 0; k2 < kline; k2++)   
//			{
//					stmp = 0.0;
//					for (k4 = 0; k4 < kmiddle; k4++)  
//					{
//						stmp += b_matrix[k][k4] * c_matrix[k4][k2];
//					}
//					a_matrix[k][k2] = stmp;
//			}
//	}
//	
//}


