#include "matrix4x4.h"
#include <cstdint>
#include <math.h>
#include "Quaternion.h"

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

Matrix4x4 Matrix4x4::Identity()
{
	Matrix4x4 ans;

	ans.IdentityMatrix();

	return ans;
}

void Matrix4x4::IdentityMatrix()
{
	for (uint16_t i = 0; i < 4; i++)
	{
		for (uint16_t j = 0; j < 4; j++)
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

	for (uint16_t i = 0; i < 4; i++)
	{
		for (uint16_t j = 0; j < 4; j++)
		{
			calcMat[i][j] = m[i][j];
		}
	}

	calcMat[0][4] = 1;
	calcMat[1][5] = 1;
	calcMat[2][6] = 1;
	calcMat[3][7] = 1;

	float a;

	//一時行列にコピー
	for (int32_t i = 0; i < 4; i++) {
		for (int32_t j = 0; j < 4; j++) {
			calcMat[i][j] = m[i][j];

			if (i == j)calcMat[i][4 + j] = 1;
		}
	}

	for (int32_t k = 0; k < 4; k++) {
		a = 1 / calcMat[k][k];

		for (int32_t j = 0; j < 8; j++) {
			calcMat[k][j] *= a;
		}

		for (int32_t i = 0; i < 4; i++) {
			if (i == k) {
				continue;
			}

			a = -calcMat[i][k];

			for (int32_t j = 0; j < 8; j++) {
				calcMat[i][j] += calcMat[k][j] * a;
			}
		}
	}

	for (int32_t i = 0; i < 4; i++) {
		for (int32_t j = 0; j < 4; j++) {
			inverse.m[i][j] = calcMat[i][4 + j];
		}
	}
	return inverse;

	//これ使った時挙動がおかしかったので上の方法に変更した(問題を調べておきたい)
	/*{
		float max = abs(calcMat[0][0]);
		uint32_t maxI = 0;

		for (uint16_t i = 1; i < 4; i++)
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
			for (uint16_t j = 0; j < 8; j++) {
				float tmp = calcMat[maxI][j];
				calcMat[maxI][j] = calcMat[0][j];
				calcMat[0][j] = tmp;
			}
		}


	}

	for (uint16_t i = 0; i < 4; i++) {
		for (uint16_t j = 0; j < 8; j++) {
			calcMatTmp[i][j] = calcMat[i][j];
		}
	}

	for (uint16_t j = 0; j < 8; j++) {
		calcMat[0][j] /= calcMatTmp[0][0];
		calcMat[1][j] += (-1) * calcMatTmp[1][0] * calcMat[0][j];
		calcMat[2][j] += (-1) * calcMatTmp[2][0] * calcMat[0][j];
		calcMat[3][j] += (-1) * calcMatTmp[3][0] * calcMat[0][j];

	}


	{
		double max = abs(calcMat[1][1]);
		uint32_t maxI = 1;

		for (uint16_t i = 2; i < 4; i++)
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
			for (uint16_t j = 0; j < 8; j++) {
				float tmp = calcMat[maxI][j];
				calcMat[maxI][j] = calcMat[1][j];
				calcMat[1][j] = tmp;
			}
		}


	}

	for (uint16_t i = 0; i < 4; i++) {
		for (uint16_t j = 0; j < 8; j++) {
			calcMatTmp[i][j] = calcMat[i][j];
		}
	}

	for (uint16_t j = 0; j < 8; j++) {
		calcMat[1][j] /= calcMatTmp[1][1];
		calcMat[0][j] += (-1) * calcMatTmp[0][1] * calcMat[1][j];
		calcMat[2][j] += (-1) * calcMatTmp[2][1] * calcMat[1][j];
		calcMat[3][j] += (-1) * calcMatTmp[3][1] * calcMat[1][j];

	}

	{
		double max = abs(calcMat[2][2]);
		uint32_t	maxI = 2;

		for (uint16_t i = 3; i < 4; i++)
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
			for (uint16_t j = 0; j < 8; j++) {
				float tmp = calcMat[maxI][j];
				calcMat[maxI][j] = calcMat[2][j];
				calcMat[2][j] = tmp;
			}
		}


	}

	for (uint16_t i = 0; i < 4; i++) {
		for (uint16_t j = 0; j < 8; j++) {
			calcMatTmp[i][j] = calcMat[i][j];
		}
	}



	for (uint16_t j = 0; j < 8; j++) {
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

	for (uint16_t i = 0; i < 4; i++) {
		for (uint16_t j = 0; j < 8; j++) {
			calcMatTmp[i][j] = calcMat[i][j];
		}
	}


	for (uint16_t j = 0; j < 8; j++) {
		calcMat[3][j] /= calcMatTmp[3][3];
		calcMat[0][j] += (-1) * calcMatTmp[0][3] * calcMat[3][j];
		calcMat[1][j] += (-1) * calcMatTmp[1][3] * calcMat[3][j];
		calcMat[2][j] += (-1) * calcMatTmp[2][3] * calcMat[3][j];

	}



	for (uint16_t i = 0; i < 4; i++) {
		for (uint16_t j = 0; j < 4; j++) {
			inverse.m[i][j] = calcMat[i][j + 4];
		}
	}


	return inverse;*/
}


Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& mat)
{
	Matrix4x4 ans = mat;

	ans=ans.InverseMatrix();

	return ans;
}

