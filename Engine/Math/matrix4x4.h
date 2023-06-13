#pragma once

#include "Vector3.h"
#include "Vector4.h"

class Matrix4x4
{
public:
	//中身は全てゼロ
	Matrix4x4();
	//要素決定用(引数の数字は要素番号左が行 右は列)
	Matrix4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);
	~Matrix4x4();

	//単位行列にする
	void IdentityMatrix();

	//逆行列にする
	Matrix4x4 InverseMatrix();

	Matrix4x4 operator*(const Matrix4x4& mat);

	Matrix4x4 operator*=(const Matrix4x4& mat);

	

	Vector3 VectorMatDivW(Matrix4x4 mat, Vector3 pos);

private:

	float abs(float num);

public:
	float m[4][4] = {};

private:

};

Vector3 VectorMat(Vector3 vector, Matrix4x4 mat);

Vector3 VectorMat(Matrix4x4 mat, Vector3 vector);

Vector4 operator*(Matrix4x4 mat, Vector4 vector);
Vector4 operator*(Vector4 vector,Matrix4x4 mat);

Vector3 PosMat(Vector3 vector, Matrix4x4 mat);

Vector3 PosMat(Matrix4x4 mat, Vector3 vector);