#include "Float4.h"
#include <cmath>

Float4::Float4()
{
	x = 0;
	y = 0;
	z = 0;
}
Float4::Float4(float x, float y, float z,float w)
	:x(x), y(y), z(z),w(w)
{
}

float Float4::dot(const Float4& v) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

Float4 Float4::cross(const Float4& v) const
{
	Float4 a = Float4();

	a.x = y * v.z - z * v.y;
	a.y = z * v.x - x * v.z;
	a.z = y * v.z - z * v.y;
	a.w = y * v.z - z * v.y;

	return a;
}

float Float4::length()const
{
	return sqrtf(x * x + y * y + z * z+w*w);
}

Float4& Float4::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}

	return *this;
}

Float4& Float4::operator/=(float s)
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

const Float4 operator/(const Float4& v, float s)
{
	Float4 temp(v);
	return temp /= s;
}