#include "StdAfx.h"
#include "OptUtil.h"
#include "MainFrm.h"

int COptUtil::XYZWPRtoMAT(double(*result)[4], double pos[])
{
	int i, j, k;
	double temp_1[4][4], temp_2[4][4], temp_3[4][4], temp_4[4][4];
	double temp_6[4][4] = { 0 };
	double temp_7[4][4] = { 0 };
	double temp_8[4][4] = { 0 };

	temp_1[0][0] = 1, temp_1[0][1] = 0, temp_1[0][2] = 0, temp_1[0][3] = pos[0];
	temp_1[1][0] = 0, temp_1[1][1] = 1, temp_1[1][2] = 0, temp_1[1][3] = pos[1];
	temp_1[2][0] = 0, temp_1[2][1] = 0, temp_1[2][2] = 1, temp_1[2][3] = pos[2];
	temp_1[3][0] = 0, temp_1[3][1] = 0, temp_1[3][2] = 0, temp_1[3][3] = 1;

	temp_2[0][0] = cos(pos[5] * de_pi), temp_2[0][1] = -sin(pos[5] * de_pi), temp_2[0][2] = 0, temp_2[0][3] = 0;
	temp_2[1][0] = sin(pos[5] * de_pi), temp_2[1][1] = cos(pos[5] * de_pi), temp_2[1][2] = 0, temp_2[1][3] = 0;
	temp_2[2][0] = 0, temp_2[2][1] = 0, temp_2[2][2] = 1, temp_2[2][3] = 0;
	temp_2[3][0] = 0, temp_2[3][1] = 0, temp_2[3][2] = 0, temp_2[3][3] = 1;

	temp_3[0][0] = cos(pos[4] * de_pi), temp_3[0][1] = 0, temp_3[0][2] = sin(pos[4] * de_pi), temp_3[0][3] = 0;
	temp_3[1][0] = 0, temp_3[1][1] = 1, temp_3[1][2] = 0, temp_3[1][3] = 0;
	temp_3[2][0] = -sin(pos[4] * de_pi), temp_3[2][1] = 0, temp_3[2][2] = cos(pos[4] * de_pi), temp_3[2][3] = 0;
	temp_3[3][0] = 0, temp_3[3][1] = 0, temp_3[3][2] = 0, temp_3[3][3] = 1;

	temp_4[0][0] = 1, temp_4[0][1] = 0, temp_4[0][2] = 0, temp_4[0][3] = 0;
	temp_4[1][0] = 0, temp_4[1][1] = cos(pos[3] * de_pi), temp_4[1][2] = -sin(pos[3] * de_pi), temp_4[1][3] = 0;
	temp_4[2][0] = 0, temp_4[2][1] = sin(pos[3] * de_pi), temp_4[2][2] = cos(pos[3] * de_pi), temp_4[2][3] = 0;
	temp_4[3][0] = 0, temp_4[3][1] = 0, temp_4[3][2] = 0, temp_4[3][3] = 1;

	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
		{
			for (k = 0; k<4; k++)
			{
				temp_6[i][j] = temp_6[i][j] + temp_1[i][k] * temp_2[k][j];
			}
		}
	}
	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
		{
			for (k = 0; k<4; k++)
			{
				temp_7[i][j] = temp_7[i][j] + temp_6[i][k] * temp_3[k][j];
			}
		}
	}
	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
		{
			for (k = 0; k<4; k++)
			{
				temp_8[i][j] = temp_8[i][j] + temp_7[i][k] * temp_4[k][j];
			}
		}
	}

	for (i = 0; i<4; i++)
		for (j = 0; j<4; j++)
			result[i][j] = temp_8[i][j];

	return 0;
}

int COptUtil::MultiplyMAT(double result[][4], double mat1[][4], double mat2[][4])
{
	int i, j, k;
	double temp[4][4] = { 0 };

	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
		{
			for (k = 0; k<4; k++)
			{
				temp[i][j] = temp[i][j] + mat1[i][k] * mat2[k][j];
			}
		}
	}

	for (i = 0; i<4; i++)
		for (j = 0; j<4; j++)
			result[i][j] = temp[i][j];

	return 0;
}

int COptUtil::MultiplyMAT2(double result[][4], double mat[][4][4], int matsize)
{
	int i, j, k, cnt;
	double temp[4][4] = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };
	double temp2[4][4] = { 0 };

	for (cnt = 0; cnt<matsize; cnt++)
	{
		for (i = 0; i<4; i++)
			for (j = 0; j<4; j++)
				temp2[i][j] = 0;

		for (i = 0; i<4; i++)
		{
			for (j = 0; j<4; j++)
			{
				for (k = 0; k<4; k++)
				{
					temp2[i][j] = temp2[i][j] + temp[i][k] * mat[cnt][k][j];
				}
			}
		}

		for (i = 0; i<4; i++)
			for (j = 0; j<4; j++)
				temp[i][j] = temp2[i][j];
	}

	for (i = 0; i<4; i++)
		for (j = 0; j<4; j++)
			result[i][j] = temp[i][j];

	return 0;
}

int COptUtil::InverseMAT(double result[][4], double mat[][4])
{
	int i, j;
	double temp[4][4] = { 0 };

	for (i = 0; i<3; i++)
		for (j = 0; j<3; j++)
			temp[i][j] = mat[j][i];

	for (i = 0; i<3; i++)
		temp[i][3] = -(mat[0][i] * mat[0][3] + mat[1][i] * mat[1][3] + mat[2][i] * mat[2][3]);

	temp[3][0] = temp[3][1] = temp[3][2] = 0;
	temp[3][3] = 1;

	for (i = 0; i<4; i++)
		for (j = 0; j<4; j++)
			result[i][j] = temp[i][j];

	return 0;
}

int COptUtil::BaseAuxFK(double CurRobPosMatFromBaseAux[][4], double BaseAuxParaMat[][4][4], double CurBaseAuxTheta[], int BaseAuxNum, int BaseAuxType[], double(*BaseAuxJointDV)[4][4])
{
	int i;
	double BaseAuxParaMatJ[MAXBASEAUXNUM][4][4] = { 0 };

	for (i = 0; i<BaseAuxNum; i++)
	{
		double JointVal[6] = { 0 };
		double JointValMat[4][4] = { 0 };
		double temp_mat[4][4] = { 0 };
		switch (BaseAuxType[i])
		{
		case 1:
			JointVal[0] = CurBaseAuxTheta[i];
			break;
		case 2:
			JointVal[1] = CurBaseAuxTheta[i];
			break;
		case 3:
			JointVal[2] = CurBaseAuxTheta[i];
			break;
		case 4:
			JointVal[3] = CurBaseAuxTheta[i];
			break;
		case 5:
			JointVal[4] = CurBaseAuxTheta[i];
			break;
		case 6:
			JointVal[5] = CurBaseAuxTheta[i];
			break;
		}
		XYZWPRtoMAT(JointValMat, JointVal);
		MultiplyMAT(temp_mat, BaseAuxParaMat[i], BaseAuxJointDV[i]);
		MultiplyMAT(BaseAuxParaMatJ[i], temp_mat, JointValMat);
	}
	MultiplyMAT2(CurRobPosMatFromBaseAux, BaseAuxParaMatJ, BaseAuxNum);

	//	test ajou
	//Identity_Matrix(CurRobPosMatFromBaseAux);
	return 0;
}

int COptUtil::RobotFK(double CurrentTcpPos[][4],
	double CurRobJointTheta[], double aa[], double dd[], double alpha,
	int RobJointSign[], double RobJointOffset[],
	double RobJointDV[][4][4], double MountMat[][4], double TcpMat[][4], double linkT[][4][4])
{
	int i, j, k, l;
	double temp_1[4][4] = { 0 };
	double matN[6][4][4] = { 0 };		//link parameters 이용한 matrices - delta value 적용 전
	double matD[6][4][4] = { 0 };		//link parameters 이용한 matrices - delta value 적용 후
	double mat[6][4][4] = { 0 };		//link parameters 이용한 matrices - theta value 적용 후
	double mat06[4][4] = { 0 };
	double current_joint_theta[6];
	for (i = 0; i<6; i++)
		current_joint_theta[i] = RobJointSign[i] * (CurRobJointTheta[i] + RobJointOffset[i]);

	//입력받은 링크 별 매트릭스 계산
	double pos[6][6] = { { 0, 0, dd[0], 0, 0, 0 },
	{ aa[1], dd[1], 0, -90, -90, 0 },
	{ aa[2], 0, 0, 0, 0, 0 },
	{ aa[3], dd[3], 0, -90, 0, 0 },
	{ aa[4], dd[4], 0, alpha, 0, 0 },
	{ 0, 0, 0, -alpha, 0, 0 } };
	for (i = 0; i<6; i++)
	{
		XYZWPRtoMAT(matN[i], pos[i]);
	}

	for (i = 0; i<6; i++)
		for (j = 0; j<4; j++)
			for (k = 0; k<4; k++)
				for (l = 0; l<4; l++)
					matD[i][j][k] = matD[i][j][k] + matN[i][j][l] * RobJointDV[i][l][k];

	for (i = 0; i<6; i++)
	{
		temp_1[0][0] = cos(current_joint_theta[i] * de_pi), temp_1[0][1] = -sin(current_joint_theta[i] * de_pi), temp_1[0][2] = 0, temp_1[0][3] = 0;
		temp_1[1][0] = sin(current_joint_theta[i] * de_pi), temp_1[1][1] = cos(current_joint_theta[i] * de_pi), temp_1[1][2] = 0, temp_1[1][3] = 0;
		temp_1[2][0] = 0, temp_1[2][1] = 0, temp_1[2][2] = 1, temp_1[2][3] = 0;
		temp_1[3][0] = 0, temp_1[3][1] = 0, temp_1[3][2] = 0, temp_1[3][3] = 1;

		MultiplyMAT(mat[i], matD[i], temp_1);

		Copy_Matrix(mat[i], linkT[i]);
	}
	/*
	for(i=0; i<4; i++)
	{
	for(j=0; j<4; j++)
	{
	cout << mat[0][i][j] << "\t";
	}
	cout << endl;
	}
	cout << endl;
	*/

	//T06 계산 T01 * T12* T23 * T34 * T45 * T56
	MultiplyMAT2(mat06, mat, 6);
	/*
	for(i=0; i<4; i++)
	{
	for(j=0; j<4; j++)
	{
	cout << mat06[i][j] << "\t";
	}
	cout << endl;
	}
	cout << endl;
	*/
	//mount 까지
	MultiplyMAT(CurrentTcpPos, mat06, MountMat);

	//Tcp 까지
	MultiplyMAT(CurrentTcpPos, CurrentTcpPos, TcpMat);

	return 0;
}