Matrix4x4 Matrix4x4::TransposeMatrix()
{

	Matrix4x4 ansMat = {};

	ansMat.m[0][0] = m[0][0];
	ansMat.m[0][1] = m[1][0];
	ansMat.m[0][2] = m[2][0];
	ansMat.m[0][3] = m[3][0];

	ansMat.m[1][0] = m[0][1];
	ansMat.m[1][1] = m[1][1];
	ansMat.m[1][2] = m[2][1];
	ansMat.m[1][3] = m[3][1];

	ansMat.m[2][0] = m[0][2];
	ansMat.m[2][1] = m[1][2];
	ansMat.m[2][2] = m[2][2];
	ansMat.m[2][3] = m[3][2];

	ansMat.m[3][0] = m[0][3];
	ansMat.m[3][1] = m[1][3];
	ansMat.m[3][2] = m[2][3];
	ansMat.m[3][3] = m[3][3];

	return ansMat;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& mat)
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

Vector3 VectorMat(const Vector3& vector, const Matrix4x4& mat)
{
	Vector3 changeVector = { 0,0,0 };

	changeVector.x = vector.x * mat.m[0][0] + vector.y * mat.m[1][0] + vector.z * mat.m[2][0] + 0.0f * mat.m[3][0];
	changeVector.y = vector.x * mat.m[0][1] + vector.y * mat.m[1][1] + vector.z * mat.m[2][1] + 0.0f * mat.m[3][1];
	changeVector.z = vector.x * mat.m[0][2] + vector.y * mat.m[1][2] + vector.z * mat.m[2][2] + 0.0f * mat.m[3][2];

	return changeVector;
}

Vector3 VectorMat(const Matrix4x4& mat, const Vector3& vector)
{
	Vector3 changeVector = { 0,0,0 };

	changeVector.x = mat.m[0][0] * vector.x + mat.m[0][1] * vector.y + mat.m[0][2] * vector.z + mat.m[0][3] * 0.0f;
	changeVector.y = mat.m[1][0] * vector.x + mat.m[1][1] * vector.y + mat.m[1][2] * vector.z + mat.m[1][3] * 0.0f;
	changeVector.z = mat.m[2][0] * vector.x + mat.m[2][1] * vector.y + mat.m[2][2] * vector.z + mat.m[2][3] * 0.0f;

	return changeVector;
}

Vector4 operator*(const Matrix4x4& mat, const Vector4& vector)
{
	Vector4 changeVector = { 0,0,0,0 };

	changeVector.x = mat.m[0][0] * vector.x + mat.m[0][1] * vector.y + mat.m[0][2] * vector.z + mat.m[0][3] * vector.w;
	changeVector.y = mat.m[1][0] * vector.x + mat.m[1][1] * vector.y + mat.m[1][2] * vector.z + mat.m[1][3] * vector.w;
	changeVector.z = mat.m[2][0] * vector.x + mat.m[2][1] * vector.y + mat.m[2][2] * vector.z + mat.m[2][3] * vector.w;

	return changeVector;
}

