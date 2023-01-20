#pragma once

class Vector3
{
public:
	Vector3();
	Vector3(float x,float y,float z);
	~Vector3();

	float length()const;
	//“n‚µ‚½‚à‚Ì‚ğ³‹K‰»‚µ‚Ä•Ô‚·
	static Vector3& normalize(const Vector3& v);
	//©•ª‚ğ³‹K‰»‚·‚é(©•ª‘‚«Š·‚¦‚é)
	Vector3& normalize();
	//“n‚µ‚½‚à‚Ì‚Å“àÏ‚ğŒvZ‚µ‚Ä•Ô‚·
	static float dot(const Vector3& v, const Vector3& v2);
	//©•ª‚Æ“n‚µ‚½‚à‚Ì‚Å“àÏ‚ğŒvZ‚µ‚Ä•Ô‚·(©•ªæ)
	float dot(const Vector3& v);
	//“n‚µ‚½‚à‚Ì‚ÅŠOÏ‚ğŒvZ‚µ‚Ä•Ô‚·
	static Vector3 cross(const Vector3& v, const Vector3& v2);
	//©•ª‚Æ“n‚µ‚½‚à‚Ì‚ÅŠOÏ‚ğŒvZ‚µ‚Ä•Ô‚·(©•ªæ)
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

//•âŠÔŠÖ”
//üŒ`•âŠÔ(1ŸŠÖ”•âŠÔ)
const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);