#pragma once

class Matrix4x4
{
public:
	//���g�͑S�ă[��
	Matrix4x4();
	//�v�f����p(�����̐����͗v�f�ԍ������s �E�͗�)
	Matrix4x4(float m00,float m01,float m02,float m03,float m10,float m11,float m12,float m13,float m20,float m21,float m22,float m23,float m30,float m31,float m32,float m33);
	~Matrix4x4();

	//�P�ʍs��ɂ���
	void IdentityMatrix();

	//�t�s��ɂ���
	Matrix4x4 InverseMatrix();

	Matrix4x4 operator*(Matrix4x4& mat);

	Matrix4x4 operator*=(const Matrix4x4& mat);

private:

	float abs(float num);

public:
	float m[4][4]={};

private:

};

