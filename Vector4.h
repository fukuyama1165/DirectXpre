#pragma once
class Vector4
{
public:
	Vector4();
	Vector4(float x, float y, float z,float w);

	float length()const;
	static Vector4& normalize(const Vector4& v);

	static float dot(const Vector4& v, const Vector4& v2);
	static Vector4 cross(const Vector4& v, const Vector4& v2);

	

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