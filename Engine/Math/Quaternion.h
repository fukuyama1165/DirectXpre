#pragma once
#include "Vector3.h"
#include "matrix4x4.h"
class Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

public:
	Quaternion();
	Quaternion(float x, float y, float z,float w);
	Quaternion(Vector3 axis,float angle);
	~Quaternion();

	//Quaternion�̐�
	static Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);

	//�P��Quaternion��Ԃ�
	static Quaternion IdentityQuaternion();

	//����Quaternion��Ԃ�
	static Quaternion Conjugate(const Quaternion& quaternion);

	//Quaternion��norm��Ԃ�
	static float Norm(const Quaternion& quaternion);

	//���K������Quaternion��Ԃ�
	static Quaternion Normalize(const Quaternion& quaternion);

	//�tQuaternion��Ԃ�
	static Quaternion Inverse(const Quaternion& quaternion);

	//�C�ӎ���]��\��Quaternion�̐���
	static Quaternion MakeAxisAngle(const Vector3& axis, float angle);

	//�x�N�g����Quaternion�ŉ�]���������ʂ̃x�N�g�������߂�
	static Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

	//Quaternion�����]�s������߂�
	static Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

	//����
	static float dot(const Quaternion& q0, const Quaternion& q1);

	//���ʐ��`���
	static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

	//u���炖�ւ�Quaternion�𐶐�
	static Quaternion DirectionToDirection(const Vector3& u, const Vector3& v);


	Quaternion operator+() const;
	Quaternion operator-() const;

	Quaternion& operator+=(const Quaternion& q);
	Quaternion& operator-=(const Quaternion& q);
	Quaternion& operator*=(float s);
	Quaternion& operator*=(const Quaternion& q);
	Quaternion& operator/=(float s);

};

const Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
const Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
const Quaternion operator*(const Quaternion& q, float s);
const Quaternion operator*(float s, const Quaternion& v);
const Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
const Quaternion operator/(const Quaternion& q, float s);