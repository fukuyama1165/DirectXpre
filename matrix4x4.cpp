#include "matrix4x4.h"

Matrix4x4::Matrix4x4()
{
}

Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;

	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;

	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;

	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;

}

Matrix4x4::~Matrix4x4()
{
}

void Matrix4x4::IdentityMatrix()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i != j)
			{
				m[i][j] = 0;
			}
			else
			{
				m[i][j] = 1;
			}
		}
	}
}

Matrix4x4 Matrix4x4::InverseMatrix()
{

	Matrix4x4 inverse = {};
	float calcMat[4][8] = {};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			calcMat[i][j] = m[i][j];
		}
	}

	calcMat[0][4] = 1;
	calcMat[1][5] = 1;
	calcMat[2][6] = 1;
	calcMat[3][7] = 1;

	double calcMatTmp[4][8];

	//‚Ps‚P—ñ‚ð‚P‚É‚·‚é

	{
		double max = abs(calcMat[0][0]);
		int	maxI = 0;

		for (int i = 1; i < 4; i++)
		{
			if (abs(calcMat[i][0]) > max)
			{
				max = abs(calcMat[i][0]);
				maxI = i;
			}
		}

		if (abs(calcMat[maxI][0]) <= 0)
		{
			Matrix4x4 a = {};
			a.IdentityMatrix();
			return a;
		}

		if (maxI != 0)
		{
			for (int j = 0; j < 8; j++) {
				double tmp = calcMat[maxI][j];
				calcMat[maxI][j] = calcMat[0][j];
				calcMat[0][j] = tmp;
			}
		}


	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			calcMatTmp[i][j] = calcMat[i][j];
		}
	}

	for (int j = 0; j < 8; j++) {
		calcMat[0][j] /= calcMatTmp[0][0];
		calcMat[1][j] += (-1) * calcMatTmp[1][0] * calcMat[0][j];
		calcMat[2][j] += (-1) * calcMatTmp[2][0] * calcMat[0][j];
		calcMat[3][j] += (-1) * calcMatTmp[3][0] * calcMat[0][j];

	}


	{
		double max = abs(calcMat[1][1]);
		int	maxI = 1;

		for (int i = 2; i < 4; i++)
		{
			if (abs(calcMat[i][1]) > max)
			{
				max = abs(calcMat[i][1]);
				maxI = i;
			}
		}

		if (abs(calcMat[maxI][1]) <= 0)
		{
			Matrix4x4 a = {};
			a.IdentityMatrix();
			return a;
		}

		if (maxI != 1)
		{
			for (int j = 0; j < 8; j++) {
				double tmp = calcMat[maxI][j];
				calcMat[maxI][j] = calcMat[1][j];
				calcMat[1][j] = tmp;
			}
		}


	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			calcMatTmp[i][j] = calcMat[i][j];
		}
	}

	for (int j = 0; j < 8; j++) {
		calcMat[1][j] /= calcMatTmp[1][1];
		calcMat[0][j] += (-1) * calcMatTmp[0][1] * calcMat[1][j];
		calcMat[2][j] += (-1) * calcMatTmp[2][1] * calcMat[1][j];
		calcMat[3][j] += (-1) * calcMatTmp[3][1] * calcMat[1][j];

	}

	{
		double max = abs(calcMat[2][2]);
		int	maxI = 2;

		for (int i = 3; i < 4; i++)
		{
			if (abs(calcMat[i][2]) > max)
			{
				max = abs(calcMat[i][2]);
				maxI = i;
			}
		}

		if (abs(calcMat[maxI][2]) <= 0)
		{
			Matrix4x4 a = {};
			a.IdentityMatrix();
			return a;
		}

		if (maxI != 2)
		{
			for (int j = 0; j < 8; j++) {
				double tmp = calcMat[maxI][j];
				calcMat[maxI][j] = calcMat[2][j];
				calcMat[2][j] = tmp;
			}
		}


	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			calcMatTmp[i][j] = calcMat[i][j];
		}
	}



	for (int j = 0; j < 8; j++) {
		calcMat[2][j] /= calcMatTmp[2][2];
		calcMat[0][j] += (-1) * calcMatTmp[0][2] * calcMat[2][j];
		calcMat[1][j] += (-1) * calcMatTmp[1][2] * calcMat[2][j];
		calcMat[3][j] += (-1) * calcMatTmp[3][2] * calcMat[2][j];
	}


	{


		if (abs(calcMat[3][3]) <= 0)
		{
			Matrix4x4 a = {};
			a.IdentityMatrix();
			return a;
		}


	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			calcMatTmp[i][j] = calcMat[i][j];
		}
	}


	for (int j = 0; j < 8; j++) {
		calcMat[3][j] /= calcMatTmp[3][3];
		calcMat[0][j] += (-1) * calcMatTmp[0][3] * calcMat[3][j];
		calcMat[1][j] += (-1) * calcMatTmp[1][3] * calcMat[3][j];
		calcMat[2][j] += (-1) * calcMatTmp[2][3] * calcMat[3][j];

	}



	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			inverse.m[i][j] = calcMat[i][j + 4];
		}
	}

	return inverse;
}


