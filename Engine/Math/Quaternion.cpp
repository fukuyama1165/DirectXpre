#include "Quaternion.h"
#include <cmath>
#include <limits>
#include "Util.h"

Quaternion::Quaternion()
{
	v = { 0,0,0 };
	w = 1;
}
Quaternion::Quaternion(float x, float y, float z, float w)
	:w(w)
{
	v = { x,y,z };
}

Quaternion::Quaternion(Vector3 axis, float angle)
{

	*this = Quaternion::MakeAxisAngle(axis, angle);

	/*w = cosf(angle / 2.f);

	float len2 = axis.length();
	float mul = sinf(angle / 2.f) / len2;
	x *= mul;
	y *= mul;
	z *= mul;*/

}

Quaternion::~Quaternion()
{

}

Quaternion Quaternion::Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	
	Quaternion ans;

	ans.v = Vector3::cross(lhs.v, rhs.v) + rhs.w * lhs.v + lhs.w * rhs.v;
	ans.w = (lhs.w * rhs.w) - Vector3::dot(lhs.v,rhs.v);
	return ans;

	/*ans.w = lhs.w * rhs.w - Vector3::dot({ lhs.x,lhs.y,lhs.z }, { rhs.x,rhs.y,rhs.z });

	Vector3 ansV;

	ansV = Vector3::cross({ lhs.x,lhs.y,lhs.z }, { rhs.x,rhs.y,rhs.z });

	ansV.x += rhs.w * lhs.x + lhs.w * rhs.x;
	ansV.y += rhs.w * lhs.y + lhs.w * rhs.y;
	ansV.z += rhs.w * lhs.z + lhs.w * rhs.z;

	ans.x = ansV.x;
	ans.y = ansV.y;
	ans.z = ansV.z;*/

	/*ans.w = lhs.w * rhs.w - lhs.v.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;

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
	ans.v.x = 0.0f;
	ans.v.y = 0.0f;
	ans.v.z = 0.0f;

	return ans;
}

Quaternion Quaternion::Conjugate(const Quaternion& quaternion)
{
	Quaternion ans;
	ans.v.x = -quaternion.v.x;
	ans.v.y = -quaternion.v.y;
	ans.v.z = -quaternion.v.z;
	ans.w = quaternion.w;

	return ans;
}

float Quaternion::Norm(const Quaternion& quaternion)
{
	return sqrtf(quaternion.v.x * quaternion.v.x + quaternion.v.y * quaternion.v.y + quaternion.v.z * quaternion.v.z + quaternion.w * quaternion.w);
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


	ans = conj / (quaternion.v.x * quaternion.v.x + quaternion.v.y * quaternion.v.y + quaternion.v.z * quaternion.v.z + quaternion.w * quaternion.w);
	
	return ans;

}

Quaternion Quaternion::MakeAxisAngle(const Vector3& axis, float angle)
{
	/*Vector3 normalVec = Vector3::normalize(axis);

	Quaternion ans;

	Vector3 ansVec = normalVec * sinf(angle / 2);

	ans.w = cosf(angle / 2);
	ans.v.x = ansVec.x;
	ans.v.y = ansVec.y;
	ans.v.z = ansVec.z;

	ans = Quaternion::Normalize(ans);

	return ans;*/

	Vector3 axis2 = axis;
	axis2.normalize();
	Quaternion q;
	q.w = cosf(angle / 2);
	q.v = axis * sinf(angle / 2);

	return q;

}

Vector3 Quaternion::RotateVector(const Vector3& vector, const Quaternion& quaternion)
{

	Quaternion quaternionVec(vector.x, vector.y, vector.z, 0);

	Quaternion multQuaternion = Multiply(quaternion, quaternionVec);

	Quaternion ans = Multiply(multQuaternion, Conjugate(quaternion));

	return {ans.v.x,ans.v.y,ans.v.z};
}

Matrix4x4 Quaternion::MakeRotateMatrix(const Quaternion& quaternion)
{
	Matrix4x4 ans;

	ans.m[0][0] = ((quaternion.w * quaternion.w) + (quaternion.v.x * quaternion.v.x) - (quaternion.v.y * quaternion.v.y) - (quaternion.v.z * quaternion.v.z));
	ans.m[0][1] = (2 * ((quaternion.v.x * quaternion.v.y) + (quaternion.w * quaternion.v.z)));
	ans.m[0][2] = (2 * ((quaternion.v.x * quaternion.v.z) - (quaternion.w * quaternion.v.y)));
	ans.m[0][3] = 0;
	
	ans.m[1][0] = (2 * ((quaternion.v.x * quaternion.v.y) - (quaternion.w * quaternion.v.z)));
	ans.m[1][1] = ((quaternion.w * quaternion.w) - (quaternion.v.x * quaternion.v.x) + (quaternion.v.y * quaternion.v.y) - (quaternion.v.z * quaternion.v.z));
	ans.m[1][2] = (2 * ((quaternion.v.y * quaternion.v.z) + (quaternion.w * quaternion.v.x)));
	ans.m[1][3] = 0;
	
	ans.m[2][0] = (2 * ((quaternion.v.x * quaternion.v.z) + (quaternion.w * quaternion.v.y)));
	ans.m[2][1] = (2 * ((quaternion.v.y * quaternion.v.z) - (quaternion.w * quaternion.v.x)));
	ans.m[2][2]= ((quaternion.w * quaternion.w) - (quaternion.v.x * quaternion.v.x) - (quaternion.v.y * quaternion.v.y) + (quaternion.v.z * quaternion.v.z));
	ans.m[2][3]= 0;

	ans.m[3][0] = 0;
	ans.m[3][1] = 0;
	ans.m[3][2] = 0;
	ans.m[3][3] = 1;

	return ans;

}

float Quaternion::dot(const Quaternion& q0, const Quaternion& q1)
{

	return q0.v.x * q1.v.x + q0.v.y * q1.v.y + q0.v.z * q1.v.z + q0.w * q1.w;

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

	

	if (cross.SquaredLength() == 0)
	{
		Vector3 axis(0.f, 1.f, 0.f);

		float theta = 0.f;

		return Quaternion(axis, theta);
	}

	//正規化する
	Vector3 axis = Vector3::normalize(cross);

	float clamp = Util::Clamp(dot, -1, 1);

	float theta = std::acos(clamp);

	Quaternion ans = MakeAxisAngle(axis, theta);

	return ans;

}

Quaternion Quaternion::operator+() const
{
	return *this;
}

Quaternion Quaternion::operator-() const
{
	return Quaternion(-v.x, -v.y, -v.z, -w);
}

Quaternion& Quaternion::operator+=(const Quaternion& q)
{
	v.x += q.v.x;
	v.y += q.v.y;
	v.z += q.v.z;
	w += q.w;
	return *this;

}

Quaternion& Quaternion::operator-=(const Quaternion& q)
{
	v.x -= q.v.x;
	v.y -= q.v.y;
	v.z -= q.v.z;
	w -= q.w;
	return *this;
}

Quaternion& Quaternion::operator*=(float s)
{
	v.x *= s;
	v.y *= s;
	v.z *= s;
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
		v.x /= s;
		v.y /= s;
		v.z /= s;
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