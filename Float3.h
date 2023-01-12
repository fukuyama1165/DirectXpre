#pragma once

class Float3
{
public:
	Float3();
	Float3(float x,float y,float z);
	~Float3();

	float length()const;
	Float3& normalize();
	float dot(const Float3& v)const;
	Float3 cross(const Float3& v)const;

	Float3 operator+() const;
	Float3 operator-() const;

	Float3& operator+=(const Float3& v);
	Float3& operator-=(const Float3& v);
	Float3& operator*=(float s);
	Float3& operator/=(float s);

public:
	float x;
	float y;
	float z;
private:

};

const Float3 operator+(const Float3& v1, const Float3& v2);
const Float3 operator-(const Float3& v1, const Float3& v2);
const Float3 operator*(const Float3& v, float s);
const Float3 operator*(float s, const Float3& v);
const Float3 operator/(const Float3& v, float s);

Float3 nainavec3(Float3 a, Float3 b);