int COptUtil::RobotIK(double theta[],		//theta[6]
	double TargetPos0T6[][4],		//0T6 값
	double aa[], double dd[],		//Link 값
	double alpha,					//로봇 5축 offset
	double RobJointDV[][4][4],		//로봇 Delta Vector Calibration 값
	int TargetConfig[],				//Config 값
	int RobJointSign[],				//조인트 회전 방향 값
	double RobJointOffset[])		//조인트 Offset 값
{
	/*
	return 0 : find Theta
	return 1 : unreachable
	return 2 : singular
	*/
	int i;
	int iteration = 0;
	int FindThetaFlag = 0;

	//config value
	int shoulder, elbow, wrist;


	//각 Joint의 delta vector matrices
	double A[6], B[6], D[6], E[6];
	double F[6], G[6], H[6], I[6];
	double J[6], K[6], L[6], M[6];

	double nx, ny, nz, ox, oy, oz, ax, ay, az, px, py, pz;//입력 받을 6축 좌표
	double CosTheta[6] = { 0 };//cos theta값
	double SinTheta[6] = { 0 };//sin theta값
	double thetaPrev[6] = { 0 };
	double Tolerance[6] = { 0 };

	double Init_a5, Init_d5;

	double AA1, AA2, AA3, AA4, AB1, AB2, AD1, AD2;
	double AE1, AE2, AE3, AE4, AF1, AF2, AG1, AG2;
	double AH1, AH2, AH3, AH4, AI1, AI2, AJ1, AJ2;
	double BA1, BA2, BA3, BA4, DA1, DA2, DA3, DA4;
	double DB1, DB2, DB3, DD1, DD2, DD3, DE1, DE2, DE3, DE4;
	double DF1, DF2, DF3, DG1, DG2, DG3, DH1, DH2, DH3, DH4;
	double DI1, DI2, DI3, DJ1, DJ2, DJ3;
	double EA1, EA2, EA3, EA4, EA5, EA6, EA7, EA8;
	double FA1, FA2, FA3, FA4, FA5, FA6;
	double FB1, FB2, FB3, FB4, FB5, FB6, FB7, FB8, FB9;
	double FD1, FD2, FD3, FD4, FD5, FD6, FD7, FD8;
	double FE1, FE2, FE3, FE4;
	double FH1, FH2, FH3, FH4, FH5, FH6;
	double FI1, FI2, FI3, FI4, FI5, FI6, FI7, FI8, FI9;
	double FJ1, FJ2, FJ3, FJ4, FJ5, FJ6;
	double FK1, FK2, FK3, FK4, FK5, FK6, FK7, FK8, FK9;
	double GA1, GA2, GA3, GA4, GA5, GA6, GA7, GA8, GB1, GB2, GB3, GB4;
	double GD1, GD2, GD3, GD4, GD5, GD6, GE1, GE2, GE3, GE4, GE5, GE6, GE7, GE8;
	double GF1, GF2, GF3, GF4;
	double GG1, GG2, GG3, GG4, GG5, GG6, GH1, GH2, GH3, GH4, GH5, GH6, GH7, GH8;
	double GI1, GI2, GI3, GI4, GJ1, GJ2, GJ3, GJ4, GJ5, GJ6;
	double HA1, HA2, HA3, HA4, HA5, HA6;
	double IA1, IA2, IA3, IA4, IA5, IA6, IB1, IB2, IB3, IB4, IB5, IB6, ID1, ID2;

	double nx03, ny03, nz03, ox03, oy03, oz03, ax03, ay03, az03, pz26;
	double KA1, KA2, KA3, KA4, KA5;
	double LA1, LA2, LA3, LB1, LB2, LB3;
	double JA1, JA2, JA3, JA4, JA5;

	double PP, QQ, RR, PQR;

	//각 조인트의 delta value
	for (i = 0; i<6; i++)
	{
		A[i] = RobJointDV[i][0][0];
		B[i] = RobJointDV[i][0][1];
		D[i] = RobJointDV[i][0][2];
		E[i] = RobJointDV[i][0][3];

		F[i] = RobJointDV[i][1][0];
		G[i] = RobJointDV[i][1][1];
		H[i] = RobJointDV[i][1][2];
		I[i] = RobJointDV[i][1][3];

		J[i] = RobJointDV[i][2][0];
		K[i] = RobJointDV[i][2][1];
		L[i] = RobJointDV[i][2][2];
		M[i] = RobJointDV[i][2][3];
	}

	BA1 = -F[4] * cos(alpha*de_pi) + J[4] * sin(alpha*de_pi);	BA2 = -G[4] * cos(alpha*de_pi) + K[4] * sin(alpha*de_pi);
	BA3 = -J[4] * cos(alpha*de_pi) - F[4] * sin(alpha*de_pi);	BA4 = -K[4] * cos(alpha*de_pi) - G[4] * sin(alpha*de_pi);

	EA1 = F[5] * cos(alpha*de_pi) + J[5] * sin(alpha*de_pi);	EA2 = G[5] * cos(alpha*de_pi) + K[5] * sin(alpha*de_pi);
	EA3 = J[5] * cos(alpha*de_pi) - F[5] * sin(alpha*de_pi);	EA4 = K[5] * cos(alpha*de_pi) - G[5] * sin(alpha*de_pi);

	EA5 = H[5] * cos(alpha*de_pi) + L[5] * sin(alpha*de_pi);	EA6 = I[5] * cos(alpha*de_pi) + M[5] * sin(alpha*de_pi);
	EA7 = L[5] * cos(alpha*de_pi) - H[5] * sin(alpha*de_pi);	EA8 = M[5] * cos(alpha*de_pi) - I[5] * sin(alpha*de_pi);

	AA1 = A[0] * F[1] + B[0] * J[1];	AA2 = A[0] * G[1] + B[0] * K[1];	AA3 = B[0] * F[1] - A[0] * J[1];	AA4 = B[0] * G[1] - A[0] * K[1];
	AB1 = A[0] * H[1] + B[0] * L[1];	AB2 = B[0] * H[1] - A[0] * L[1];

	AD1 = B[0] * (M[1] - dd[1]) + A[0] * (I[1] + aa[1]);	AD2 = B[0] * (I[1] + aa[1]) - A[0] * (M[1] - dd[1]);

	AE1 = F[0] * F[1] + G[0] * J[1];	AE2 = F[0] * G[1] + G[0] * K[1];	AE3 = G[0] * F[1] - F[0] * J[1];	AE4 = G[0] * G[1] - F[0] * K[1];
	AF1 = F[0] * H[1] + G[0] * L[1];	AF2 = G[0] * H[1] - F[0] * L[1];

	AG1 = G[0] * (M[1] - dd[1]) + F[0] * (I[1] + aa[1]);	AG2 = G[0] * (I[1] + aa[1]) - F[0] * (M[1] - dd[1]);

	AH1 = J[0] * F[1] + K[0] * J[1];	AH2 = K[0] * F[1] - J[0] * J[1];	AH3 = J[0] * G[1] + K[0] * K[1];	AH4 = K[0] * G[1] - J[0] * K[1];
	AI1 = J[0] * H[1] + K[0] * L[1];	AI2 = K[0] * H[1] - J[0] * L[1];

	AJ1 = K[0] * (M[1] - dd[1]) + J[0] * (I[1] + aa[1]);	AJ2 = K[0] * (I[1] + aa[1]) - J[0] * (M[1] - dd[1]);

	IA1 = AA1*AD1 + AE1*AG1 + AH1*AJ1;	IA2 = AA2*AD1 + AE2*AG1 + AH3*AJ1;	IA3 = AA3*AD2 + AE3*AG2 + AH2*AJ2;	IA4 = AA4*AD2 + AE4*AG2 + AH4*AJ2;
	IA5 = AA1*AD2 + AE1*AG2 + AH1*AJ2 + AA3*AD1 + AE3*AG1 + AH2*AJ1;	IA6 = AA2*AD2 + AE2*AG2 + AH3*AJ2 + AA4*AD1 + AE4*AG1 + AH4*AJ1;

	IB1 = D[0] * A[1] * AD1 + AA1*(E[0] + D[0] * E[1]) + H[0] * A[1] * AG1 + AE1*(I[0] + H[0] * E[1]) + L[0] * A[1] * AJ1 + AH1*(M[0] + dd[0] + L[0] * E[1]);
	IB2 = D[0] * A[1] * AD2 + AA3*(E[0] + D[0] * E[1]) + H[0] * A[1] * AG2 + AE3*(I[0] + H[0] * E[1]) + L[0] * A[1] * AJ2 + AH2*(M[0] + dd[0] + L[0] * E[1]);
	IB3 = D[0] * B[1] * AD1 + AA2*(E[0] + D[0] * E[1]) + H[0] * B[1] * AG1 + AE2*(I[0] + H[0] * E[1]) + L[0] * B[1] * AJ1 + AH3*(M[0] + dd[0] + L[0] * E[1]);
	IB4 = D[0] * B[1] * AD2 + AA4*(E[0] + D[0] * E[1]) + H[0] * B[1] * AG2 + AE4*(I[0] + H[0] * E[1]) + L[0] * B[1] * AJ2 + AH4*(M[0] + dd[0] + L[0] * E[1]);
	IB5 = D[0] * A[1] * (E[0] + D[0] * E[1]) + H[0] * A[1] * (I[0] + H[0] * E[1]) + L[0] * A[1] * (M[0] + dd[0] + L[0] * E[1]);
	IB6 = D[0] * B[1] * (E[0] + D[0] * E[1]) + H[0] * B[1] * (I[0] + H[0] * E[1]) + L[0] * B[1] * (M[0] + dd[0] + L[0] * E[1]);

	HA1 = AB1*AD1 + AF1*AG1 + AI1*AJ1;	HA2 = AB2*AD2 + AF2*AG2 + AI2*AJ2;
	HA3 = AB1*AD2 + AB2*AD1 + AF1*AG2 + AF2*AG1 + AI1*AJ2 + AI2*AJ1;
	HA4 = AB1*(E[0] + D[0] * E[1]) + D[0] * D[1] * AD1 + AF1*(I[0] + H[0] * E[1]) + H[0] * D[1] * AG1 + AI1*(M[0] + dd[0] + L[0] * E[1]) + L[0] * D[1] * AJ1;
	HA5 = AB2*(E[0] + D[0] * E[1]) + D[0] * D[1] * AD2 + AF2*(I[0] + H[0] * E[1]) + H[0] * D[1] * AG2 + AI2*(M[0] + dd[0] + L[0] * E[1]) + L[0] * D[1] * AJ2;
	HA6 = D[0] * D[1] * (E[0] + D[0] * E[1]) + H[0] * D[1] * (I[0] + H[0] * E[1]) + L[0] * D[1] * (M[0] + dd[0] + L[0] * E[1]);

	DA1 = A[3] * A[4] - B[3] * BA1;	DA2 = A[3] * B[4] - B[3] * BA2;	DA3 = B[3] * A[4] + A[3] * BA1;	DA4 = B[3] * B[4] + A[3] * BA2;

	DB1 = A[3] * D[4] + B[3] * (H[4] * cos(alpha*de_pi) - L[4] * sin(alpha*de_pi));
	DB2 = B[3] * D[4] - A[3] * (H[4] * cos(alpha*de_pi) - L[4] * sin(alpha*de_pi));
	DB3 = -D[3] * (L[4] * cos(alpha*de_pi) + H[4] * sin(alpha*de_pi));

	DD1 = A[3] * (E[4] + aa[4]) + B[3] * (I[4] * cos(alpha*de_pi) - M[4] * sin(alpha*de_pi) - dd[4]);
	DD2 = B[3] * (E[4] + aa[4]) - A[3] * (I[4] * cos(alpha*de_pi) - M[4] * sin(alpha*de_pi) - dd[4]);
	DD3 = D[3] * (M[4] * cos(alpha*de_pi) + I[4] * sin(alpha*de_pi)) + (E[3] + aa[3]);

	DE1 = J[3] * A[4] - K[3] * BA1;	DE2 = J[3] * B[4] - K[3] * BA2;	DE3 = K[3] * A[4] + J[3] * BA1;	DE4 = K[3] * B[4] + J[3] * BA2;

	DF1 = J[3] * D[4] + K[3] * (H[4] * cos(alpha*de_pi) - L[4] * sin(alpha*de_pi));
	DF2 = K[3] * D[4] - J[3] * (H[4] * cos(alpha*de_pi) - L[4] * sin(alpha*de_pi));
	DF3 = -L[3] * (L[4] * cos(alpha*de_pi) + H[4] * sin(alpha*de_pi));

	DG1 = J[3] * (E[4] + aa[4]) + K[3] * (I[4] * cos(alpha*de_pi) - M[4] * sin(alpha*de_pi) - dd[4]);
	DG2 = K[3] * (E[4] + aa[4]) - J[3] * (I[4] * cos(alpha*de_pi) - M[4] * sin(alpha*de_pi) - dd[4]);
	DG3 = L[3] * (M[4] * cos(alpha*de_pi) + I[4] * sin(alpha*de_pi)) + (M[3] + dd[3]);

	DH1 = G[3] * BA1 - F[3] * A[4];	DH2 = G[3] * BA2 - F[3] * B[4];	DH3 = F[3] * BA1 + G[3] * A[4];	DH4 = F[3] * BA2 + G[3] * B[4];

	DI1 = -F[3] * D[4] - G[3] * (H[4] * cos(alpha*de_pi) - L[4] * sin(alpha*de_pi));
	DI2 = -G[3] * D[4] + F[3] * (H[4] * cos(alpha*de_pi) - L[4] * sin(alpha*de_pi));
	DI3 = -H[3] * (L[4] * cos(alpha*de_pi) + H[4] * sin(alpha*de_pi));

	DJ1 = -F[3] * (E[4] + aa[4]) - G[3] * (I[4] * cos(alpha*de_pi) - M[4] * sin(alpha*de_pi) - dd[4]);
	DJ2 = G[3] * (E[4] + aa[4]) - F[3] * (I[4] * cos(alpha*de_pi) - M[4] * sin(alpha*de_pi) - dd[4]);
	DJ3 = -H[3] * (M[4] * cos(alpha*de_pi) + I[4] * sin(alpha*de_pi)) - I[3];

	FA1 = D[5] * DA1 + EA5*DA2;	FA2 = D[5] * DA2 - EA5*DA1;	FA3 = D[5] * DA3 + EA5*DA4;	FA4 = D[5] * DA4 - EA5*DA3;
	FA5 = D[3] * D[5] * BA3 + EA5*D[3] * BA4;	FA6 = -D[3] * D[5] * BA4 + EA5*D[3] * BA3;

	FB1 = E[5] * DA1 + EA6*DA2;	FB2 = E[5] * DA2 - EA6*DA1;	FB3 = E[5] * DA3 + EA6*DA4;	FB4 = E[5] * DA4 - EA6*DA3;
	FB5 = D[3] * E[5] * BA3 + EA6*D[3] * BA4;	FB6 = -D[3] * E[5] * BA4 + EA6*D[3] * BA3;
	FB7 = EA8*DB1 + DD1;	FB8 = EA8*DB2 + DD2;	FB9 = -EA8*DB3 + DD3;

	FD1 = A[5] * DE1 + EA1*DE2;	FD2 = A[5] * DE2 - EA1*DE1;	FD3 = A[5] * DE3 + EA1*DE4;	FD4 = A[5] * DE4 - EA1*DE3;
	FD5 = B[5] * DE1 + EA2*DE2;	FD6 = B[5] * DE2 - EA2*DE1;	FD7 = B[5] * DE3 + EA2*DE4;	FD8 = B[5] * DE4 - EA2*DE3;

	FE1 = A[5] * L[3] * BA3 + L[3] * EA1*BA4;	FE2 = -A[5] * L[3] * BA4 + L[3] * EA1*BA3;
	FE3 = B[5] * L[3] * BA3 + L[3] * EA2*BA4;	FE4 = -B[5] * L[3] * BA4 + L[3] * EA2*BA3;

	FH1 = D[5] * DE1 + EA5*DE2;	FH2 = D[5] * DE2 - EA5*DE1;	FH3 = D[5] * DE3 + EA5*DE4;	FH4 = D[5] * DE4 - EA5*DE3;
	FH5 = L[3] * D[5] * BA3 + EA5*L[3] * BA4;	FH6 = -L[3] * D[5] * BA4 + EA5*L[3] * BA3;

	FI1 = E[5] * DE1 + EA6*DE2;	FI2 = E[5] * DE2 - EA6*DE1;	FI3 = E[5] * DE3 + EA6*DE4;	FI4 = E[5] * DE4 - EA6*DE3;
	FI5 = L[3] * E[5] * BA3 + EA6*L[3] * BA4;	FI6 = -L[3] * E[5] * BA4 + EA6*L[3] * BA3;
	FI7 = EA8*DF1 + DG1;	FI8 = EA8*DF2 + DG2;	FI9 = -EA8*DF3 + DG3;

	FJ1 = D[5] * DH1 + EA5*DH2;	FJ2 = D[5] * DH2 - EA5*DH1;	FJ3 = D[5] * DH3 + EA5*DH4;	FJ4 = -D[5] * DH4 + EA5*DH3;
	FJ5 = H[3] * D[5] * BA3 + EA5*H[3] * BA4;	FJ6 = H[3] * D[5] * BA4 - EA5*H[3] * BA3;

	FK1 = E[5] * DH1 + EA6*DH2;	FK2 = E[5] * DH2 - EA6*DH1;	FK3 = E[5] * DH3 + EA6*DH4;	FK4 = -E[5] * DH4 + EA6*DH3;
	FK5 = H[3] * E[5] * BA3 + EA6*H[3] * BA4;	FK6 = H[3] * E[5] * BA4 - EA6*H[3] * BA3;
	FK7 = EA8*DI1 + DJ1;	FK8 = EA8*DI2 - DJ2;	FK9 = EA8*DI3 + DJ3;

	GA1 = A[2] * AA1 + F[2] * AA2;	GA2 = A[2] * AA2 - F[2] * AA1;	GA3 = A[2] * AA3 + F[2] * AA4;	GA4 = A[2] * AA4 - F[2] * AA3;
	GA5 = B[2] * AA1 + G[2] * AA2;	GA6 = B[2] * AA2 - G[2] * AA1;	GA7 = B[2] * AA3 + G[2] * AA4;	GA8 = B[2] * AA4 - G[2] * AA3;

	GB1 = A[2] * D[0] * A[1] + F[2] * D[0] * B[1];	GB2 = A[2] * D[0] * B[1] - F[2] * D[0] * A[1];
	GB3 = B[2] * D[0] * A[1] + G[2] * D[0] * B[1];	GB4 = B[2] * D[0] * B[1] - G[2] * D[0] * A[1];

	GD1 = D[2] * AA1 + H[2] * AA2;	GD2 = D[2] * AA2 - H[2] * AA1;	GD3 = D[2] * AA3 + H[2] * AA4;	GD4 = D[2] * AA4 - H[2] * AA3;
	GD5 = D[2] * D[0] * A[1] + H[2] * D[0] * B[1];	GD6 = D[2] * D[0] * B[1] - H[2] * D[0] * A[1];

	GE1 = A[2] * AE1 + F[2] * AE2;	GE2 = A[2] * AE2 - F[2] * AE1;	GE3 = A[2] * AE3 + F[2] * AE4;	GE4 = A[2] * AE4 - F[2] * AE3;
	GE5 = B[2] * AE1 + G[2] * AE2;	GE6 = B[2] * AE2 - G[2] * AE1;	GE7 = B[2] * AE3 + G[2] * AE4;	GE8 = B[2] * AE4 - G[2] * AE3;

	GF1 = A[2] * H[0] * A[1] + F[2] * H[0] * B[1];	GF2 = A[2] * H[0] * B[1] - F[2] * H[0] * A[1];
	GF3 = B[2] * H[0] * A[1] + G[2] * H[0] * B[1];	GF4 = B[2] * H[0] * B[1] - G[2] * H[0] * A[1];

	GG1 = D[2] * AE1 + H[2] * AE2;	GG2 = D[2] * AE2 - H[2] * AE1;	GG3 = D[2] * AE3 + H[2] * AE4;	GG4 = D[2] * AE4 - H[2] * AE3;
	GG5 = D[2] * H[0] * A[1] + H[2] * H[0] * B[1];	GG6 = D[2] * H[0] * B[1] - H[2] * H[0] * A[1];

	GH1 = A[2] * AH1 + F[2] * AH3;	GH2 = A[2] * AH2 + F[2] * AH4;	GH3 = A[2] * AH3 - F[2] * AH1;	GH4 = A[2] * AH4 - F[2] * AH2;
	GH5 = B[2] * AH1 + G[2] * AH3;	GH6 = B[2] * AH2 + G[2] * AH4;	GH7 = B[2] * AH3 - G[2] * AH1;	GH8 = B[2] * AH4 - G[2] * AH2;

	GI1 = A[2] * L[0] * A[1] + F[2] * L[0] * B[1];	GI2 = A[2] * L[0] * B[1] - F[2] * L[0] * A[1];
	GI3 = B[2] * L[0] * A[1] + G[2] * L[0] * B[1];	GI4 = B[2] * L[0] * B[1] - G[2] * L[0] * A[1];

	GJ1 = D[2] * AH1 + H[2] * AH3;	GJ2 = D[2] * AH2 + H[2] * AH4;	GJ3 = D[2] * AH3 - H[2] * AH1;	GJ4 = D[2] * AH4 - H[2] * AH2;
	GJ5 = D[2] * L[0] * A[1] + H[2] * L[0] * B[1];	GJ6 = D[2] * L[0] * B[1] - H[2] * L[0] * A[1];

	//입력 받은 TCP 좌표
	nx = TargetPos0T6[0][0];
	ny = TargetPos0T6[1][0];
	nz = TargetPos0T6[2][0];
	ox = TargetPos0T6[0][1];
	oy = TargetPos0T6[1][1];
	oz = TargetPos0T6[2][1];
	ax = TargetPos0T6[0][2];
	ay = TargetPos0T6[1][2];
	az = TargetPos0T6[2][2];
	px = TargetPos0T6[0][3];
	py = TargetPos0T6[1][3];
	pz = TargetPos0T6[2][3];

	//config 값 설정
	shoulder = TargetConfig[0];
	elbow = TargetConfig[1];
	wrist = TargetConfig[2];

	//현재 구하 theta 값이 이전 theta 값과 비교해 차이가 없을 때가지 반복 계산 
	while (!FindThetaFlag)
	{
		iteration++;

		if (iteration == 1)
		{
			//find initial theta 1
			PP = py;
			QQ = -px;
			Init_a5 = Init_d5 = 0;
			RR = dd[1] - Init_d5*CosTheta[3] + Init_a5*SinTheta[3];
			PQR = PP*PP + QQ*QQ - RR*RR;

			if (PQR < 0)//unreachable
				return 1;

			CosTheta[0] = (-PP*RR - shoulder*QQ*sqrt(PQR)) / (PP*PP + QQ*QQ);
			SinTheta[0] = (-QQ*RR + shoulder*PP*sqrt(PQR)) / (PP*PP + QQ*QQ);
			theta[0] = atan2(SinTheta[0], CosTheta[0]);

			//find initial theta 3
			JA1 = pz - dd[0];
			JA2 = px*CosTheta[0] + py*SinTheta[0] - aa[1];
			JA3 = Init_a5*CosTheta[3] + Init_d5*SinTheta[3] + aa[3];

			PP = 2 * aa[2] * JA3;
			QQ = -2 * aa[2] * dd[3];
			RR = aa[2] * aa[2] + dd[3] * dd[3] - JA1*JA1 - JA2*JA2 + JA3*JA3;
			PQR = PP*PP + QQ*QQ - RR*RR;

			if (PQR < 0)//unreachable
				return 1;

			CosTheta[2] = (-PP*RR - elbow*QQ*sqrt(PQR)) / (PP*PP + QQ*QQ);
			SinTheta[2] = (-QQ*RR + elbow*PP*sqrt(PQR)) / (PP*PP + QQ*QQ);
			theta[2] = atan2(SinTheta[2], CosTheta[2]);

			//find initial theta 2
			JA4 = JA3*CosTheta[2] - dd[3] * SinTheta[2] + aa[2];
			JA5 = JA3*SinTheta[2] + dd[3] * CosTheta[2];

			CosTheta[1] = (JA1*JA4 + JA2*JA5) / (JA1*JA1 + JA2*JA2);
			SinTheta[1] = (JA2*JA4 - JA1*JA5) / (JA1*JA1 + JA2*JA2);
			theta[1] = atan2(SinTheta[1], CosTheta[1]);

			//--------------------------------------------------------------------------------------------------
			nx03
				= GA1*CosTheta[0] * CosTheta[1] * CosTheta[2] + GA2*CosTheta[0] * SinTheta[1] * CosTheta[2]
				+ GA3*SinTheta[0] * CosTheta[1] * CosTheta[2] + GA4*SinTheta[0] * SinTheta[1] * CosTheta[2]
				+ GA5*CosTheta[0] * CosTheta[1] * SinTheta[2] + GA6*CosTheta[0] * SinTheta[1] * SinTheta[2]
				+ GA7*SinTheta[0] * CosTheta[1] * SinTheta[2] + GA8*SinTheta[0] * SinTheta[1] * SinTheta[2]
				+ GB1*CosTheta[1] * CosTheta[2] + GB2*SinTheta[1] * CosTheta[2]
				+ GB3*CosTheta[1] * SinTheta[2] + GB4*SinTheta[1] * SinTheta[2]
				+ J[2] * AB1*CosTheta[0] * CosTheta[2] + J[2] * AB2*SinTheta[0] * CosTheta[2]
				+ K[2] * AB1*CosTheta[0] * SinTheta[2] + K[2] * AB2*SinTheta[0] * SinTheta[2]
				+ J[2] * D[0] * D[1] * CosTheta[2] + K[2] * D[0] * D[1] * SinTheta[2];
			ny03
				= GE1*CosTheta[0] * CosTheta[1] * CosTheta[2] + GE2*CosTheta[0] * SinTheta[1] * CosTheta[2]
				+ GE3*SinTheta[0] * CosTheta[1] * CosTheta[2] + GE4*SinTheta[0] * SinTheta[1] * CosTheta[2]
				+ GE5*CosTheta[0] * CosTheta[1] * SinTheta[2] + GE6*CosTheta[0] * SinTheta[1] * SinTheta[2]
				+ GE7*SinTheta[0] * CosTheta[1] * SinTheta[2] + GE8*SinTheta[0] * SinTheta[1] * SinTheta[2]
				+ GF1*CosTheta[1] * CosTheta[2] + GF2*SinTheta[1] * CosTheta[2]
				+ GF3*CosTheta[1] * SinTheta[2] + GF4*SinTheta[1] * SinTheta[2]
				+ J[2] * AF1*CosTheta[0] * CosTheta[2] + J[2] * AF2*SinTheta[0] * CosTheta[2]
				+ K[2] * AF1*CosTheta[0] * SinTheta[2] + K[2] * AF2*SinTheta[0] * SinTheta[2]
				+ J[2] * H[0] * D[1] * CosTheta[2] + K[2] * H[0] * D[1] * SinTheta[2];
			nz03
				= GH1*CosTheta[0] * CosTheta[1] * CosTheta[2] + GH3*CosTheta[0] * SinTheta[1] * CosTheta[2]
				+ GH2*SinTheta[0] * CosTheta[1] * CosTheta[2] + GH4*SinTheta[0] * SinTheta[1] * CosTheta[2]
				+ GH5*CosTheta[0] * CosTheta[1] * SinTheta[2] + GH7*CosTheta[0] * SinTheta[1] * SinTheta[2]
				+ GH6*SinTheta[0] * CosTheta[1] * SinTheta[2] + GH8*SinTheta[0] * SinTheta[1] * SinTheta[2]
				+ GI1*CosTheta[1] * CosTheta[2] + GI2*SinTheta[1] * CosTheta[2]
				+ GI3*CosTheta[1] * SinTheta[2] + GI4*SinTheta[1] * SinTheta[2]
				+ J[2] * AI1*CosTheta[0] * CosTheta[2] + J[2] * AI2*SinTheta[0] * CosTheta[2]
				+ K[2] * AI1*CosTheta[0] * SinTheta[2] + K[2] * AI2*SinTheta[0] * SinTheta[2]
				+ J[2] * L[0] * D[1] * CosTheta[2] + K[2] * L[0] * D[1] * SinTheta[2];

			ox03
				= GA5*CosTheta[0] * CosTheta[1] * CosTheta[2] + GA6*CosTheta[0] * SinTheta[1] * CosTheta[2]
				+ GA7*SinTheta[0] * CosTheta[1] * CosTheta[2] + GA8*SinTheta[0] * SinTheta[1] * CosTheta[2]
				- GA1*CosTheta[0] * CosTheta[1] * SinTheta[2] - GA2*CosTheta[0] * SinTheta[1] * SinTheta[2]
				- GA3*SinTheta[0] * CosTheta[1] * SinTheta[2] - GA4*SinTheta[0] * SinTheta[1] * SinTheta[2]
				+ GB3*CosTheta[1] * CosTheta[2] + GB4*SinTheta[1] * CosTheta[2]
				- GB1*CosTheta[1] * SinTheta[2] - GB2*SinTheta[1] * SinTheta[2]
				+ K[2] * AB1*CosTheta[0] * CosTheta[2] + K[2] * AB2*SinTheta[0] * CosTheta[2]
				- J[2] * AB1*CosTheta[0] * SinTheta[2] - J[2] * AB2*SinTheta[0] * SinTheta[2]
				+ K[2] * D[0] * D[1] * CosTheta[2] - J[2] * D[0] * D[1] * SinTheta[2];
			oy03
				= GE5*CosTheta[0] * CosTheta[1] * CosTheta[2] + GE6*CosTheta[0] * SinTheta[1] * CosTheta[2]
				+ GE7*SinTheta[0] * CosTheta[1] * CosTheta[2] + GE8*SinTheta[0] * SinTheta[1] * CosTheta[2]
				- GE1*CosTheta[0] * CosTheta[1] * SinTheta[2] - GE2*CosTheta[0] * SinTheta[1] * SinTheta[2]
				- GE3*SinTheta[0] * CosTheta[1] * SinTheta[2] - GE4*SinTheta[0] * SinTheta[1] * SinTheta[2]
				+ GF3*CosTheta[1] * CosTheta[2] + GF4*SinTheta[1] * CosTheta[2]
				- GF1*CosTheta[1] * SinTheta[2] - GF2*SinTheta[1] * SinTheta[2]
				+ K[2] * AF1*CosTheta[0] * CosTheta[2] + K[2] * AF2*SinTheta[0] * CosTheta[2]
				- J[2] * AF1*CosTheta[0] * SinTheta[2] - J[2] * AF2*SinTheta[0] * SinTheta[2]
				+ K[2] * H[0] * D[1] * CosTheta[2] - J[2] * H[0] * D[1] * SinTheta[2];
			oz03
				= GH5*CosTheta[0] * CosTheta[1] * CosTheta[2] + GH7*CosTheta[0] * SinTheta[1] * CosTheta[2]
				+ GH6*SinTheta[0] * CosTheta[1] * CosTheta[2] + GH8*SinTheta[0] * SinTheta[1] * CosTheta[2]
				- GH1*CosTheta[0] * CosTheta[1] * SinTheta[2] - GH3*CosTheta[0] * SinTheta[1] * SinTheta[2]
				- GH2*SinTheta[0] * CosTheta[1] * SinTheta[2] - GH4*SinTheta[0] * SinTheta[1] * SinTheta[2]
				+ GI3*CosTheta[1] * CosTheta[2] + GI4*SinTheta[1] * CosTheta[2]
				- GI1*CosTheta[1] * SinTheta[2] - GI2*SinTheta[1] * SinTheta[2]
				+ K[2] * AI1*CosTheta[0] * CosTheta[2] + K[2] * AI2*SinTheta[0] * CosTheta[2]
				- J[2] * AI1*CosTheta[0] * SinTheta[2] - J[2] * AI2*SinTheta[0] * SinTheta[2]
				+ K[2] * L[0] * D[1] * CosTheta[2] - J[2] * L[0] * D[1] * SinTheta[2];

			ax03
				= GD1*CosTheta[0] * CosTheta[1] + GD2*CosTheta[0] * SinTheta[1]
				+ GD3*SinTheta[0] * CosTheta[1] + GD4*SinTheta[0] * SinTheta[1]
				+ GD5*CosTheta[1] + GD6*SinTheta[1]
				+ L[2] * AB1*CosTheta[0] + L[2] * AB2*SinTheta[0]
				+ L[2] * D[0] * D[1];
			ay03
				= GG1*CosTheta[0] * CosTheta[1] + GG2*CosTheta[0] * SinTheta[1]
				+ GG3*SinTheta[0] * CosTheta[1] + GG4*SinTheta[0] * SinTheta[1]
				+ GG5*CosTheta[1] + GG6*SinTheta[1]
				+ L[2] * AF1*CosTheta[0] + L[2] * AF2*SinTheta[0]
				+ L[2] * H[0] * D[1];
			az03
				= GJ1*CosTheta[0] * CosTheta[1] + GJ3*CosTheta[0] * SinTheta[1]
				+ GJ2*SinTheta[0] * CosTheta[1] + GJ4*SinTheta[0] * SinTheta[1]
				+ GJ5*CosTheta[1] + GJ6*SinTheta[1]
				+ L[2] * AI1*CosTheta[0] + L[2] * AI2*SinTheta[0]
				+ L[2] * L[0] * D[1];

			KA1 = nx03*ax + ny03*ay + nz03*az;
			KA2 = ox03*nx + oy03*ny + oz03*nz;
			KA3 = ox03*ox + oy03*oy + oz03*oz;
			KA4 = ox03*ax + oy03*ay + oz03*az;
			KA5 = ax03*ax + ay03*ay + az03*az;

			//----------------------------------------------------------------------------------------------------

			//find initial theta 5
			CosTheta[4] = (KA4 + pow(cos(alpha*de_pi), 2)) / (pow(sin(alpha*de_pi), 2));
			PQR = ((KA2*KA2 + KA3*KA3) - pow(sin(alpha*de_pi), 2)*pow(cos(alpha*de_pi), 2)*pow((CosTheta[4] + 1), 2)) / (pow(sin(alpha*de_pi), 2));
			if (PQR < 0)//unreachable
				PQR = 0;

			SinTheta[4] = wrist*sqrt(PQR);
			theta[4] = atan2(SinTheta[4], CosTheta[4]);

			JA1 = sin(alpha*de_pi)*SinTheta[4];
			JA2 = sin(alpha*de_pi)*cos(alpha*de_pi)*(CosTheta[4] + 1);
			PQR = JA1*JA1 + JA2*JA2;

			if (PQR < 10e-10)//singular
				return 2;

			//find initial theta 4
			CosTheta[3] = (KA5*JA2 - KA1*JA1) / PQR;
			SinTheta[3] = (KA1*JA2 + KA5*JA1) / PQR;
			theta[3] = atan2(SinTheta[3], CosTheta[3]);

			//find initial theta 6
			CosTheta[5] = (KA2*JA1 + KA3*JA2) / PQR;
			SinTheta[5] = (KA2*JA2 + KA3*JA1) / PQR;
			theta[5] = atan2(SinTheta[5], CosTheta[5]);
		}
		else
		{
			//find theta 1
			pz26
				= (J[2] * FB1 + K[2] * FI1)*CosTheta[2] * CosTheta[3] * CosTheta[4] + (J[2] * FB2 + K[2] * FI2)*CosTheta[2] * CosTheta[3] * SinTheta[4]
				+ (J[2] * FB3 + K[2] * FI3)*CosTheta[2] * SinTheta[3] * CosTheta[4] + (J[2] * FB4 + K[2] * FI4)*CosTheta[2] * SinTheta[3] * SinTheta[4]
				+ (K[2] * FB1 - J[2] * FI1)*SinTheta[2] * CosTheta[3] * CosTheta[4] + (K[2] * FB2 - J[2] * FI2)*SinTheta[2] * CosTheta[3] * SinTheta[4]
				+ (K[2] * FB3 - J[2] * FI3)*SinTheta[2] * SinTheta[3] * CosTheta[4] + (K[2] * FB4 - J[2] * FI4)*SinTheta[2] * SinTheta[3] * SinTheta[4]
				- (J[2] * FB5 + K[2] * FI5)*CosTheta[2] * CosTheta[4] + (J[2] * FB6 + K[2] * FI6)*CosTheta[2] * SinTheta[4]
				+ (J[2] * FB7 + K[2] * FI7)*CosTheta[2] * CosTheta[3] + (J[2] * FB8 + K[2] * FI8)*CosTheta[2] * SinTheta[3]
				+ (J[2] * FI5 - K[2] * FB5)*SinTheta[2] * CosTheta[4] + (K[2] * FB6 - J[2] * FI6)*SinTheta[2] * SinTheta[4]
				+ (K[2] * FB7 - J[2] * FI7)*SinTheta[2] * CosTheta[3] + (K[2] * FB8 - J[2] * FI8)*SinTheta[2] * SinTheta[3]
				+ L[2] * FK1*CosTheta[3] * CosTheta[4] + L[2] * FK2*CosTheta[3] * SinTheta[4]
				- L[2] * FK3*SinTheta[3] * CosTheta[4] + L[2] * FK4*SinTheta[3] * SinTheta[4]
				+ (J[2] * FB9 + K[2] * FI9)*CosTheta[2] + (K[2] * FB9 - J[2] * FI9)*SinTheta[2]
				+ L[2] * FK7*CosTheta[3] + L[2] * FK8*SinTheta[3]
				+ L[2] * FK5*CosTheta[4] + L[2] * FK6*SinTheta[4]
				+ L[2] * FK9 + M[2];

			PP = AB1*px + AF1*py + AI1*pz - HA4;
			QQ = AB2*px + AF2*py + AI2*pz - HA5;
			RR = D[0] * D[1] * px + H[0] * D[1] * py + L[0] * D[1] * pz - HA6 - HA1*CosTheta[0] * CosTheta[0] - HA2*SinTheta[0] * SinTheta[0] - HA3*SinTheta[0] * CosTheta[0] - pz26;
			PQR = PP*PP + QQ*QQ - RR*RR;

			if (PQR < 0)//unreachable
				return 1;

			CosTheta[0] = (-PP*RR - shoulder*QQ*sqrt(PQR)) / (PP*PP + QQ*QQ);
			SinTheta[0] = (-QQ*RR + shoulder*PP*sqrt(PQR)) / (PP*PP + QQ*QQ);
			theta[0] = atan2(SinTheta[0], CosTheta[0]);

			//find theta 3
			LA1
				= (A[2] * FB1 + B[2] * FI1)*CosTheta[3] * CosTheta[4] + (A[2] * FB2 + B[2] * FI2)*CosTheta[3] * SinTheta[4]
				+ (A[2] * FB3 + B[2] * FI3)*SinTheta[3] * CosTheta[4] + (A[2] * FB4 + B[2] * FI4)*SinTheta[3] * SinTheta[4]
				- (A[2] * FB5 + B[2] * FI5)*CosTheta[4] + (A[2] * FB6 + B[2] * FI6)*SinTheta[4]
				+ (A[2] * FB7 + B[2] * FI7)*CosTheta[3] + (A[2] * FB8 + B[2] * FI8)*SinTheta[3]
				+ (A[2] * FB9 + B[2] * FI9);
			LA2
				= (B[2] * FB1 - A[2] * FI1)*CosTheta[3] * CosTheta[4] + (B[2] * FB2 - A[2] * FI2)*CosTheta[3] * SinTheta[4]
				+ (B[2] * FB3 - A[2] * FI3)*SinTheta[3] * CosTheta[4] + (B[2] * FB4 - A[2] * FI4)*SinTheta[3] * SinTheta[4]
				+ (A[2] * FI5 - B[2] * FB5)*CosTheta[4] + (B[2] * FB6 - A[2] * FI6)*SinTheta[4]
				+ (B[2] * FB7 - A[2] * FI7)*CosTheta[3] + (B[2] * FB8 - A[2] * FI8)*SinTheta[3]
				+ (B[2] * FB9 - A[2] * FI9);
			LA3
				= D[2] * FK1*CosTheta[3] * CosTheta[4] + D[2] * FK2*CosTheta[3] * SinTheta[4]
				- D[2] * FK3*SinTheta[3] * CosTheta[4] + D[2] * FK4*SinTheta[3] * SinTheta[4]
				+ D[2] * FK7*CosTheta[3] + D[2] * FK8*SinTheta[3]
				+ D[2] * FK5*CosTheta[4] + D[2] * FK6*SinTheta[4]
				+ D[2] * FK9 + E[2] + aa[2];
			LB1
				= (F[2] * FB1 + G[2] * FI1)*CosTheta[3] * CosTheta[4] + (F[2] * FB2 + G[2] * FI2)*CosTheta[3] * SinTheta[4]
				+ (F[2] * FB3 + G[2] * FI3)*SinTheta[3] * CosTheta[4] + (F[2] * FB4 + G[2] * FI4)*SinTheta[3] * SinTheta[4]
				- (F[2] * FB5 + G[2] * FI5)*CosTheta[4] + (F[2] * FB6 + G[2] * FI6)*SinTheta[4]
				+ (F[2] * FB7 + G[2] * FI7)*CosTheta[3] + (F[2] * FB8 + G[2] * FI8)*SinTheta[3]
				+ (F[2] * FB9 + G[2] * FI9);
			LB2
				= (G[2] * FB1 - F[2] * FI1)*CosTheta[3] * CosTheta[4] + (G[2] * FB2 - F[2] * FI2)*CosTheta[3] * SinTheta[4]
				+ (G[2] * FB3 - F[2] * FI3)*SinTheta[3] * CosTheta[4] + (G[2] * FB4 - F[2] * FI4)*SinTheta[3] * SinTheta[4]
				+ (F[2] * FI5 - G[2] * FB5)*CosTheta[4] + (G[2] * FB6 - F[2] * FI6)*SinTheta[4]
				+ (G[2] * FB7 - F[2] * FI7)*CosTheta[3] + (G[2] * FB8 - F[2] * FI8)*SinTheta[3]
				+ (G[2] * FB9 - F[2] * FI9);
			LB3
				= H[2] * FK1*CosTheta[3] * CosTheta[4] + H[2] * FK2*CosTheta[3] * SinTheta[4]
				- H[2] * FK3*SinTheta[3] * CosTheta[4] + H[2] * FK4*SinTheta[3] * SinTheta[4]
				+ H[2] * FK7*CosTheta[3] + H[2] * FK8*SinTheta[3]
				+ H[2] * FK5*CosTheta[4] + H[2] * FK6*SinTheta[4]
				+ H[2] * FK9 + I[2];

			ID1 = IA1*pow(CosTheta[0], 2) + IA3*pow(SinTheta[0], 2) + IA5*SinTheta[0] * CosTheta[0] + IB1*CosTheta[0] + IB2*SinTheta[0] + IB5;
			ID2 = IA2*pow(CosTheta[0], 2) + IA4*pow(SinTheta[0], 2) + IA6*SinTheta[0] * CosTheta[0] + IB3*CosTheta[0] + IB4*SinTheta[0] + IB6;

			JA1 = (AA1*CosTheta[0] + AA3*SinTheta[0] + D[0] * A[1])*px
				+ (AE1*CosTheta[0] + AE3*SinTheta[0] + H[0] * A[1])*py
				+ (AH1*CosTheta[0] + AH2*SinTheta[0] + L[0] * A[1])*pz - ID1;
			JA2 = (AA2*CosTheta[0] + AA4*SinTheta[0] + D[0] * B[1])*px
				+ (AE2*CosTheta[0] + AE4*SinTheta[0] + H[0] * B[1])*py
				+ (AH3*CosTheta[0] + AH4*SinTheta[0] + L[0] * B[1])*pz - ID2;

			PP = 2 * (LA1*LA3 + LB1*LB3);
			QQ = 2 * (LA2*LA3 + LB2*LB3);
			RR = (pow(LA1, 2) + pow(LB1, 2))*pow(CosTheta[2], 2) + (pow(LA2, 2) + pow(LB2, 2))*pow(SinTheta[2], 2)
				+ (pow(LA3, 2) + pow(LB3, 2)) + 2 * (LA1*LA2 + LB1*LB2)*SinTheta[2] * CosTheta[2] - (JA1*JA1 + JA2*JA2);
			PQR = PP*PP + QQ*QQ - RR*RR;

			if (PQR < 0)//unreachable
				return 1;

			CosTheta[2] = (-PP*RR - elbow*QQ*sqrt(PQR)) / (PP*PP + QQ*QQ);
			SinTheta[2] = (-QQ*RR + elbow*PP*sqrt(PQR)) / (PP*PP + QQ*QQ);
			theta[2] = atan2(SinTheta[2], CosTheta[2]);

			//find theta 2
			JA4 = LA1*CosTheta[2] + LA2*SinTheta[2] + LA3;
			JA5 = LB1*CosTheta[2] + LB2*SinTheta[2] + LB3;

			CosTheta[1] = (JA1*JA4 + JA2*JA5) / (JA1*JA1 + JA2*JA2);
			SinTheta[1] = (JA2*JA4 - JA1*JA5) / (JA1*JA1 + JA2*JA2);
			theta[1] = atan2(SinTheta[1], CosTheta[1]);

			//--------------------------------------------------------------------------------------------------
			nx03
				= GA1*CosTheta[0] * CosTheta[1] * CosTheta[2] + GA2*CosTheta[0] * SinTheta[1] * CosTheta[2]
				+ GA3*SinTheta[0] * CosTheta[1] * CosTheta[2] + GA4*SinTheta[0] * SinTheta[1] * CosTheta[2]
				+ GA5*CosTheta[0] * CosTheta[1] * SinTheta[2] + GA6*CosTheta[0] * SinTheta[1] * SinTheta[2]
				+ GA7*SinTheta[0] * CosTheta[1] * SinTheta[2] + GA8*SinTheta[0] * SinTheta[1] * SinTheta[2]
				+ GB1*CosTheta[1] * CosTheta[2] + GB2*SinTheta[1] * CosTheta[2]
				+ GB3*CosTheta[1] * SinTheta[2] + GB4*SinTheta[1] * SinTheta[2]
				+ J[2] * AB1*CosTheta[0] * CosTheta[2] + J[2] * AB2*SinTheta[0] * CosTheta[2]
				+ K[2] * AB1*CosTheta[0] * SinTheta[2] + K[2] * AB2*SinTheta[0] * SinTheta[2]
				+ J[2] * D[0] * D[1] * CosTheta[2] + K[2] * D[0] * D[1] * SinTheta[2];
			ny03
				= GE1*CosTheta[0] * CosTheta[1] * CosTheta[2] + GE2*CosTheta[0] * SinTheta[1] * CosTheta[2]
				+ GE3*SinTheta[0] * CosTheta[1] * CosTheta[2] + GE4*SinTheta[0] * SinTheta[1] * CosTheta[2]
				+ GE5*CosTheta[0] * CosTheta[1] * SinTheta[2] + GE6*CosTheta[0] * SinTheta[1] * SinTheta[2]
				+ GE7*SinTheta[0] * CosTheta[1] * SinTheta[2] + GE8*SinTheta[0] * SinTheta[1] * SinTheta[2]
				+ GF1*CosTheta[1] * CosTheta[2] + GF2*SinTheta[1] * CosTheta[2]
				+ GF3*CosTheta[1] * SinTheta[2] + GF4*SinTheta[1] * SinTheta[2]
				+ J[2] * AF1*CosTheta[0] * CosTheta[2] + J[2] * AF2*SinTheta[0] * CosTheta[2]
				+ K[2] * AF1*CosTheta[0] * SinTheta[2] + K[2] * AF2*SinTheta[0] * SinTheta[2]
				+ J[2] * H[0] * D[1] * CosTheta[2] + K[2] * H[0] * D[1] * SinTheta[2];
			nz03
				= GH1*CosTheta[0] * CosTheta[1] * CosTheta[2] + GH3*CosTheta[0] * SinTheta[1] * CosTheta[2]
				+ GH2*SinTheta[0] * CosTheta[1] * CosTheta[2] + GH4*SinTheta[0] * SinTheta[1] * CosTheta[2]
				+ GH5*CosTheta[0] * CosTheta[1] * SinTheta[2] + GH7*CosTheta[0] * SinTheta[1] * SinTheta[2]
				+ GH6*SinTheta[0] * CosTheta[1] * SinTheta[2] + GH8*SinTheta[0] * SinTheta[1] * SinTheta[2]
				+ GI1*CosTheta[1] * CosTheta[2] + GI2*SinTheta[1] * CosTheta[2]
				+ GI3*CosTheta[1] * SinTheta[2] + GI4*SinTheta[1] * SinTheta[2]
				+ J[2] * AI1*CosTheta[0] * CosTheta[2] + J[2] * AI2*SinTheta[0] * CosTheta[2]
				+ K[2] * AI1*CosTheta[0] * SinTheta[2] + K[2] * AI2*SinTheta[0] * SinTheta[2]
				+ J[2] * L[0] * D[1] * CosTheta[2] + K[2] * L[0] * D[1] * SinTheta[2];

			ox03
				= GA5*CosTheta[0] * CosTheta[1] * CosTheta[2] + GA6*CosTheta[0] * SinTheta[1] * CosTheta[2]
				+ GA7*SinTheta[0] * CosTheta[1] * CosTheta[2] + GA8*SinTheta[0] * SinTheta[1] * CosTheta[2]
				- GA1*CosTheta[0] * CosTheta[1] * SinTheta[2] - GA2*CosTheta[0] * SinTheta[1] * SinTheta[2]
				- GA3*SinTheta[0] * CosTheta[1] * SinTheta[2] - GA4*SinTheta[0] * SinTheta[1] * SinTheta[2]
				+ GB3*CosTheta[1] * CosTheta[2] + GB4*SinTheta[1] * CosTheta[2]
				- GB1*CosTheta[1] * SinTheta[2] - GB2*SinTheta[1] * SinTheta[2]
				+ K[2] * AB1*CosTheta[0] * CosTheta[2] + K[2] * AB2*SinTheta[0] * CosTheta[2]
				- J[2] * AB1*CosTheta[0] * SinTheta[2] - J[2] * AB2*SinTheta[0] * SinTheta[2]
				+ K[2] * D[0] * D[1] * CosTheta[2] - J[2] * D[0] * D[1] * SinTheta[2];
			oy03
				= GE5*CosTheta[0] * CosTheta[1] * CosTheta[2] + GE6*CosTheta[0] * SinTheta[1] * CosTheta[2]
				+ GE7*SinTheta[0] * CosTheta[1] * CosTheta[2] + GE8*SinTheta[0] * SinTheta[1] * CosTheta[2]
				- GE1*CosTheta[0] * CosTheta[1] * SinTheta[2] - GE2*CosTheta[0] * SinTheta[1] * SinTheta[2]
				- GE3*SinTheta[0] * CosTheta[1] * SinTheta[2] - GE4*SinTheta[0] * SinTheta[1] * SinTheta[2]
				+ GF3*CosTheta[1] * CosTheta[2] + GF4*SinTheta[1] * CosTheta[2]
				- GF1*CosTheta[1] * SinTheta[2] - GF2*SinTheta[1] * SinTheta[2]
				+ K[2] * AF1*CosTheta[0] * CosTheta[2] + K[2] * AF2*SinTheta[0] * CosTheta[2]
				- J[2] * AF1*CosTheta[0] * SinTheta[2] - J[2] * AF2*SinTheta[0] * SinTheta[2]
				+ K[2] * H[0] * D[1] * CosTheta[2] - J[2] * H[0] * D[1] * SinTheta[2];
			oz03
				= GH5*CosTheta[0] * CosTheta[1] * CosTheta[2] + GH7*CosTheta[0] * SinTheta[1] * CosTheta[2]
				+ GH6*SinTheta[0] * CosTheta[1] * CosTheta[2] + GH8*SinTheta[0] * SinTheta[1] * CosTheta[2]
				- GH1*CosTheta[0] * CosTheta[1] * SinTheta[2] - GH3*CosTheta[0] * SinTheta[1] * SinTheta[2]
				- GH2*SinTheta[0] * CosTheta[1] * SinTheta[2] - GH4*SinTheta[0] * SinTheta[1] * SinTheta[2]
				+ GI3*CosTheta[1] * CosTheta[2] + GI4*SinTheta[1] * CosTheta[2]
				- GI1*CosTheta[1] * SinTheta[2] - GI2*SinTheta[1] * SinTheta[2]
				+ K[2] * AI1*CosTheta[0] * CosTheta[2] + K[2] * AI2*SinTheta[0] * CosTheta[2]
				- J[2] * AI1*CosTheta[0] * SinTheta[2] - J[2] * AI2*SinTheta[0] * SinTheta[2]
				+ K[2] * L[0] * D[1] * CosTheta[2] - J[2] * L[0] * D[1] * SinTheta[2];

			ax03
				= GD1*CosTheta[0] * CosTheta[1] + GD2*CosTheta[0] * SinTheta[1]
				+ GD3*SinTheta[0] * CosTheta[1] + GD4*SinTheta[0] * SinTheta[1]
				+ GD5*CosTheta[1] + GD6*SinTheta[1]
				+ L[2] * AB1*CosTheta[0] + L[2] * AB2*SinTheta[0]
				+ L[2] * D[0] * D[1];
			ay03
				= GG1*CosTheta[0] * CosTheta[1] + GG2*CosTheta[0] * SinTheta[1]
				+ GG3*SinTheta[0] * CosTheta[1] + GG4*SinTheta[0] * SinTheta[1]
				+ GG5*CosTheta[1] + GG6*SinTheta[1]
				+ L[2] * AF1*CosTheta[0] + L[2] * AF2*SinTheta[0]
				+ L[2] * H[0] * D[1];
			az03
				= GJ1*CosTheta[0] * CosTheta[1] + GJ3*CosTheta[0] * SinTheta[1]
				+ GJ2*SinTheta[0] * CosTheta[1] + GJ4*SinTheta[0] * SinTheta[1]
				+ GJ5*CosTheta[1] + GJ6*SinTheta[1]
				+ L[2] * AI1*CosTheta[0] + L[2] * AI2*SinTheta[0]
				+ L[2] * L[0] * D[1];

			KA1 = nx03*ax + ny03*ay + nz03*az;
			KA2 = ox03*nx + oy03*ny + oz03*nz;
			KA3 = ox03*ox + oy03*oy + oz03*oz;
			KA4 = ox03*ax + oy03*ay + oz03*az;
			KA5 = ax03*ax + ay03*ay + az03*az;
			//----------------------------------------------------------------------------------------------------

			//find theta 5
			//3T6 의 ay를 PQR로 풀었다.
			PP = FH1*CosTheta[3] + FH3*SinTheta[3] - FH5;
			QQ = FH2*CosTheta[3] + FH4*SinTheta[3] + FH6;
			RR = EA7*(DF1*CosTheta[3] + DF2*SinTheta[3] - DF3) - KA4;
			PQR = PP*PP + QQ*QQ - RR*RR;

			if (PQR < 0)//unreachable
				return 1;

			CosTheta[4] = (-PP*RR - wrist*QQ*sqrt(PQR)) / (PP*PP + QQ*QQ);
			SinTheta[4] = (-QQ*RR + wrist*PP*sqrt(PQR)) / (PP*PP + QQ*QQ);
			theta[4] = atan2(SinTheta[4], CosTheta[4]);

			//find theta 4
			//3T6 의 ax를 c4에 대해 정리, az를 s4에 대해 정리하여 풀었다.
			JA1 = FA1*CosTheta[4] + FA2*SinTheta[4] + EA7*DB1;
			JA2 = FA3*CosTheta[4] + FA4*SinTheta[4] + EA7*DB2;
			JA3 = -FA5*CosTheta[4] + FA6*SinTheta[4] - EA7*DB3;

			if (fabs(JA1) < 10e-10)//singular
				return 2;
			CosTheta[3] = (KA1 - JA2*sin(theta[3]) - JA3) / JA1;

			JA1 = FJ1*CosTheta[4] + FJ2*SinTheta[4] + EA7*DI1;
			JA2 = -FJ3*CosTheta[4] + FJ4*SinTheta[4] + EA7*DI2;
			JA3 = FJ5*CosTheta[4] + FJ6*SinTheta[4] + EA7*DI3;

			if (fabs(JA2) < 10e-10)//singular
				return 2;
			SinTheta[3] = (KA5 - JA1*cos(theta[3]) - JA3) / JA2;

			theta[3] = atan2(SinTheta[3], CosTheta[3]);

			//find theta 6
			//3T6 의 ny, oy 를 C6, S6 에 대해 정리하여 풀었다.
			JA1 = FD1*CosTheta[3] * CosTheta[4] + FD2*CosTheta[3] * SinTheta[4] + FD3*SinTheta[3] * CosTheta[4] + FD4*SinTheta[3] * SinTheta[4]
				- FE1*CosTheta[4] + FE2*SinTheta[4] + DF1*EA3*CosTheta[3] + DF2*EA3*SinTheta[3] - DF3*EA3;
			JA2 = FD5*CosTheta[3] * CosTheta[4] + FD6*CosTheta[3] * SinTheta[4] + FD7*SinTheta[3] * CosTheta[4] + FD8*SinTheta[3] * SinTheta[4]
				- FE3*CosTheta[4] + FE4*SinTheta[4] + DF1*EA4*CosTheta[3] + DF2*EA4*SinTheta[3] - DF3*EA4;
			PQR = JA1*JA1 + JA2*JA2;
			if (PQR < 10e-10)//singular
				return 2;

			CosTheta[5] = (KA2*JA1 + KA3*JA2) / PQR;
			SinTheta[5] = (KA2*JA2 - KA3*JA1) / PQR;
			theta[5] = atan2(SinTheta[5], CosTheta[5]);
		}

		//반복 확인
		if (iteration == 1)
		{
			for (i = 0; i<6; i++)
				thetaPrev[i] = theta[i];
		}
		else
		{
			for (i = 0; i<6; i++)
				Tolerance[i] = fabs(theta[i] - thetaPrev[i]);

			if ((Tolerance[0] < 10e-10 && Tolerance[1] < 10e-10 && Tolerance[2] < 10e-10 &&
				Tolerance[3] < 10e-10 && Tolerance[4] < 10e-10 && Tolerance[5] < 10e-10) || iteration >= 100)
				FindThetaFlag = 1;

			for (i = 0; i<6; i++)
				thetaPrev[i] = theta[i];
		}
	}

	//	cout << "interation Num = " << iteration << endl;
	for (i = 0; i<6; i++)
		theta[i] = RobJointSign[i] * (theta[i] * pi_de + RobJointOffset[i]);

	return 0;
}

