#pragma once
class Vector4
{
public:
	Vector4();
	Vector4(float x, float y, float z,float w);

	float length()const;
	//渡したものを正規化して返す
	static Vector4 normalize(const Vector4& v);
	//自分を正規化する(自分書き換える)
	Vector4& normalize();

	//渡したもので内積を計算して返す
	static float dot(const Vector4& v, const Vector4& v2);
	//自分と渡したもので内積を計算して返す(自分先)
	float dot(const Vector4& v);

	

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