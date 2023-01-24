#include "Vector3.h"
#include <cmath>

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}
Vector3::Vector3(float x, float y, float z) 
	:x(x),y(y),z(z)
{
}

Vector3::~Vector3()
{
}

float Vector3::length()const
{
	return sqrtf(x * x + y * y + z * z);
}

Vector3& Vector3::normalize(const Vector3& v)
{
	float len = v.length();
	Vector3 ans = v;
	if (len != 0)
	{
		return ans /= len;
	}

	return ans;
}

Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}

	return *this;
}

float Vector3::dot(const Vector3& v, const Vector3& v2)
{
	return v.x * v2.x + v.y * v2.y + v.z * v2.z;
}

float Vector3::dot(const Vector3& v)
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::cross(const Vector3& v, const Vector3& v2)
{
	Vector3 a = Vector3();

	a.x = v.y * v2.z - v.z * v2.y;
	a.y = v.z * v2.x - v.x * v2.z;
	a.z = v.x * v2.y - v.y * v2.x;

	return a;
}

Vector3 Vector3::cross(const Vector3& v)
{
	Vector3 a = Vector3();

	a.x = y * v.z - z * v.y;
	a.y = z * v.x - x * v.z;
	a.z = x * v.y - y * v.x;

	return a;
}

Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;

}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	if (s != 0)
	{
		x /= s;
		y /= s;
		z /= s;
	}
	return *this;
}


const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}

Vector3 nainavec3(Vector3 a, Vector3 b)
{
	Vector3 ans = {};

	ans = { a.x - b.x,a.y - b.y,a.z - b.z };

	return ans;

}

const Vector3 lerp(const Vector3& start, const Vector3& end, const float t)
{
	return start * (1.0f - t) + end * t;
}