Matrix4x4 Matrix4x4::operator*(Matrix4x4& mat)
{
	Matrix4x4 ansMat = Matrix4x4();

	ansMat.m[0][0] = (m[0][0] * mat.m[0][0]) + (m[0][1] * mat.m[1][0]) + (m[0][2] * mat.m[2][0]) + (m[0][3] * mat.m[3][0]);
	ansMat.m[0][1] = (m[0][0] * mat.m[0][1]) + (m[0][1] * mat.m[1][1]) + (m[0][2] * mat.m[2][1]) + (m[0][3] * mat.m[3][1]);
	ansMat.m[0][2] = (m[0][0] * mat.m[0][2]) + (m[0][1] * mat.m[1][2]) + (m[0][2] * mat.m[2][2]) + (m[0][3] * mat.m[3][2]);
	ansMat.m[0][3] = (m[0][0] * mat.m[0][3]) + (m[0][1] * mat.m[1][3]) + (m[0][2] * mat.m[2][3]) + (m[0][3] * mat.m[3][3]);

	ansMat.m[1][0] = (m[1][0] * mat.m[0][0]) + (m[1][1] * mat.m[1][0]) + (m[1][2] * mat.m[2][0]) + (m[1][3] * mat.m[3][0]);
	ansMat.m[1][1] = (m[1][0] * mat.m[0][1]) + (m[1][1] * mat.m[1][1]) + (m[1][2] * mat.m[2][1]) + (m[1][3] * mat.m[3][1]);
	ansMat.m[1][2] = (m[1][0] * mat.m[0][2]) + (m[1][1] * mat.m[1][2]) + (m[1][2] * mat.m[2][2]) + (m[1][3] * mat.m[3][2]);
	ansMat.m[1][3] = (m[1][0] * mat.m[0][3]) + (m[1][1] * mat.m[1][3]) + (m[1][2] * mat.m[2][3]) + (m[1][3] * mat.m[3][3]);

	ansMat.m[2][0] = (m[2][0] * mat.m[0][0]) + (m[2][1] * mat.m[1][0]) + (m[2][2] * mat.m[2][0]) + (m[2][3] * mat.m[3][0]);
	ansMat.m[2][1] = (m[2][0] * mat.m[0][1]) + (m[2][1] * mat.m[1][1]) + (m[2][2] * mat.m[2][1]) + (m[2][3] * mat.m[3][1]);
	ansMat.m[2][2] = (m[2][0] * mat.m[0][2]) + (m[2][1] * mat.m[1][2]) + (m[2][2] * mat.m[2][2]) + (m[2][3] * mat.m[3][2]);
	ansMat.m[2][3] = (m[2][0] * mat.m[0][3]) + (m[2][1] * mat.m[1][3]) + (m[2][2] * mat.m[2][3]) + (m[2][3] * mat.m[3][3]);

	ansMat.m[3][0] = (m[3][0] * mat.m[0][0]) + (m[3][1] * mat.m[1][0]) + (m[3][2] * mat.m[2][0]) + (m[3][3] * mat.m[3][0]);
	ansMat.m[3][1] = (m[3][0] * mat.m[0][1]) + (m[3][1] * mat.m[1][1]) + (m[3][2] * mat.m[2][1]) + (m[3][3] * mat.m[3][1]);
	ansMat.m[3][2] = (m[3][0] * mat.m[0][2]) + (m[3][1] * mat.m[1][2]) + (m[3][2] * mat.m[2][2]) + (m[3][3] * mat.m[3][2]);
	ansMat.m[3][3] = (m[3][0] * mat.m[0][3]) + (m[3][1] * mat.m[1][3]) + (m[3][2] * mat.m[2][3]) + (m[3][3] * mat.m[3][3]);

	return ansMat;
}