int COptUtil::FindFinalTheta(double TargetRobJointTheta[], double BeginRobJointTheta[], int MotionType, int JointInterPolMode, int TurnNumberValue[], double JointLimit[][6], int kin_type)
{
	if ((MotionType == JOINT_MOTION && JointInterPolMode == SHORTESTANGLE) || MotionType == LINEAR_MOTION)
	{
		double DeltaTheta;
		int i, TN;
		for (i = 0; i<6; i++)
		{
			double new_value = TargetRobJointTheta[i];
			while (1)
			{
				if (new_value - BeginRobJointTheta[i] > 180.0f)
				{
					new_value -= 360.0f;
				}
				else if (new_value - BeginRobJointTheta[i] <= -180.0f)
				{
					new_value += 360.0f;
				}
				else
				{
					break;
				}
			}
			//DeltaTheta = TargetRobJointTheta[i] - BeginRobJointTheta[i];
			//
			//if(DeltaTheta >= 180)
			//	DeltaTheta = DeltaTheta - 180;
			//else if(DeltaTheta <= -180)
			//	DeltaTheta = DeltaTheta + 180;

			//TN = int(DeltaTheta/360);

			//TargetRobJointTheta[i] = TargetRobJointTheta[i] + TN * 360;
			TargetRobJointTheta[i] = new_value;
			//float low_limit = JointLimit[0][i];
			//float up_limit = JointLimit[1][i];
			//while (TargetRobJointTheta[i] < low_limit)
			//{
			//	TargetRobJointTheta[i] += 360.0f;
			//}
			//while (TargetRobJointTheta[i] > up_limit)
			//{
			//	TargetRobJointTheta[i] -= 360.0f;
			//}
		}
		//TRACE("%lf - %lf\n", TargetRobJointTheta[3], BeginRobJointTheta[3]);
	}
	else if (MotionType == JOINT_MOTION && JointInterPolMode == TURNNUMBER)
	{
		int i;
		for (i = 0; i<6; i++)
			TargetRobJointTheta[i] = TargetRobJointTheta[i] + TurnNumberValue[i] * 360;
	}

	if (kin_type == 1)
		TargetRobJointTheta[2] = TargetRobJointTheta[1] + TargetRobJointTheta[2];

	return 0;
}