Vector4 operator*(const Vector4& vector, const Matrix4x4& mat)
{
	Vector4 changeVector = { 0,0,0,0};

	changeVector.x = vector.x * mat.m[0][0] + vector.y * mat.m[1][0] + vector.z * mat.m[2][0] + vector.w * mat.m[3][0];
	changeVector.y = vector.x * mat.m[0][1] + vector.y * mat.m[1][1] + vector.z * mat.m[2][1] + vector.w * mat.m[3][1];
	changeVector.z = vector.x * mat.m[0][2] + vector.y * mat.m[1][2] + vector.z * mat.m[2][2] + vector.w * mat.m[3][2];

	return changeVector;
}

Vector3 PosMat(const Vector3& vector, const Matrix4x4& mat)
{

	Vector3 changeVector = { 0,0,0 };

	changeVector.x = vector.x * mat.m[0][0] + vector.y * mat.m[1][0] + vector.z * mat.m[2][0] + 1.0f * mat.m[3][0];
	changeVector.y = vector.x * mat.m[0][1] + vector.y * mat.m[1][1] + vector.z * mat.m[2][1] + 1.0f * mat.m[3][1];
	changeVector.z = vector.x * mat.m[0][2] + vector.y * mat.m[1][2] + vector.z * mat.m[2][2] + 1.0f * mat.m[3][2];

	return changeVector;
}

Vector3 PosMat(const Matrix4x4& mat, const Vector3& vector)
{
	Vector3 changeVector = { 0,0,0 };

	changeVector.x = mat.m[0][0] * vector.x + mat.m[0][1] * vector.y + mat.m[0][2] * vector.z + mat.m[0][3] * 1.0f;
	changeVector.y = mat.m[1][0] * vector.x + mat.m[1][1] * vector.y + mat.m[1][2] * vector.z + mat.m[1][3] * 1.0f;
	changeVector.z = mat.m[2][0] * vector.x + mat.m[2][1] * vector.y + mat.m[2][2] * vector.z + mat.m[2][3] * 1.0f;

	return changeVector;
}

Vector3 Matrix4x4::VectorMatDivW(const Matrix4x4& mat, const Vector3& pos)
{
	float w = pos.x * mat.m[0][3] + pos.y * mat.m[1][3] + pos.z * mat.m[2][3] + mat.m[3][3];

	if (w == 0)
	{
		w = 1;
	}
	Vector3 result =
	{
		(pos.x * mat.m[0][0] + pos.y * mat.m[1][0] + pos.z * mat.m[2][0] + mat.m[3][0]) / w,
		(pos.x * mat.m[0][1] + pos.y * mat.m[1][1] + pos.z * mat.m[2][1] + mat.m[3][1]) / w,
		(pos.x * mat.m[0][2] + pos.y * mat.m[1][2] + pos.z * mat.m[2][2] + mat.m[3][2]) / w
	};

	return result;
}