Matrix4x4 Matrix4x4::operator*=(const Matrix4x4& mat)
{
	Matrix4x4 ansMat = Matrix4x4();

	ansMat.m[0][0] = (m[0][0] * mat.m[0][0]) + (m[0][1] * mat.m[1][0]) + (m[0][2] * mat.m[2][0]) + (m[0][3] * mat.m[3][0]);
	ansMat.m[0][1] = (m[0][0] * mat.m[0][1]) + (m[0][1] * mat.m[1][1]) + (m[0][2] * mat.m[2][1]) + (m[0][3] * mat.m[3][1]);
	ansMat.m[0][2] = (m[0][0] * mat.m[0][2]) + (m[0][1] * mat.m[1][2]) + (m[0][2] * mat.m[2][2]) + (m[0][3] * mat.m[3][2]);
	ansMat.m[0][3] = (m[0][0] * mat.m[0][3]) + (m[0][1] * mat.m[1][3]) + (m[0][2] * mat.m[2][3]) + (m[0][3] * mat.m[3][3]);

	ansMat.m[1][0] = (m[1][0] * mat.m[0][0]) + (m[1][1] * mat.m[1][0]) + (m[1][2] * mat.m[2][0]) + (m[1][3] * mat.m[3][0]);
	ansMat.m[1][1] = (m[1][0] * mat.m[0][1]) + (m[1][1] * mat.m[1][1]) + (m[1][2] * mat.m[2][1]) + (m[1][3] * mat.m[3][1]);
	ansMat.m[1][2] = (m[1][0] * mat.m[0][2]) + (m[1][1] * mat.m[1][2]) + (m[1][2] * mat.m[2][2]) + (m[1][3] * mat.m[3][2]);
	ansMat.m[1][3] = (m[1][0] * mat.m[0][3]) + (m[1][1] * mat.m[1][3]) + (m[1][2] * mat.m[2][3]) + (m[1][3] * mat.m[3][3]);

	ansMat.m[2][0] = (m[2][0] * mat.m[0][0]) + (m[2][1] * mat.m[1][0]) + (m[2][2] * mat.m[2][0]) + (m[2][3] * mat.m[3][0]);
	ansMat.m[2][1] = (m[2][0] * mat.m[0][1]) + (m[2][1] * mat.m[1][1]) + (m[2][2] * mat.m[2][1]) + (m[2][3] * mat.m[3][1]);
	ansMat.m[2][2] = (m[2][0] * mat.m[0][2]) + (m[2][1] * mat.m[1][2]) + (m[2][2] * mat.m[2][2]) + (m[2][3] * mat.m[3][2]);
	ansMat.m[2][3] = (m[2][0] * mat.m[0][3]) + (m[2][1] * mat.m[1][3]) + (m[2][2] * mat.m[2][3]) + (m[2][3] * mat.m[3][3]);

	ansMat.m[3][0] = (m[3][0] * mat.m[0][0]) + (m[3][1] * mat.m[1][0]) + (m[3][2] * mat.m[2][0]) + (m[3][3] * mat.m[3][0]);
	ansMat.m[3][1] = (m[3][0] * mat.m[0][1]) + (m[3][1] * mat.m[1][1]) + (m[3][2] * mat.m[2][1]) + (m[3][3] * mat.m[3][1]);
	ansMat.m[3][2] = (m[3][0] * mat.m[0][2]) + (m[3][1] * mat.m[1][2]) + (m[3][2] * mat.m[2][2]) + (m[3][3] * mat.m[3][2]);
	ansMat.m[3][3] = (m[3][0] * mat.m[0][3]) + (m[3][1] * mat.m[1][3]) + (m[3][2] * mat.m[2][3]) + (m[3][3] * mat.m[3][3]);

	*this = ansMat;

	return ansMat;

}

float Matrix4x4::abs(float num)
{
	if (num < 0)
	{
		num = -num;
	}

	return num;
}