#pragma once

class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
	~Vector2();

	float length()const;
	//“n‚µ‚½‚à‚Ì‚ğ³‹K‰»‚µ‚Ä•Ô‚·
	static Vector2 normalize(const Vector2& v);
	//©•ª‚ğ³‹K‰»‚·‚é(©•ª‘‚«Š·‚¦‚é)
	Vector2& normalize();

	Vector2 operator+() const;
	Vector2 operator-() const;

	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(float s);
	Vector2& operator/=(float s);

public:
	float x;
	float y;
private:

};

const Vector2 operator+(const Vector2& v1, const Vector2& v2);
const Vector2 operator-(const Vector2& v1, const Vector2& v2);
const Vector2 operator*(const Vector2& v, float s);
const Vector2 operator*(float s, const Vector2& v);
const Vector2 operator/(const Vector2& v, float s);