//int COptUtil::ThetaGenerator(double CurRobJointTheta[], double CurBaseAuxTheta[], double CurMountAuxTheta[],										// output
//	double CurrentTime,																													// input from user
//	double ConstantVelTime, double AccelerateTime,																						// input from motion planner
//	double BaseAuxParaMat[][4][4], double BaseAuxRobPosMat[][4],																// input from 데이터 정리
//	double MountMat[][4], double TcpMat[][4], int TargetConfig[], double RobJointDV[][4][4],									// input from 데이터 정리
//	double BeginPosTcpFromBaseAux[][4], double TargetPosTcpFromBaseAux[][4], double DU1[][3], double TcpRotationValue, 			// input from 데이터 정리
//	double TargetRobJointTheta[],																			// input from inverse kinematics
//	COptDeviceAttribute* pAttribute,		// input from Device attribute
//	double BeginRobJointTheta[], double BeginBaseAuxTheta[], double BeginMountAuxTheta[],										// input from Device state
//	double TargetBaseAuxTheta[], double TargetMountAuxTheta[], int MotionType, int TurnNumberValue[], double JointLimit[][6],
//	double BaseAuxJointDV[][4][4], int kin_type)							// input from Tag attribute
//{
//	return ThetaGenerator(CurRobJointTheta, CurBaseAuxTheta, CurMountAuxTheta,										// output
//		CurrentTime,																													// input from user
//		ConstantVelTime, AccelerateTime,																						// input from motion planner
//		BaseAuxParaMat, BaseAuxRobPosMat,																// input from 데이터 정리
//		MountMat, TcpMat, TargetConfig, RobJointDV,									// input from 데이터 정리
//		BeginPosTcpFromBaseAux, TargetPosTcpFromBaseAux, DU1, TcpRotationValue, 			// input from 데이터 정리
//		TargetRobJointTheta,																			// input from inverse kinematics
//		pAttribute->aa, pAttribute->dd, pAttribute->alpha, pAttribute->JointInterPolMode,																// input from Device Attribute
//		pAttribute->BaseAuxNum, pAttribute->MountAuxNum, pAttribute->BaseAuxType, pAttribute->MountAuxType, pAttribute->RobJointSign, pAttribute->RobJointOffset,		// input from Device attribute
//		BeginRobJointTheta, BeginBaseAuxTheta, BeginMountAuxTheta,										// input from Device state
//		TargetBaseAuxTheta, TargetMountAuxTheta, MotionType, TurnNumberValue, JointLimit, BaseAuxJointDV, kin_type);
//}
//
//int COptUtil::ThetaGenerator(double CurRobJointTheta[], double CurBaseAuxTheta[], double CurMountAuxTheta[],										// output
//	double CurrentTime,																													// input from user
//	double ConstantVelTime, double AccelerateTime,																						// input from motion planner
//	double BaseAuxParaMat[][4][4], double BaseAuxRobPosMat[][4],																// input from 데이터 정리
//	double MountMat[][4], double TcpMat[][4], int TargetConfig[], double RobJointDV[][4][4],									// input from 데이터 정리
//	double BeginPosTcpFromBaseAux[][4], double TargetPosTcpFromBaseAux[][4], double DU1[][3], double TcpRotationValue, 			// input from 데이터 정리
//	double TargetRobJointTheta[],																			// input from inverse kinematics
//	double aa[], double dd[], double alpha, int JointInterPolMode,																// input from Device Attribute
//	int BaseAuxNum, int MountAuxNum, int BaseAuxType[], int MountAuxType[], int RobJointSign[], double RobJointOffset[],		// input from Device attribute
//	double BeginRobJointTheta[], double BeginBaseAuxTheta[], double BeginMountAuxTheta[],										// input from Device state
//	double TargetBaseAuxTheta[], double TargetMountAuxTheta[], int MotionType, int TurnNumberValue[], double JointLimit[][6],
//	double BaseAuxJointDV[][4][4], int kin_type)							// input from Tag attribute
//{
//	int resultFLMT = 0;
//	//1. 시간 함수 Ut 구하기
//	//Ut = UtFunction()
//	double Ut;
//	FindUtFunction(&Ut, CurrentTime, ConstantVelTime, AccelerateTime);
//
//	//2. 현재 시간 Theta 구하기
//	switch (MotionType)
//	{
//	case JOINT_MOTION:
//		FindJointMotionTheta(CurRobJointTheta, CurBaseAuxTheta, CurMountAuxTheta,	// output
//			Ut,																		// input from Utfunction
//			TargetRobJointTheta,													// input from inverse kinematics
//			BaseAuxNum, MountAuxNum, 												// input from Device attribute
//			BeginRobJointTheta, BeginBaseAuxTheta, BeginMountAuxTheta,				// input from Device state
//			TargetBaseAuxTheta, TargetMountAuxTheta, kin_type);								// input from Tag attribute
//		break;
//	case LINEAR_MOTION:
//		resultFLMT = FindLinearMotionTheta(CurRobJointTheta, CurBaseAuxTheta, CurMountAuxTheta,		// output
//			Ut,																						// input from Utfunction
//			BaseAuxParaMat, BaseAuxRobPosMat,														// input from 데이터 정리
//			MountMat, TcpMat, TargetConfig, RobJointDV,												// input from 데이터 정리
//			BeginPosTcpFromBaseAux, TargetPosTcpFromBaseAux, DU1, TcpRotationValue, 				// input from 데이터 정리
//			aa, dd, alpha, JointInterPolMode,														// input from Device Attribute
//			BaseAuxNum, MountAuxNum, BaseAuxType, MountAuxType, RobJointSign, RobJointOffset,		// input from Device attribute
//			BeginRobJointTheta, BeginBaseAuxTheta, BeginMountAuxTheta,								// input from Device state
//			TargetBaseAuxTheta, TargetMountAuxTheta, MotionType, TurnNumberValue, JointLimit, BaseAuxJointDV, kin_type);					// input from Tag attribute
//		break;
//	}
//
//	switch (resultFLMT)
//	{
//	case 0:
//		break;
//	case 1:
//		return 1;
//		break;
//	case 2:
//		return 2;
//		break;
//	}
//
//	for (int i = 0; i < 6; i++)
//	{
//		if ((CurRobJointTheta[i] - JointLimit[0][i]) < -0.01f || (CurRobJointTheta[i] - JointLimit[1][i]) > 0.01f)
//		{
//			return 3;
//		}
//	}
//
//	return 0;
//}

