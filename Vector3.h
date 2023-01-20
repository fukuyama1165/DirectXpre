#pragma once

class Vector3
{
public:
	Vector3();
	Vector3(float x,float y,float z);
	~Vector3();

	float length()const;
	//�n�������̂𐳋K�����ĕԂ�
	static Vector3& normalize(const Vector3& v);
	//�����𐳋K������(��������������)
	Vector3& normalize();
	//�n�������̂œ��ς��v�Z���ĕԂ�
	static float dot(const Vector3& v, const Vector3& v2);
	//�����Ɠn�������̂œ��ς��v�Z���ĕԂ�(������)
	float dot(const Vector3& v);
	//�n�������̂ŊO�ς��v�Z���ĕԂ�
	static Vector3 cross(const Vector3& v, const Vector3& v2);
	//�����Ɠn�������̂ŊO�ς��v�Z���ĕԂ�(������)
	Vector3 cross(const Vector3& v);

	Vector3 operator+() const;
	Vector3 operator-() const;

	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);

public:
	float x;
	float y;
	float z;
private:

};

const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);

Vector3 nainavec3(Vector3 a, Vector3 b);

//��Ԋ֐�
//���`���(1���֐����)
const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);