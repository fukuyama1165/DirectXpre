#pragma once
class Vector4
{
public:
	Vector4();
	Vector4(float x, float y, float z,float w);

	float length()const;
	//“n‚µ‚½‚à‚Ì‚ğ³‹K‰»‚µ‚Ä•Ô‚·
	static Vector4 normalize(const Vector4& v);
	//©•ª‚ğ³‹K‰»‚·‚é(©•ª‘‚«Š·‚¦‚é)
	Vector4& normalize();

	//“n‚µ‚½‚à‚Ì‚Å“àÏ‚ğŒvZ‚µ‚Ä•Ô‚·
	static float dot(const Vector4& v, const Vector4& v2);
	//©•ª‚Æ“n‚µ‚½‚à‚Ì‚Å“àÏ‚ğŒvZ‚µ‚Ä•Ô‚·(©•ªæ)
	float dot(const Vector4& v);
	//“n‚µ‚½‚à‚Ì‚ÅŠOÏ‚ğŒvZ‚µ‚Ä•Ô‚·
	static Vector4 cross(const Vector4& v, const Vector4& v2);
	//©•ª‚Æ“n‚µ‚½‚à‚Ì‚ÅŠOÏ‚ğŒvZ‚µ‚Ä•Ô‚·(©•ªæ)
	Vector4 cross(const Vector4& v);

	

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