int COptUtil::FindUtFunction(double* Ut, double CurrentTime, double ConstantVelTime, double AccelerateTime)
{
	//등속 시간이 가속 시간보다 클 경우
	if (ConstantVelTime > AccelerateTime)
	{
		//1. 0 <= CurrentTime < AccelerateTime
		//Ut = CurrentTime^2 / ( 2 * ConstantVelocityTime * AccelerateTime )
		if (CurrentTime >= 0 && CurrentTime < AccelerateTime)
		{
			*Ut = pow(CurrentTime, 2) / (2 * ConstantVelTime*AccelerateTime);
		}
		//2. AccelerateTime <= CurrentTime < ConstantVelocityTime
		//Ut = ( CurrentTime – AccelerateTime/2 ) / ConstantVelocityTime
		else if (CurrentTime >= AccelerateTime && CurrentTime < ConstantVelTime)
		{
			*Ut = (CurrentTime - AccelerateTime / 2) / ConstantVelTime;
		}
		//3. ConstantVelocityTime <= CurrentTime <= ConstantVelocityTime + AccelerateTime
		//Ut = 1 – ( (ConstantVelocityTime + AccelerateTime) - CurrentTime )^2 / ( 2 * ConstantVelocityTime * AccelerateTime )
		else if (CurrentTime >= ConstantVelTime && CurrentTime <= (ConstantVelTime + AccelerateTime))
		{
			if (AccelerateTime <= 0.0f)
			{
				*Ut = 1.0f;
			}
			else
			{
				*Ut = 1 - pow(((ConstantVelTime + AccelerateTime) - CurrentTime), 2) / (2 * ConstantVelTime*AccelerateTime);
			}
		}
	}
	//등속 시간이 가속 시간보다 작거나 같을 경우
	else
	{
		//1. 0 <= CurrentTime < AccelerateTime
		//Ut = CurrentTime^2 / ( 2 * AccelerateTime * AccelerateTime )
		if (CurrentTime >= 0 && CurrentTime < AccelerateTime)
		{
			*Ut = pow(CurrentTime, 2) / (2 * pow(AccelerateTime, 2));
		}
		//2. AccelerateTime <= CurrentTime <= 2 * AccelerateTime
		//Ut = 1 – ( (2 * AccelerateTime) - CurrentTime )^2 / ( 2 * AccelerateTime^2 )
		else if (CurrentTime >= AccelerateTime && CurrentTime <= (2 * AccelerateTime))
		{
			*Ut = 1 - pow(((2 * AccelerateTime) - CurrentTime), 2) / (2 * pow(AccelerateTime, 2));
		}
	}

	return 0;
}

