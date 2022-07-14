#pragma once

class Matrix4x4
{
public:
	//中身は全てゼロ
	Matrix4x4();
	//要素決定用(引数の数字は要素番号左が行 右は列)
	Matrix4x4(float m00,float m01,float m02,float m03,float m10,float m11,float m12,float m13,float m20,float m21,float m22,float m23,float m30,float m31,float m32,float m33);
	~Matrix4x4();

	//単位行列にする
	void IdentityMatrix();

	//逆行列にする
	Matrix4x4 InverseMatrix();

	Matrix4x4 operator*(Matrix4x4& mat);

	Matrix4x4 operator*=(const Matrix4x4& mat);

private:

	float abs(float num);

public:
	float m[4][4]={};

private:

};

