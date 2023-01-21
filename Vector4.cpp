#include "Vector4.h"
#include <cmath>

Vector4::Vector4()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}
Vector4::Vector4(float x, float y, float z,float w)
	:x(x), y(y), z(z),w(w)
{
}

float Vector4::dot(const Vector4& v, const Vector4& v2)
{
	return v.x * v2.x + v.y * v2.y + v.z * v2.z + v.w * v2.w;
}

float Vector4::dot(const Vector4& v)
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

Vector4 Vector4::cross(const Vector4& v, const Vector4& v2)
{
	Vector4 a = Vector4();

	a.x = v.y * v2.z - v.z * v2.y;
	a.y = v.z * v2.x - v.x * v2.z;
	a.z = v.y * v2.z - v.z * v2.y;
	a.w = v.y * v2.z - v.z * v2.y;

	return a;
}

Vector4 Vector4::cross(const Vector4& v)
{
	Vector4 a = Vector4();

	a.x = y * v.z - z * v.y;
	a.y = z * v.x - x * v.z;
	a.z = y * v.z - z * v.y;
	a.w = y * v.z - z * v.y;

	return a;
}

float Vector4::length()const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

Vector4& Vector4::normalize(const Vector4& v)
{
	float len = v.length();
	Vector4 ans = v;
	if (len != 0)
	{
		return ans /= len;
	}

	return ans;
}

Vector4& Vector4::normalize()
{
	float len = length();
	
	if (len != 0)
	{
		return *this /= len;
	}

	return *this;
}

Vector4 Vector4::operator+() const
{

	return *this;

}

Vector4 Vector4::operator-() const
{

	return Vector4(-x, -y, -z, -w);

}

Vector4& Vector4::operator+=(const Vector4& v)
{

	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;

}

Vector4& Vector4::operator-=(const Vector4& v)
{

	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;

}

Vector4& Vector4::operator*=(float s)
{

	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;

}

Vector4& Vector4::operator/=(float s)
{
	if (s != 0)
	{
		x /= s;
		y /= s;
		z /= s;
		w /= s;
	}
	return *this;
}

const Vector4 operator+(const Vector4& v1, const Vector4& v2)
{
	Vector4 temp(v1);
	return temp += v2;
}

const Vector4 operator-(const Vector4& v1, const Vector4& v2)
{
	Vector4 temp(v1);
	return temp -= v2;
}

const Vector4 operator*(const Vector4& v, float s)
{
	Vector4 temp(v);
	return temp *= s;
}

const Vector4 operator*(float s, const Vector4& v)
{
	return v * s;
}

const Vector4 operator/(const Vector4& v, float s)
{
	Vector4 temp(v);
	return temp /= s;
}