int COptUtil::FindJointMotionTheta(double CurRobJointTheta[], double CurBaseAuxTheta[], double CurMountAuxTheta[],	// output
	double Ut,																								// input from Utfunction
	double TargetRobJointTheta[],																			// input from inverse kinematics
	int BaseAuxNum, int MountAuxNum, 																		// input from Device attribute
	double BeginRobJointTheta[], double BeginBaseAuxTheta[], double BeginMountAuxTheta[],					// input from Device state
	double TargetBaseAuxTheta[], double TargetMountAuxTheta[], int kin_type)												// input from Tag attribute
{
	int i;
	//1. 조인트별 변위량 찾기
	//DeltaTheta = TargetTheta – BeginTheta
	double DeltaRobJointTheta[6] = { 0 };
	double DeltaBaseAuxTheta[MAXBASEAUXNUM] = { 0 };
	double DeltaMountAuxTheta[MAXMOUNTAUXNUM] = { 0 };

	for (i = 0; i<6; i++)
		DeltaRobJointTheta[i] = TargetRobJointTheta[i] - BeginRobJointTheta[i];
	for (i = 0; i<BaseAuxNum; i++)
		DeltaBaseAuxTheta[i] = TargetBaseAuxTheta[i] - BeginBaseAuxTheta[i];
	for (i = 0; i<MountAuxNum; i++)
		DeltaMountAuxTheta[i] = TargetMountAuxTheta[i] - BeginMountAuxTheta[i];

	//2. 현재 시간 조인트 값 찾기
	//CurrentTheta = BeginTheta + Ut * DeltaTheta
	for (i = 0; i<6; i++)
		CurRobJointTheta[i] = BeginRobJointTheta[i] + Ut*DeltaRobJointTheta[i];
	for (i = 0; i<BaseAuxNum; i++)
		CurBaseAuxTheta[i] = BeginBaseAuxTheta[i] + Ut*DeltaBaseAuxTheta[i];
	for (i = 0; i<MountAuxNum; i++)
		CurMountAuxTheta[i] = BeginMountAuxTheta[i] + Ut*DeltaMountAuxTheta[i];

	/*
	for(i=0; i<6; i++)
	cout << CurRobJointTheta[i] << endl;
	for(i=0; i<BaseAuxNum; i++)
	cout << CurBaseAuxTheta[i] << endl;
	for(i=0; i<MountAuxNum; i++)
	cout << CurMountAuxTheta[i] << endl;
	*/
	return 0;
}

int COptUtil::FindLinearMotionTheta(double CurRobJointTheta[], double CurBaseAuxTheta[], double CurMountAuxTheta[],						// output
	double Ut,																													// input from Utfunction
	double BaseAuxParaMat[][4][4], double BaseAuxRobPosMat[][4],																// input from 데이터 정리
	double MountMat[][4], double TcpMat[][4], int TargetConfig[], double RobJointDV[][4][4],									// input from 데이터 정리
	double BeginPosTcpFromBaseAux[][4], double TargetPosTcpFromBaseAux[][4], double DU1[][3], double TcpRotationValue, 			// input from 데이터 정리
	double aa[], double dd[], double alpha, int JointInterPolMode,																// input from Device Attribute
	int BaseAuxNum, int MountAuxNum, int BaseAuxType[], int MountAuxType[], int RobJointSign[], double RobJointOffset[],		// input from Device attribute
	double BeginRobJointTheta[], double BeginBaseAuxTheta[], double BeginMountAuxTheta[],										// input from Device state
	double TargetBaseAuxTheta[], double TargetMountAuxTheta[], int MotionType, int TurnNumberValue[], double JointLimit[][6],
	double(*BaseAuxJointDV)[4][4], int kin_type)							// input from Tag attribute
{
	int i, j, k;

	double prev_rob_joint_theta[6];
	for (i = 0; i<6; i++)
		prev_rob_joint_theta[i] = CurRobJointTheta[i];
	//1. 부가축 base 기준 Tcp 변위량 구하기 ( array[3] )
	//DeltaTcpPositionFromBaseAux = TargetPosTcpPositionFromBaseAux – BeginPosTcpPositionFromBaseAux
	double DeltaTcpPositionFromBaseAux[3] = { 0 };
	for (i = 0; i<3; i++)
		DeltaTcpPositionFromBaseAux[i] = TargetPosTcpFromBaseAux[i][3] - BeginPosTcpFromBaseAux[i][3];
	/*
	for(i=0; i<3; i++)
	cout << DeltaTcpPositionFromBaseAux[i] << "\t";
	cout << endl;
	*/

	//2. 부가축 조인트별 변위량 찾기
	//DeltaBaseAuxTheta = TargetBaseAuxTheta – BeginBaseAuxTheta
	double DeltaBaseAuxTheta[MAXBASEAUXNUM] = { 0 };
	double DeltaMountAuxTheta[MAXMOUNTAUXNUM] = { 0 };

	for (i = 0; i<BaseAuxNum; i++)
		DeltaBaseAuxTheta[i] = TargetBaseAuxTheta[i] - BeginBaseAuxTheta[i];
	for (i = 0; i<MountAuxNum; i++)
		DeltaMountAuxTheta[i] = TargetMountAuxTheta[i] - BeginMountAuxTheta[i];
	/*
	for(i=0; i<3; i++)
	cout << DeltaBaseAuxTheta[i] << "\t";
	cout << endl;
	for(i=0; i<3; i++)
	cout << DeltaMountAuxTheta[i] << "\t";
	cout << endl;
	*/

	//3. 현재 시간 부가축 조인트 값 찾기
	//CurrentBaseAuxTheta = BeginBaseAuxTheta + Ut * DeltaBaseAuxTheta
	for (i = 0; i<BaseAuxNum; i++)
		CurBaseAuxTheta[i] = BeginBaseAuxTheta[i] + Ut*DeltaBaseAuxTheta[i];
	for (i = 0; i<MountAuxNum; i++)
		CurMountAuxTheta[i] = BeginMountAuxTheta[i] + Ut*DeltaMountAuxTheta[i];

	//4. 현재 부가축 base 에서 로봇 base 위치 4X4 행렬
	//CurRobPosMatFromBaseAux = Trans() * Rotz() * Roty() * Rotx() * Trans/Rot(CurrentBaseAuxTheta) * … * Trans() * Rotz() * Roty() * Rotx()	---BaseAuxPara * BaseAuxRobPos
	double CurRobPosMatFromBaseAux[4][4] = { 0 };

	BaseAuxFK(CurRobPosMatFromBaseAux, BaseAuxParaMat, CurBaseAuxTheta, BaseAuxNum, BaseAuxType, BaseAuxJointDV);
	MultiplyMAT(CurRobPosMatFromBaseAux, CurRobPosMatFromBaseAux, BaseAuxRobPosMat);
	/*
	for(i=0; i<3; i++)
	cout << CurRobPosMatFromBaseAux[i][3] << "\t";
	cout << endl;
	*/

	//5. 부가축 base 기준으로 현재 Tcp 위치 3X1 행렬
	//CurrentPosTcpPositionFromBaseAux = BeginPosTcpFromBaseAux + Ut * DeltaTcpPositionFromBaseAux
	double CurrentPosTcpPositionFromBaseAux[3] = { 0 };
	for (i = 0; i<3; i++)
		CurrentPosTcpPositionFromBaseAux[i] = BeginPosTcpFromBaseAux[i][3] + Ut * DeltaTcpPositionFromBaseAux[i];
	/*
	for(i=0; i<3; i++)
	cout << CurrentPosTcpPositionFromBaseAux[i] << "\t";
	cout << endl;
	*/

	//6. 부가축 base 기준으로 K-vector 값 구하기
	//KVector = [ kx, ky, kz ] = [ oz-ay, ax-nz, ny-ox ] / ( 2 * sin(TcpRotationValue) )
	double CurrentPosTcpOrientationFromBaseAux[3][3] = { 0 };
	if (TcpRotationValue == 0)
	{
		for (i = 0; i<3; i++)
			for (j = 0; j<3; j++)
				CurrentPosTcpOrientationFromBaseAux[i][j] = BeginPosTcpFromBaseAux[i][j];
	}
	else
	{
		double kx, ky, kz;
		kx = (DU1[2][1] - DU1[1][2]) / (2 * sin(TcpRotationValue));
		ky = (DU1[0][2] - DU1[2][0]) / (2 * sin(TcpRotationValue));
		kz = (DU1[1][0] - DU1[0][1]) / (2 * sin(TcpRotationValue));

		//7. 부가축 base 기준으로 D(U(t)) 구하기
		//St = sin( Ut * TcpRotationValue )	Ct = cos( Ut * TcpRotationValue )	Vt = 1-Ct
		//D(U(t)) =	DUt = [ [ kx*kx*Vt + Ct         ky*kx*Vt – kz*St     kz*kx*Vt + ky*St ], 
		//					[ kx*ky*Vt + kz*St      ky*ky*Vt + Ct        kz*ky*Vt - kx*St ], 
		//					[ kx*kz*Vt - ky*St      ky*kz*Vt + kx*St     kz*kz*Vt + Ct    ] ]
		double St, Ct, Vt;
		double DUt[3][3] = { 0 };
		St = sin(Ut * TcpRotationValue);
		Ct = cos(Ut * TcpRotationValue);
		Vt = 1 - Ct;
		DUt[0][0] = kx*kx*Vt + Ct;
		DUt[0][1] = ky*kx*Vt - kz*St;
		DUt[0][2] = kz*kx*Vt + ky*St;

		DUt[1][0] = kx*ky*Vt + kz*St;
		DUt[1][1] = ky*ky*Vt + Ct;
		DUt[1][2] = kz*ky*Vt - kx*St;

		DUt[2][0] = kx*kz*Vt - ky*St;
		DUt[2][1] = ky*kz*Vt + kx*St;
		DUt[2][2] = kz*kz*Vt + Ct;
		/*
		for(i=0; i<3; i++)
		{
		for(j=0; j<3; j++)
		{
		cout << DUt[i][j] << "\t";
		}
		cout << endl;
		}
		cout << endl;
		*/

		//8. 부가축 base 기준으로 현재 Tcp 방향 계산 3X3행렬
		//R(t) = CurrentPosTcpOrientationFromBaseAux = DUt * BeginPosTcpFromBaseAux
		for (i = 0; i<3; i++)
			for (j = 0; j<3; j++)
				for (k = 0; k<3; k++)
					CurrentPosTcpOrientationFromBaseAux[i][j] = CurrentPosTcpOrientationFromBaseAux[i][j] + DUt[i][k] * BeginPosTcpFromBaseAux[k][j];
	}
	/*
	for(i=0; i<3; i++)
	{
	for(j=0; j<3; j++)
	{
	cout << CurrentPosTcpOrientationFromBaseAux[i][j] << "\t";
	}
	cout << endl;
	}
	cout << endl;
	*/

	//9. 부가축 base 기준으로 현재 tcp 위치
	//CurrentPosTcpFromBaseAux = [ CurrentPosTcpOrientationFromBaseAux, CurrentPosTcpPositionFromBaseAux ]
	double CurrentPosTcpFromBaseAux[4][4] = { 0 };
	for (i = 0; i<3; i++)
		for (j = 0; j<3; j++)
			CurrentPosTcpFromBaseAux[i][j] = CurrentPosTcpOrientationFromBaseAux[i][j];
	for (i = 0; i<3; i++)
		CurrentPosTcpFromBaseAux[i][3] = CurrentPosTcpPositionFromBaseAux[i];
	for (i = 0; i<3; i++)
		CurrentPosTcpFromBaseAux[3][i] = 0;
	CurrentPosTcpFromBaseAux[3][3] = 1;
	/*
	for(i=0; i<4; i++)
	{
	for(j=0; j<4; j++)
	cout << CurrentPosTcpFromBaseAux[i][j] << "\t";
	cout << endl;
	}
	cout << endl;
	*/

	//10. 로봇 기준 현재 tcp 위치
	//CurrentPosTcp = Inv( CurRobPosMatFromBaseAux ) * CurrentPosTcpFromBaseAux
	double CurrentPosTcp[4][4] = { 0 };
	double InvCurRobPosMatFromBaseAux[4][4] = { 0 };
	InverseMAT(InvCurRobPosMatFromBaseAux, CurRobPosMatFromBaseAux);
	MultiplyMAT(CurrentPosTcp, InvCurRobPosMatFromBaseAux, CurrentPosTcpFromBaseAux);
	/*
	for(i=0; i<3; i++)
	cout << CurrentPosTcp[i][3] << "\t";
	cout << endl;
	*/

	//11. 로봇 base 기준 현재 0T6 계산
	//CurrentPos0T6 = CurrentPosTcp * inv( TcpMat ) * inv( MountMat )
	double CurrentPos0T6[4][4] = { 0 };
	double InvTcpMat[4][4] = { 0 };
	double InvMountMat[4][4] = { 0 };
	InverseMAT(InvTcpMat, TcpMat);
	InverseMAT(InvMountMat, MountMat);
	MultiplyMAT(CurrentPos0T6, CurrentPosTcp, InvTcpMat);
	MultiplyMAT(CurrentPos0T6, CurrentPos0T6, InvMountMat);
	/*
	for(i=0; i<4; i++)
	{
	for(j=0; j<4; j++)
	cout << CurrentPos0T6[i][j] << "\t";
	cout << endl;
	}
	cout << endl;
	*/
	//12. 현재 위치일때 TargetTheta 찾기 - Inverse Kin 풀기
	//CurrRobJointTheta = RobotIK(CurRobJointTheta, CurrentPos0T6, aa, dd, alpha, RobJointDV, TargetConfig, RobJointSign, RobJointOffset)
	int resultRobotIK;

	resultRobotIK = RobotIK(CurRobJointTheta, CurrentPos0T6, aa, dd, alpha, RobJointDV, TargetConfig, RobJointSign, RobJointOffset);

	switch (resultRobotIK)
	{
	case 0:
		//		cout << "Find Current Theta" << endl;
		/*		for(i=0; i<6; i++)
		cout << CurrentTheta[i] << endl;
		cout << endl;*/
		break;
	case 1:
		cout << "unreachable" << endl;
		return 1;
		break;
	case 2:
		cout << "singular Position" << endl;
		return 2;
		break;
	}

	//13. 현재 위치일때 최종 Theta 찾기
	//CurRobJointTheta = FindFinalTheta(CurRobJointTheta, BeginRobJointTheta, MotionType, JointInterPolMode, TurnNumberValue)
	FindFinalTheta(CurRobJointTheta, prev_rob_joint_theta, MotionType, JointInterPolMode, TurnNumberValue, JointLimit, kin_type);

	return 0;
}

