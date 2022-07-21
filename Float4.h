#pragma once
class Float4
{
public:
	Float4();
	Float4(float x, float y, float z,float w);

	float length()const;
	Float4& normalize();

	float dot(const Float4& v)const;
	Float4 cross(const Float4& v)const;

	Float4& operator/=(float s);

public:
	float x;
	float y;
	float z;
	float w;


};

const Float4 operator/(const Float4& v, float s);