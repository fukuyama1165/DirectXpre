#include "Float3.h"
#include <cmath>

Float3::Float3()
{
	x = 0;
	y = 0;
	z = 0;
}
Float3::Float3(float x, float y, float z) 
	:x(x),y(y),z(z)
{
}

Float3::~Float3()
{
}

float Float3::length()const
{
	return sqrtf(x * x + y * y + z * z);
}

Float3& Float3::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}

	return *this;
}

float Float3::dot(const Float3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Float3 Float3::cross(const Float3& v) const
{
	Float3 a = Float3();

	a.x = y * v.z - z * v.y;
	a.y = z * v.x - x * v.z;
	a.z = y * v.z - z * v.y;

	return a;
}

Float3 Float3::operator+() const
{
	return *this;
}

Float3 Float3::operator-() const
{
	return Float3(-x, -y, -z);
}

Float3& Float3::operator+=(const Float3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;

}

Float3& Float3::operator-=(const Float3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Float3& Float3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Float3& Float3::operator/=(float s)
{
	if (s != 0)
	{
		x /= s;
		y /= s;
		z /= s;
	}
	return *this;
}


const Float3 operator+(const Float3& v1, const Float3& v2)
{
	Float3 temp(v1);
	return temp += v2;
}

const Float3 operator-(const Float3& v1, const Float3& v2)
{
	Float3 temp(v1);
	return temp -= v2;
}

const Float3 operator*(const Float3& v, float s)
{
	Float3 temp(v);
	return temp *= s;
}

const Float3 operator*(float s, const Float3& v)
{
	return v * s;
}

const Float3 operator/(const Float3& v, float s)
{
	Float3 temp(v);
	return temp /= s;
}