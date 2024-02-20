#pragma once

/**
 * @file Matrix4x4.h
 * @brief 行列クラス
 * @author フクヤマ
 * @date 2023_12/29
 */

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

	//単位行列を返す
	static Matrix4x4 Identity();

	//逆行列にする
	Matrix4x4 InverseMatrix();

	//引数を逆行列にして返す
	static Matrix4x4 Inverse(const Matrix4x4& mat);

	//転置行列を返す
	Matrix4x4 TransposeMatrix();

	Matrix4x4 operator*(const Matrix4x4& mat);

	Matrix4x4 operator*=(const Matrix4x4& mat);

	

	static Vector3 VectorMatDivW(const Matrix4x4& mat,const Vector3& pos);

private:

	float abs(float num);

public:
	float m[4][4] = {};

private:

};

Vector3 VectorMat(const Vector3& vector,const Matrix4x4& mat);

Vector3 VectorMat(const Matrix4x4& mat,const Vector3& vector);

Vector4 operator*(const Matrix4x4& mat,const Vector4& vector);
Vector4 operator*(const Vector4& vector,const Matrix4x4& mat);

Vector3 PosMat(const Vector3& vector,const Matrix4x4& mat);

Vector3 PosMat(const Matrix4x4 mat,const Vector3& vector);

Matrix4x4 matScaleGeneration(const Vector3& scale);

Matrix4x4 matRotateXGeneration(float rotateX);
Matrix4x4 matRotateYGeneration(float rotateY);
Matrix4x4 matRotateZGeneration(float rotateZ);

Matrix4x4 matRotateGeneration(const Vector3& rotate);

Matrix4x4 matMoveGeneration(const Vector3& translation);

Matrix4x4 QuaternionMatRotateGeneration(const Vector3& rotate);

//マトリックスの中身を二次元配列から一次元配列に変更(imguizmo用)
float* ChengeMatrix(Matrix4x4 mat);
//マトリックスの中身を一次元配列から二次元配列に変更(imguizmo用)
Matrix4x4 ChengeTwoDimensionalMatrix(float mat[16]);