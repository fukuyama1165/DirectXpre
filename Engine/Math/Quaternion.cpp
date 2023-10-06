#include "Quaternion.h"
#include <cmath>
#include <limits>

Quaternion::Quaternion()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}
Quaternion::Quaternion(float x, float y, float z, float w)
	:x(x), y(y), z(z), w(w)
{

}

Quaternion::Quaternion(Vector3 axis, float angle)
{

	*this = Quaternion::MakeAxisAngle(axis, angle);

}

Quaternion::~Quaternion()
{

}

Quaternion Quaternion::Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	
	Quaternion ans;

	ans.w = lhs.w * rhs.w - Vector3::dot({ lhs.x,lhs.y,lhs.z }, { rhs.x,rhs.y,rhs.z });

	Vector3 ansV;

	ansV = Vector3::cross({ lhs.x,lhs.y,lhs.z }, { rhs.x,rhs.y,rhs.z });

	ansV.x += rhs.w * lhs.x + lhs.w * rhs.x;
	ansV.y += rhs.w * lhs.y + lhs.w * rhs.y;
	ansV.z += rhs.w * lhs.z + lhs.w * rhs.z;

	ans.x = ansV.x;
	ans.y = ansV.y;
	ans.z = ansV.z;

	/*ans.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;

	ans.x = lhs.y * rhs.z - lhs.z * rhs.y + rhs.w * lhs.x + lhs.w * rhs.w;

	ans.y = lhs.z * rhs.x - lhs.x * rhs.z + rhs.w * lhs.y + lhs.w * rhs.y;

	ans.z = lhs.x * rhs.y - lhs.y * rhs.x + rhs.w * lhs.z + lhs.w * rhs.z;*/

	//ans = Normalize(ans);

	return ans;

}

Quaternion Quaternion::IdentityQuaternion()
{
	Quaternion ans;
	ans.w = 1.0f;
	ans.x = 0.0f;
	ans.y = 0.0f;
	ans.z = 0.0f;

	return ans;
}

Quaternion Quaternion::Conjugate(const Quaternion& quaternion)
{
	Quaternion ans;
	ans.x = -quaternion.x;
	ans.y = -quaternion.y;
	ans.z = -quaternion.z;
	ans.w = quaternion.w;

	return ans;
}

float Quaternion::Norm(const Quaternion& quaternion)
{
	return sqrtf(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);
}

Quaternion Quaternion::Normalize(const Quaternion& quaternion)
{
	float len = Norm(quaternion);
	Quaternion ans;
	if (len != 0)
	{
		ans = quaternion / len;

		return ans;
	}

	return quaternion;
}

Quaternion Quaternion::Inverse(const Quaternion& quaternion)
{
	Quaternion conj = Conjugate(quaternion);
	Quaternion ans;


	ans = conj / (quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);
	
	return ans;

}

Quaternion Quaternion::MakeAxisAngle(const Vector3& axis, float angle)
{
	Vector3 normalVec = Vector3::normalize(axis);

	Quaternion ans;

	Vector3 ansVec = normalVec * sinf(angle / 2);

	ans.w = cosf(angle / 2);
	ans.x = ansVec.x;
	ans.y = ansVec.y;
	ans.z = ansVec.z;

	ans = Quaternion::Normalize(ans);

	return ans;

}

Vector3 Quaternion::RotateVector(const Vector3& vector, const Quaternion& quaternion)
{

	Quaternion quaternionVec(vector.x, vector.y, vector.z, 0);

	Quaternion multQuaternion = Multiply(quaternion, quaternionVec);

	Quaternion ans = Multiply(multQuaternion, Conjugate(quaternion));

	return {ans.x,ans.y,ans.z};
}