int COptUtil::MotionPlanner(double* ConstantVelTime, double* AccelerateTime,																	//output
	double BeginPosTcpFromBaseAux[][4], double TargetPosTcpFromBaseAux[][4], double DU1[][3], double TcpRotationValue,				//input from 데이터 정리
	double TargetRobJointTheta[],																									//input from inverse kinematics	
	int BaseAuxNum, int MountAuxNum, double MaxRobJointVel[], double MaxRobJointAcc[],												//input from device attribute
	double MaxBaseAuxVel[], double MaxBaseAuxAcc[], double MaxMountAuxVel[], double MaxMountAuxAcc[],								//input from device attribute
	double MaxTcpLinVel, double MaxTcpLinAcc, double MaxTcpAngVel, double MaxTcpAngAcc,												//input from device attribute
	double BeginRobJointTheta[], double BeginBaseAuxTheta[], double BeginMountAuxTheta[],											//input from device state
	int MotionType, double TargetBaseAuxTheta[], double TargetMountAuxTheta[], double TargetVelPercent, double TargetAccPercent)	//input from Tag Attribute
{
	switch (MotionType)
	{
	case JOINT_MOTION:
		FindJointMotionTime(ConstantVelTime, AccelerateTime,								//output
			TargetRobJointTheta,															//input from inverse kinematics
			BaseAuxNum, MountAuxNum, MaxRobJointVel, MaxRobJointAcc,						//input from device attribute
			MaxBaseAuxVel, MaxBaseAuxAcc, MaxMountAuxVel, MaxMountAuxAcc,					//input from device attribute
			BeginRobJointTheta, BeginBaseAuxTheta, BeginMountAuxTheta,						//input from device state
			TargetBaseAuxTheta, TargetMountAuxTheta, TargetVelPercent, TargetAccPercent);	//input from Tag Attribute
		break;
	case LINEAR_MOTION:
		FindLinearMotionTime(ConstantVelTime, AccelerateTime,								//output
			BeginPosTcpFromBaseAux, TargetPosTcpFromBaseAux, DU1, TcpRotationValue, 		//input from 데이터 정리
			BaseAuxNum, MountAuxNum,														//input from device attribute
			MaxBaseAuxVel, MaxBaseAuxAcc, MaxMountAuxVel, MaxMountAuxAcc,					//input from device attribute
			MaxTcpLinVel, MaxTcpLinAcc, MaxTcpAngVel, MaxTcpAngAcc,							//input from device attribute
			BeginBaseAuxTheta, BeginMountAuxTheta,											//input from device state
			TargetBaseAuxTheta, TargetMountAuxTheta, TargetVelPercent, TargetAccPercent);	//input from Tag Attribute
		break;

		if (*ConstantVelTime <= *AccelerateTime)
		{
			*AccelerateTime = sqrt(*ConstantVelTime * *AccelerateTime);
			*ConstantVelTime = 0;
		}
	}

	return 0;
}

int COptUtil::FindJointMotionTime(double* ConstantVelTime, double* AccelerateTime,											//output
	double TargetRobJointTheta[],																					//input from inverse kinematics
	int BaseAuxNum, int MountAuxNum, double MaxRobJointVel[], double MaxRobJointAcc[],								//input from device attribute
	double MaxBaseAuxVel[], double MaxBaseAuxAcc[], double MaxMountAuxVel[], double MaxMountAuxAcc[],				//input from device attribute
	double BeginRobJointTheta[], double BeginBaseAuxTheta[], double BeginMountAuxTheta[],							//input from device state
	double TargetBaseAuxTheta[], double TargetMountAuxTheta[], double TargetVelPercent, double TargetAccPercent)	//input from Tag Attribute
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CString temp_string;
	int i;
	//1. 조인트별 주어진 속도 구하기
	//TargetPosRobJointVel = MaxRobJointVel * TargetVelPercent
	double TargetPosRobJointVel[6];
	double TargetPosBaseAuxVel[MAXBASEAUXNUM];
	double TargetPosMountAuxVel[MAXMOUNTAUXNUM];

	for (i = 0; i<6; i++)
		TargetPosRobJointVel[i] = MaxRobJointVel[i] * TargetVelPercent / 100;
	for (i = 0; i<BaseAuxNum; i++)
		TargetPosBaseAuxVel[i] = MaxBaseAuxVel[i] * TargetVelPercent / 100;
	for (i = 0; i<MountAuxNum; i++)
		TargetPosMountAuxVel[i] = MaxMountAuxVel[i] * TargetVelPercent / 100;

	//2. 조인트별 등속 운동에 걸리는 시간 구하기
	//RobJointConstantVelTime = (TargetRobJointTheta – BeginRobJointTheta) / TargetPosRobJointVel
	double RobJointConstantVelTime[6];
	double BaseAuxConstantVelTime[MAXBASEAUXNUM];
	double MountAuxConstantVelTime[MAXMOUNTAUXNUM];

	for (i = 0; i<6; i++)
		RobJointConstantVelTime[i] = abs(TargetRobJointTheta[i] - BeginRobJointTheta[i]) / TargetPosRobJointVel[i];
	for (i = 0; i<BaseAuxNum; i++)
		BaseAuxConstantVelTime[i] = abs(TargetBaseAuxTheta[i] - BeginBaseAuxTheta[i]) / TargetPosBaseAuxVel[i];
	for (i = 0; i<MountAuxNum; i++)
		MountAuxConstantVelTime[i] = abs(TargetMountAuxTheta[i] - BeginMountAuxTheta[i]) / TargetPosMountAuxVel[i];

	//3. 2에서 구한 값 중 최대 값 선택
	//MaxJointConstantVelTime = MAX( RobJointConstantVelTime )
	*ConstantVelTime = 0;
	for (i = 0; i<6; i++)
	{
		//temp_string.Format(_T("RobJointConstantVelTime[%d] : %lf"), i, RobJointConstantVelTime[i]);
		//pFrame->AddOutput(O_INFO, temp_string);
		*ConstantVelTime = max(*ConstantVelTime, RobJointConstantVelTime[i]);
	}
	for (i = 0; i<BaseAuxNum; i++)
	{
		//temp_string.Format(_T("BaseAuxConstantVelTime[%d] : %lf"), i, BaseAuxConstantVelTime[i]);
		//pFrame->AddOutput(O_INFO, temp_string);
		*ConstantVelTime = max(*ConstantVelTime, BaseAuxConstantVelTime[i]);
	}
	for (i = 0; i<MountAuxNum; i++)
	{
		//temp_string.Format(_T("MountAuxConstantVelTime[%d] : %lf"), i, MountAuxConstantVelTime[i]);
		//pFrame->AddOutput(O_INFO, temp_string);
		*ConstantVelTime = max(*ConstantVelTime, MountAuxConstantVelTime[i]);
	}

	//4. 조인트별 실제 속도 구하기
	//TargetPosRobJointVel = ( TargetRobJointTheta – BeginRobJointTheta ) / MaxJointConstantVelTime
	for (i = 0; i<6; i++)
	{
		if (abs(*ConstantVelTime) == 0.001f)
		{
			TargetPosRobJointVel[i] = 0.0f;
		}
		else
		{
			TargetPosRobJointVel[i] = abs(TargetRobJointTheta[i] - BeginRobJointTheta[i]) / *ConstantVelTime;
		}
	}
	for (i = 0; i<BaseAuxNum; i++)
	{
		if (abs(*ConstantVelTime) == 0.001f)
		{
			TargetPosBaseAuxVel[i] = 0.0f;
		}
		else
		{
			TargetPosBaseAuxVel[i] = abs(TargetBaseAuxTheta[i] - BeginBaseAuxTheta[i]) / *ConstantVelTime;
		}
	}
	for (i = 0; i<MountAuxNum; i++)
	{
		if (abs(*ConstantVelTime) == 0.001f)
		{
			TargetPosMountAuxVel[i] = 0.0f;
		}
		else
		{
			TargetPosMountAuxVel[i] = abs(TargetMountAuxTheta[i] - BeginMountAuxTheta[i]) / *ConstantVelTime;
		}
	}

	//5. 조인트별 가속도 구하기
	//TargetPosRobJointAcc = MaxRobJointAcc * TargetAccPercent
	double TargetPosRobJointAcc[6];
	double TargetPosBaseAuxAcc[MAXBASEAUXNUM];
	double TargetPosMountAuxAcc[MAXMOUNTAUXNUM];

	for (i = 0; i<6; i++)
		TargetPosRobJointAcc[i] = MaxRobJointAcc[i] * TargetAccPercent / 100;
	for (i = 0; i<BaseAuxNum; i++)
		TargetPosBaseAuxAcc[i] = MaxBaseAuxAcc[i] * TargetAccPercent / 100;
	for (i = 0; i<MountAuxNum; i++)
		TargetPosMountAuxAcc[i] = MaxMountAuxAcc[i] * TargetAccPercent / 100;
	/*
	for(i=0; i<6; i++)
	cout << TargetPosJointAcc[i] << endl;
	for(i=0; i<DA->BaseAuxNum; i++)
	cout << TargetPosBaseAuxAcc[i] << endl;
	for(i=0; i<DA->MountAuxNum; i++)
	cout << TargetPosMountAuxAcc[i] << endl;
	*/
	//6. 조인트별 가속 운동에 걸리는 시간 구하기
	//RobJointAccelerateTime = TargetPosRobJointVel  / TargetPosRobJointAcc
	double RobJointAccelerateTime[6];
	double BaseAuxAccelerateTime[MAXBASEAUXNUM];
	double MountAuxAccelerateTime[MAXMOUNTAUXNUM];

	for (i = 0; i<6; i++)
		RobJointAccelerateTime[i] = TargetPosRobJointVel[i] / TargetPosRobJointAcc[i];
	for (i = 0; i<BaseAuxNum; i++)
		BaseAuxAccelerateTime[i] = TargetPosBaseAuxVel[i] / TargetPosBaseAuxAcc[i];
	for (i = 0; i<MountAuxNum; i++)
		MountAuxAccelerateTime[i] = TargetPosMountAuxVel[i] / TargetPosMountAuxAcc[i];

	/*
	for(i=0; i<6; i++)
	cout << JointAccelerateTime[i] << endl;
	for(i=0; i<DA->BaseAuxNum; i++)
	cout << BaseAuxAccelerateTime[i] << endl;
	for(i=0; i<DA->MountAuxNum; i++)
	cout << MountAuxAccelerateTime[i] << endl;
	*/
	//7. 6에서 구한 값 중 최대 값 선택
	//MaxRobJointAccelerateTime = MAX( RobJointAccelerateTime )
	*AccelerateTime = 0;
	for (i = 0; i<6; i++)
	{
		//temp_string.Format(_T("TargetPosRobJointAcc[%d] : %lf"), i, TargetPosRobJointAcc[i]);
		//pFrame->AddOutput(O_INFO, temp_string);
		*AccelerateTime = max(*AccelerateTime, RobJointAccelerateTime[i]);
	}
	for (i = 0; i<BaseAuxNum; i++)
	{
		//temp_string.Format(_T("TargetPosBaseAuxAcc[%d] : %lf"), i, TargetPosBaseAuxAcc[i]);
		//pFrame->AddOutput(O_INFO, temp_string);
		*AccelerateTime = max(*AccelerateTime, BaseAuxAccelerateTime[i]);
	}
	for (i = 0; i<MountAuxNum; i++)
	{
		//temp_string.Format(_T("TargetPosMountAuxAcc[%d] : %lf"), i, TargetPosMountAuxAcc[i]);
		//pFrame->AddOutput(O_INFO, temp_string);
		*AccelerateTime = max(*AccelerateTime, MountAuxAccelerateTime[i]);
	}

	return 0;
}


