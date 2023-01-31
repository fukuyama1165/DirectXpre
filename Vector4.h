#pragma once
class Vector4
{
public:
	Vector4();
	Vector4(float x, float y, float z,float w);

	float length()const;
	//�n�������̂𐳋K�����ĕԂ�
	static Vector4 normalize(const Vector4& v);
	//�����𐳋K������(��������������)
	Vector4& normalize();

	//�n�������̂œ��ς��v�Z���ĕԂ�
	static float dot(const Vector4& v, const Vector4& v2);
	//�����Ɠn�������̂œ��ς��v�Z���ĕԂ�(������)
	float dot(const Vector4& v);
	//�n�������̂ŊO�ς��v�Z���ĕԂ�
	static Vector4 cross(const Vector4& v, const Vector4& v2);
	//�����Ɠn�������̂ŊO�ς��v�Z���ĕԂ�(������)
	Vector4 cross(const Vector4& v);

	

	Vector4 operator+() const;
	Vector4 operator-() const;

	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float s);
	Vector4& operator/=(float s);

public:
	float x;
	float y;
	float z;
	float w;


};



const Vector4 operator+(const Vector4& v1, const Vector4& v2);
const Vector4 operator-(const Vector4& v1, const Vector4& v2);
const Vector4 operator*(const Vector4& v, float s);
const Vector4 operator*(float s, const Vector4& v);
const Vector4 operator/(const Vector4& v, float s);