Matrix4x4 Quaternion::MakeRotateMatrix(const Quaternion& quaternion)
{
	Matrix4x4 ans;

	ans.m[0][0] = ((quaternion.w * quaternion.w) + (quaternion.x * quaternion.x) - (quaternion.y * quaternion.y) - (quaternion.z * quaternion.z));
	ans.m[0][1] = (2 * ((quaternion.x * quaternion.y) + (quaternion.w * quaternion.z)));
	ans.m[0][2] = (2 * ((quaternion.x * quaternion.z) - (quaternion.w * quaternion.y)));
	ans.m[0][3] = 0;
	
	ans.m[1][0] = (2 * ((quaternion.x * quaternion.y) - (quaternion.w * quaternion.z)));
	ans.m[1][1] = ((quaternion.w * quaternion.w) - (quaternion.x * quaternion.x) + (quaternion.y * quaternion.y) - (quaternion.z * quaternion.z));
	ans.m[1][2] = (2 * ((quaternion.y * quaternion.z) + (quaternion.w * quaternion.x)));
	ans.m[1][3] = 0;
	
	ans.m[2][0] = (2 * ((quaternion.x * quaternion.z) + (quaternion.w * quaternion.y)));
	ans.m[2][1] = (2 * ((quaternion.y * quaternion.z) - (quaternion.w * quaternion.x)));
	ans.m[2][2]= ((quaternion.w * quaternion.w) - (quaternion.x * quaternion.x) - (quaternion.y * quaternion.y) + (quaternion.z * quaternion.z));
	ans.m[2][3]= 0;

	ans.m[3][0] = 0;
	ans.m[3][1] = 0;
	ans.m[3][2] = 0;
	ans.m[3][3] = 1;

	return ans;

}

float Quaternion::dot(const Quaternion& q0, const Quaternion& q1)
{

	return q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;

}

Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, float t)
{
	Quaternion Q0 = q0;
	Quaternion Q1 = q1;

	float qDot = dot(Q0, Q1);

	//0未満なら反対からまわしたほうがいいので反転
	if (qDot < 0)
	{

		Q0 = -Q0;
		qDot = -qDot;

	}

	if (qDot >= 1.0f - FLT_EPSILON)
	{

		return (1.0f - t) * Q0 + t * Q1;

	}

	float theta = std::acos(qDot);

	float scale0 = sin((1 - t) * theta) / sin(theta);

	float scale1 = sin(t * theta) / sin(theta);

	return scale0 * Q0 + scale1 * Q1;

}

Quaternion Quaternion::DirectionToDirection(const Vector3& u, const Vector3& v)
{
	
	//単位ベクトルにする
	Vector3 u1 = Vector3::normalize(u);
	Vector3 v1 = Vector3::normalize(v);

	//なす角を求めるために内積を計算する
	float dot = Vector3::dot(u1, v1);

	Vector3 cross = Vector3::cross(u1, v1);

	//正規化する
	Vector3 axis = Vector3::normalize(cross);

	float theta = std::acos(dot);

	Quaternion ans = MakeAxisAngle(axis, theta);

	return ans;

}

Quaternion Quaternion::operator+() const
{
	return *this;
}

Quaternion Quaternion::operator-() const
{
	return Quaternion(-x, -y, -z, -w);
}

Quaternion& Quaternion::operator+=(const Quaternion& q)
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;

}

Quaternion& Quaternion::operator-=(const Quaternion& q)
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}

Quaternion& Quaternion::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& q)
{
	Quaternion temp = *this;

	*this=Multiply(temp, q);

	return *this;
}

Quaternion& Quaternion::operator/=(float s)
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


const Quaternion operator+(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion temp(q1);
	return temp += q2;
}

const Quaternion operator-(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion temp(q1);
	return temp -= q2;
}

const Quaternion operator*(const Quaternion& q, float s)
{
	Quaternion temp(q);
	return temp *= s;
}

const Quaternion operator*(float s, const Quaternion& q)
{
	return q * s;
}

const Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
{


	return Quaternion::Multiply(q1, q2);
}

const Quaternion operator/(const Quaternion& q, float s)
{
	Quaternion temp(q);
	return temp /= s;
}