int COptUtil::FindLinearMotionTime(double* ConstantVelTime, double* AccelerateTime,													//output
	double BeginPosTcpFromBaseAux[][4], double TargetPosTcpFromBaseAux[][4], double DU1[][3], double TcpRotationValue, 		//input from 데이터 정리
	int BaseAuxNum, int MountAuxNum,																						//input from device attribute
	double MaxBaseAuxVel[], double MaxBaseAuxAcc[], double MaxMountAuxVel[], double MaxMountAuxAcc[],						//input from device attribute
	double MaxTcpLinVel, double MaxTcpLinAcc, double MaxTcpAngVel, double MaxTcpAngAcc,										//input from device attribute
	double BeginBaseAuxTheta[], double BeginMountAuxTheta[],																//input from device state
	double TargetBaseAuxTheta[], double TargetMountAuxTheta[], double TargetVelPercent, double TargetAccPercent)			//input from Tag Attribute
{
	int i, j;
	//----------<<부가축에 대한 등속 시간 계산>>----------
	//1. 조인트별 주어진 속도 구하기
	//TargetPosBaseAuxVel = MaxBaseAuxVel * TargetVelPercent
	double TargetPosBaseAuxVel[MAXBASEAUXNUM];
	double TargetPosMountAuxVel[MAXMOUNTAUXNUM];

	for (i = 0; i<BaseAuxNum; i++)
		TargetPosBaseAuxVel[i] = MaxBaseAuxVel[i] * TargetVelPercent / 100;
	for (i = 0; i<MountAuxNum; i++)
		TargetPosMountAuxVel[i] = MaxMountAuxVel[i] * TargetVelPercent / 100;

	//2. 조인트별 등속 운동에 걸리는 시간 구하기
	//BaseAuxConstantVelTime = ( TargetBaseAuxTheta – BeginBaseAuxTheta ) / TargetPosBaseAuxVel
	double BaseAuxConstantVelTime[MAXBASEAUXNUM];
	double MountAuxConstantVelTime[MAXMOUNTAUXNUM];

	for (i = 0; i<BaseAuxNum; i++)
		BaseAuxConstantVelTime[i] = abs(TargetBaseAuxTheta[i] - BeginBaseAuxTheta[i]) / TargetPosBaseAuxVel[i];
	for (i = 0; i<MountAuxNum; i++)
		MountAuxConstantVelTime[i] = abs(TargetMountAuxTheta[i] - BeginMountAuxTheta[i]) / TargetPosMountAuxVel[i];


	//----------<<TCP Position에 대한 등속 시간 계산>>----------
	//1. TCP Position 주어진 속도 구하기
	//TargetPosTcpLinVel = MaxTcpLinVel * TargetVelPercent
	double TargetPosTcpLinVel;

	TargetPosTcpLinVel = MaxTcpLinVel * TargetVelPercent / 100;

	//2. TCP Position 변위 구하기
	//DeltaTcpPosition = TargetPosTcpFromBaseAux – BeginPosTcpFromBaseAux
	double DeltaTcpPosition = 0;

	DeltaTcpPosition
		= sqrt(
		pow(TargetPosTcpFromBaseAux[0][3] - BeginPosTcpFromBaseAux[0][3], 2) +
		pow(TargetPosTcpFromBaseAux[1][3] - BeginPosTcpFromBaseAux[1][3], 2) +
		pow(TargetPosTcpFromBaseAux[2][3] - BeginPosTcpFromBaseAux[2][3], 2));

	//3. 등속 운동에 걸리는 시간 구하기
	//TcpLinConstantVelTime = DeltaTcpPosition / TargetPosTcpLinVel
	double TcpLinConstantVelTime = 0;

	TcpLinConstantVelTime = DeltaTcpPosition / TargetPosTcpLinVel;


	//----------<<TCP Orientation에 대한 등속 시간 계산>>----------
	//1. TCP Orientation 주어진 속도 구하기
	//TargetPosTcpAngVel = MaxTcpAngVel * TargetVelPercent
	double TargetPosTcpAngVel;
	TargetPosTcpAngVel = MaxTcpAngVel * TargetVelPercent / 100;

	//2. 등속 운동에 걸리는 시간 구하기
	//TcpAngConstantVelTime = TcpRotationValue / TargetPosTcpAngVel
	double TcpAngConstantVelTime = 0;

	TcpAngConstantVelTime = TcpRotationValue / TargetPosTcpAngVel;



	//----------<<부가축 Tcp Position, Tcp Orientation 에서 구한 값 중 최대 값 선택>>----------
	*ConstantVelTime = 0;
	for (i = 0; i<BaseAuxNum; i++)
		*ConstantVelTime = max(*ConstantVelTime, BaseAuxConstantVelTime[i]);
	for (i = 0; i<MountAuxNum; i++)
		*ConstantVelTime = max(*ConstantVelTime, MountAuxConstantVelTime[i]);
	*ConstantVelTime = max(*ConstantVelTime, TcpLinConstantVelTime);
	*ConstantVelTime = max(*ConstantVelTime, TcpAngConstantVelTime);


	//----------<<부가축에 대한 가속 시간 계산>>----------
	//1. 조인트별 실제 속도 구하기
	//TargetPosBaseAuxVel = ( TargetBaseAuxTheta – BeginBaseAuxTheta ) / ConstantVelTime
	for (i = 0; i<BaseAuxNum; i++)
		TargetPosBaseAuxVel[i] = abs(TargetBaseAuxTheta[i] - BeginBaseAuxTheta[i]) / *ConstantVelTime;
	for (i = 0; i<MountAuxNum; i++)
		TargetPosMountAuxVel[i] = abs(TargetMountAuxTheta[i] - BeginMountAuxTheta[i]) / *ConstantVelTime;

	//2. 조인트별 가속도 구하기
	//TargetPosBaseAuxAcc = MaxBaseAuxAcc * TargetAccPercent
	double TargetPosBaseAuxAcc[MAXBASEAUXNUM];
	double TargetPosMountAuxAcc[MAXMOUNTAUXNUM];

	for (i = 0; i<BaseAuxNum; i++)
		TargetPosBaseAuxAcc[i] = MaxBaseAuxAcc[i] * TargetAccPercent / 100;
	for (i = 0; i<MountAuxNum; i++)
		TargetPosMountAuxAcc[i] = MaxMountAuxAcc[i] * TargetAccPercent / 100;

	//3. 조인트별 가속 운동에 걸리는 시간 구하기
	//BaseAuxAccelerateTime = TargetPosBaseAuxVel  / TargetPosBaseAuxAcc
	double BaseAuxAccelerateTime[MAXBASEAUXNUM];
	double MountAuxAccelerateTime[MAXMOUNTAUXNUM];

	for (i = 0; i<BaseAuxNum; i++)
		BaseAuxAccelerateTime[i] = TargetPosBaseAuxVel[i] / TargetPosBaseAuxAcc[i];
	for (i = 0; i<MountAuxNum; i++)
		MountAuxAccelerateTime[i] = TargetPosMountAuxVel[i] / TargetPosMountAuxAcc[i];

	//----------<<Tcp Position에 대한 가속 시간 계산>>----------
	//1. Tcp Position 실제 속도 구하기
	//TargetPosTcpLinVel = DeltaTcpPosition / ConstantVelTime
	TargetPosTcpLinVel = DeltaTcpPosition / *ConstantVelTime;

	//2. Tcp Position 주어진 가속도 구하기
	//TargetPosTcpLinAcc = MaxTcpLinAcc * TargetAccPercent
	double TargetPosTcpLinAcc;
	TargetPosTcpLinAcc = MaxTcpLinAcc * TargetAccPercent / 100;

	//3. 가속 운동에 걸리는 시간 구하기
	//TcpLinAccelerateTime = TargetPosTcpLinVel / TargetPosTcpLinAcc
	double TcpLinAccelerateTime;
	TcpLinAccelerateTime = TargetPosTcpLinVel / TargetPosTcpLinAcc;

	//----------<<Tcp Orientation에 대한 가속 시간 계산>>----------
	//1. Tcp Orientation 실제 속도 구하기
	//TargetPosTcpAngVel = TcpRotationValue / ConstantVelTime
	TargetPosTcpAngVel = TcpRotationValue / *ConstantVelTime;

	//2. Tcp Orientation 주어진 가속도 구하기
	//TargetPosTcpAngAcc = MaxTcpAngAcc * TargetAccPercent
	double TargetPosTcpAngAcc;
	TargetPosTcpAngAcc = MaxTcpAngAcc * TargetAccPercent / 100;

	//3. 가속 운동에 걸리는 시간 구하기
	//TcpAngAccelerateTime = TargetPosTcpAngVel / TargetPosTcpAngAcc
	double TcpAngAccelerateTime;
	TcpAngAccelerateTime = TargetPosTcpAngVel / TargetPosTcpAngAcc;

	//----------<<부가축 Tcp Position, Tcp Orientation 에서 구한 값 중 최대 값 선택>>----------
	*AccelerateTime = 0;
	for (i = 0; i<BaseAuxNum; i++)
		*AccelerateTime = max(*AccelerateTime, BaseAuxAccelerateTime[i]);
	for (i = 0; i<MountAuxNum; i++)
		*AccelerateTime = max(*AccelerateTime, MountAuxAccelerateTime[i]);
	*AccelerateTime = max(*AccelerateTime, TcpLinAccelerateTime);
	*AccelerateTime = max(*AccelerateTime, TcpAngAccelerateTime);
	*AccelerateTime = 0;
	return 0;
}

int COptUtil::InverseKinematics(double TargetRobJointTheta[],																			// output
	double TargetTagPosMat[][4], double DevicePosMat[][4], double BaseAuxParaMat[][4][4], double BaseAuxRobPosMat[][4],		// input from 데이터 정리
	double MountMat[][4], double TcpMat[][4], int TargetConfig[], double RobJointDV[][4][4],								// input from 데이터 정리
	double aa[], double dd[], double alpha, int JointInterPolMode,															// input from Device Attribute
	int RobJointSign[], double RobJointOffset[], int BaseAuxNum, int BaseAuxType[], 										// input from Device Attribute
	double BeginRobJointTheta[], 																							// input from Device State
	double TargetBaseAuxTheta[], int MotionType, int TurnNumberValue[], double JointLimit[][6],
	double(*BaseAuxJointDV)[4][4], int kin_type)														// input from Tag Attribute
{
	//1. 목표 위치일때 Device Base에서 로봇 위치 4X4 행렬
	//TargetRobPosMatFromBaseAux = BaseAuxParaMat * Trans/Rot(TargetBaseAuxTheta) * … * BaseAuxRobPos 	---BaseAuxPara * BaseAuxRobPos
	double TargetRobPosMatFromBaseAux[4][4] = { 0 };
	BaseAuxFK(TargetRobPosMatFromBaseAux, BaseAuxParaMat, TargetBaseAuxTheta, BaseAuxNum, BaseAuxType, BaseAuxJointDV);
	MultiplyMAT(TargetRobPosMatFromBaseAux, TargetRobPosMatFromBaseAux, BaseAuxRobPosMat);

	//2. 목표 위치일때 로봇 기준 Tag 위치 4X4 행렬
	//TargetPosTcp = inv( TargetRobPosMatFromBaseAux ) * inv( DevicePosMat ) * TargetTagPosMat
	double invTargetRobPosMatFromBaseAux[4][4] = { 0 };
	InverseMAT(invTargetRobPosMatFromBaseAux, TargetRobPosMatFromBaseAux);
	double invDevicePosMat[4][4] = { 0 };
	InverseMAT(invDevicePosMat, DevicePosMat);

	double TargetPosTcp[4][4] = { 0 };
	MultiplyMAT(TargetPosTcp, invTargetRobPosMatFromBaseAux, invDevicePosMat);
	MultiplyMAT(TargetPosTcp, TargetPosTcp, TargetTagPosMat);

	//3. 목표 위치일때 로봇 0T6 구하기
	//TargetPos0T6 = TargetPosTcp * inv( TcpMat ) * inv( MountMat )
	double invTcpMat[4][4] = { 0 };
	InverseMAT(invTcpMat, TcpMat);
	double invMountMat[4][4] = { 0 };
	InverseMAT(invMountMat, MountMat);

	double TargetPos0T6[4][4] = { 0 };
	MultiplyMAT(TargetPos0T6, TargetPosTcp, invTcpMat);
	MultiplyMAT(TargetPos0T6, TargetPos0T6, invMountMat);

	//4. 목표 위치일때 Inverse Kin 풀기
	//TargetRobJointTheta = RobotIK(TargetRobJointTheta, TargetPos0T6, aa, dd, alpha, RobJointDV, TargetConfig, RobJointSign, RobJointOffset)
	int resultRobotIK;

	resultRobotIK = RobotIK(TargetRobJointTheta,
		TargetPos0T6, aa, dd, alpha, RobJointDV, TargetConfig, RobJointSign, RobJointOffset);

	switch (resultRobotIK)
	{
	case 0:
		break;
	case 1:
		return 1;
		break;
	case 2:
		return 2;
		break;
	}

	//5. 목표 위치일때 최종 Theta 찾기
	//FindFinalTheta(TargetRobJointTheta, BeginRobJointTheta, MotionType, JointInterPolMode, TurnNumberValue)

	FindFinalTheta(TargetRobJointTheta, BeginRobJointTheta, MotionType, JointInterPolMode, TurnNumberValue, JointLimit, kin_type);
	/*
	cout << "Final TargetTheta" << endl;
	for(ii=0; ii<6; ii++)
	cout << TargetTheta[ii] << endl;
	cout << endl;
	*/
	return 0;
}