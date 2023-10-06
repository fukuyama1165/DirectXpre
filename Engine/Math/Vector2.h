#pragma once

class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
	~Vector2();
	Vector2(const Vector2&) = default;
	Vector2& operator=(const Vector2&) = default;


	float length()const;
	//渡したものを正規化して返す
	static Vector2 normalize(const Vector2& v);
	//自分を正規化する(自分書き換える)
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