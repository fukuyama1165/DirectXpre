#include "Vector2.h"
#include <cmath>

#include "Vector3.h"

Vector2::Vector2()
{
	x = 0;
	y = 0;
}
Vector2::Vector2(float x, float y)
	:x(x),y(y)
{
}

Vector2::~Vector2()
{
}

float Vector2::length()const
{
	return sqrtf(x * x + y * y);
}

Vector2 Vector2::normalize(const Vector2& v)
{
	float len = v.length();
	Vector2 ans = v;
	if (len != 0)
	{
		return ans /= len;
	}

	return ans;
}

Vector2& Vector2::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}

	return *this;
}

Vector2 Vector2::operator+() const
{
	return *this;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

Vector2& Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;
	return *this;

}

Vector2& Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2& Vector2::operator*=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

Vector2& Vector2::operator/=(float s)
{
	if (s != 0)
	{
		x /= s;
		y /= s;
	}
	return *this;
}


const Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
	Vector2 temp(v1);
	return temp += v2;
}

const Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
	Vector2 temp(v1);
	return temp -= v2;
}

const Vector2 operator*(const Vector2& v, float s)
{
	Vector2 temp(v);
	return temp *= s;
}

const Vector2 operator*(float s, const Vector2& v)
{
	return v * s;
}

const Vector2 operator/(const Vector2& v, float s)
{
	Vector2 temp(v);
	return temp /= s;
}

Vector2 nainavec3(Vector2 a, Vector2 b)
{
	Vector2 ans = {};

	ans = { a.x - b.x,a.y - b.y};

	return ans;

}

const Vector2 lerp(const Vector2& start, const Vector2& end, const float t)
{
	return start * (1.0f - t) + end * t;
}