Matrix4x4 matScaleGeneration(const Vector3& scale)
{
	//スケーリング行列を宣言
	Matrix4x4 matScale;
	matScale.IdentityMatrix();

	//スケーリング倍率を行列に設定
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4x4 matRotateXGeneration(float rotateX)
{
	//X軸回転行列を宣言
	Matrix4x4 matRotateX;
	matRotateX.IdentityMatrix();

	//回転角を行列に設定(ラジアン)
	matRotateX.m[0][0] = 1;
	matRotateX.m[1][1] = cosf(rotateX);
	matRotateX.m[1][2] = sinf(rotateX);
	matRotateX.m[2][1] = -sinf(rotateX);
	matRotateX.m[2][2] = cosf(rotateX);
	matRotateX.m[3][3] = 1;

	return matRotateX;
}

Matrix4x4 matRotateYGeneration(float rotateY)
{
	//Y軸回転行列を宣言
	Matrix4x4 matRotateY;
	matRotateY.IdentityMatrix();

	//回転角を行列に設定(ラジアン)
	matRotateY.m[0][0] = cosf(rotateY);
	matRotateY.m[0][2] = -sinf(rotateY);
	matRotateY.m[1][1] = 1;
	matRotateY.m[2][0] = sinf(rotateY);
	matRotateY.m[2][2] = cosf(rotateY);
	matRotateY.m[3][3] = 1;

	return matRotateY;
}

Matrix4x4 matRotateZGeneration(float rotateZ)
{
	//Z軸回転行列を宣言
	Matrix4x4 matRotateZ;
	matRotateZ.IdentityMatrix();

	//回転角を行列に設定(ラジアン)
	matRotateZ.m[0][0] = cosf(rotateZ);
	matRotateZ.m[0][1] = sinf(rotateZ);
	matRotateZ.m[1][0] = -sinf(rotateZ);
	matRotateZ.m[1][1] = cosf(rotateZ);
	matRotateZ.m[2][2] = 1;
	matRotateZ.m[3][3] = 1;

	return matRotateZ;
}

Matrix4x4 matRotateGeneration(const Vector3& rotate)
{
	//X軸回転行列を宣言
	Matrix4x4 matRotateX = matRotateXGeneration(rotate.x);

	//Y軸回転行列を宣言
	Matrix4x4 matRotateY = matRotateYGeneration(rotate.y);

	//Z軸回転行列を宣言
	Matrix4x4 matRotateZ = matRotateZGeneration(rotate.z);

	//回転軸合成行列を宣言
	Matrix4x4 matRotate;
	matRotate.IdentityMatrix();

	//計算した角度を計算(順番は回転させるモデルによって変える)
	matRotate = matRotateZ * matRotateX * matRotateY;

	return matRotate;



}

Matrix4x4 matMoveGeneration(const Vector3& translation)
{
	//移動するための行列を用意
	Matrix4x4 matMove;
	matMove.IdentityMatrix();

	//行列に移動量を代入
	matMove.m[3][0] = translation.x;
	matMove.m[3][1] = translation.y;
	matMove.m[3][2] = translation.z;

	return matMove;
}

Matrix4x4 QuaternionMatRotateGeneration(const Vector3& rotate)
{
	Quaternion ans;

	Quaternion rotaX = Quaternion::MakeAxisAngle({ 1.0f,0.0f,0.0f }, rotate.x);
	Quaternion rotaY = Quaternion::MakeAxisAngle({ 0.0f,1.0f,0.0f }, rotate.y);
	Quaternion rotaZ = Quaternion::MakeAxisAngle({ 0.0f,0.0f,1.0f }, rotate.z);

	ans = rotaX * rotaY * rotaZ;


	return Quaternion::MakeRotateMatrix(Quaternion::Normalize(ans));

}

float* ChengeMatrix(Matrix4x4 mat)
{
	float buff[16] = {};
	buff[0] = mat.m[0][0];
	buff[1] = mat.m[0][1];
	buff[2] = mat.m[0][2];
	buff[3] = mat.m[0][3];

	buff[4] = mat.m[1][0];
	buff[5] = mat.m[1][1];
	buff[6] = mat.m[1][2];
	buff[7] = mat.m[1][3];

	buff[8] = mat.m[2][0];
	buff[9] = mat.m[2][1];
	buff[10] = mat.m[2][2];
	buff[11] = mat.m[2][3];

	buff[12] = mat.m[3][0];
	buff[13] = mat.m[3][1];
	buff[14] = mat.m[3][2];
	buff[15] = mat.m[3][3];

	return buff;
}

Matrix4x4 ChengeTwoDimensionalMatrix(float mat[16])
{
	Matrix4x4 buff = {};
	buff.m[0][0] = mat[0];
	buff.m[0][1] = mat[1];
	buff.m[0][2] = mat[2];
	buff.m[0][3] = mat[3];

	buff.m[1][0] = mat[4];
	buff.m[1][1] = mat[5];
	buff.m[1][2] = mat[6];
	buff.m[1][3] = mat[7];

	buff.m[2][0] = mat[8];
	buff.m[2][1] = mat[9];
	buff.m[2][2] = mat[10];
	buff.m[2][3] = mat[11];

	buff.m[3][0] = mat[12];
	buff.m[3][1] = mat[13];
	buff.m[3][2] = mat[14];
	buff.m[3][3] = mat[15];

	return buff;
}