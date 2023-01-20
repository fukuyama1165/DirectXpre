#pragma once

class Vector3
{
public:
	Vector3();
	Vector3(float x,float y,float z);
	~Vector3();

	float length()const;
	static Vector3& normalize(const Vector3& v);
	Vector3& normalize();
	static float dot(const Vector3& v, const Vector3& v2);
	float dot(const Vector3& v);
	static Vector3 cross(const Vector3& v, const Vector3& v2);
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

//補間関数
//線形補間(1次関数補間)
const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);