#include "Collison.h"

float abs(float a)
{
	if (a < 0)
	{
		a = -a;
	}

	return a;
}


bool Collison::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector4* inter)
{

	float distV = Vector4::dot(sphere.center, plane.normal);

	float dist = distV - plane.distance;

	if (abs(dist) > sphere.radius) return false;

	if (inter)
	{

		*inter = -dist * plane.normal + sphere.center;

	}

	return true;
	
}

void Collison::ClosestPtPoint2Triangle(const Vector4& point, const Triangle& triangle, Vector4* closest)
{

	//pointがp0の外側の頂点領域の中にあるかどうかチェック

#pragma region p0チェック

	//triangleのp0とp1のベクトル
	Vector4 p0p1 = triangle.p1 - triangle.p0;

	//triangleのp0とp2のベクトル
	Vector4 p0p2 = triangle.p2 - triangle.p0;

	//triangleのp0とPointのベクトル
	Vector4 p0Po = point - triangle.p0;

	//triangleのp0とp1のベクトルとtriangleのp0とPointのベクトルの内積
	float d1 = Vector4::dot(p0p1, p0Po);

	//triangleのp0とp2のベクトルとtriangleのp0とPointのベクトルの内積
	float d2 = Vector4::dot(p0p2, p0Po);

	//2つの内積が負の数ならp0が一番近い
	if (d1 <= 0.0f and d2 <= 0.0f)
	{

		*closest = triangle.p0;
		return;

	}

#pragma endregion

	//pointがp1の外側の頂点領域の中にあるかどうかチェック

#pragma region p1チェック

	//triangleのp1とPointのベクトル
	Vector4 p1Po = point - triangle.p1;

	//triangleのp0とp1のベクトルとtriangleのp1とPointのベクトルの内積
	float d3 = Vector4::dot(p0p1, p1Po);

	//triangleのp0とp2のベクトルとtriangleのp1とPointのベクトルの内積
	float d4 = Vector4::dot(p0p2, p1Po);


	
	if (d3 >= 0.0f and d4 <= d3)
	{

		*closest = triangle.p1;
		return;

	}


	// pointがp0p1の辺領域の中にあるかどうかチェックし、あればpointのp0p1上に対する射影を返す

	//領域内チェックのための計算これが負の数なら三角形の外にいる
	float p0p1Projection = d1 * d4 - d3 * d2;

	if (p0p1Projection <= 0.0f and d1 >= 0.0f and d3 <= 0.0f)
	{
		*closest = triangle.p0 + ((d1 / (d1 - d3)) * p0p1);
		return;
	}

#pragma endregion

	//pointがp2の外側の頂点領域の中にあるかどうかチェック

#pragma region p2チェック

	//triangleのp2とPointのベクトル
	Vector4 p2Po = point - triangle.p2;

	//triangleのp0とp1のベクトルとtriangleのp2とPointのベクトルの内積
	float d5 = Vector4::dot(p0p1, p2Po);

	//triangleのp0とp2のベクトルとtriangleのp2とPointのベクトルの内積
	float d6 = Vector4::dot(p0p2, p2Po);


	
	if (d6 >= 0.0f and d5 <= d6)
	{

		*closest = triangle.p2;
		return;

	}


	// pointがp0p2の辺領域の中にあるかどうかチェックし、あればpointのp0p2上に対する射影を返す

	//領域内チェックのための計算これが負の数なら三角形の外にいる
	float p0p2Projection = d2 * d5 - d6 * d1;

	if (p0p2Projection <= 0.0f and d2 >= 0.0f and d6 <= 0.0f)
	{
		*closest = triangle.p0 + ((d2 / (d2 - d6)) * p0p2);
		return;
	}

	// pointがp1p2の辺領域の中にあるかどうかチェックし、あればpointのp1p2上に対する射影を返す

	//領域内チェックのための計算これが負の数なら三角形の外にいる
	float p1p2Projection = d3 * d6 - d5 * d4;

	if (p1p2Projection <= 0.0f and (d4-d3) >= 0.0f and (d5-d6) >= 0.0f)
	{

		*closest = triangle.p1 + (((d4 - d3) / ((d4 - d3) + (d5 - d6))) * (triangle.p2-triangle.p1));
		return;

	}

#pragma endregion

	//三角形の中にある場合
	float denom = 1.0f / (p1p2Projection + p0p2Projection + p0p1Projection);
	float v = p0p2Projection * denom;
	float w = p0p1Projection * denom;

	*closest = triangle.p0 + p0p1 * v + p0p2 * w;

}

bool Collison::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector4* inter)
{

	Vector4 p;

	Collison::ClosestPtPoint2Triangle(sphere.center, triangle, &p);

	Vector4 v = p - sphere.center;

	float vDot = Vector4::dot(v,v);

	if (vDot > sphere.radius * sphere.radius)
	{
		return false;
	}

	if (inter)
	{
		*inter = p;
	}

	return true;
}

bool Collison::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, Vector4* inter)
{

	//誤差を撮るためのやつ
	const float epsilon = 1.0e-5f;

	//内積
	float d1 = Vector4::dot(plane.normal, ray.dir);

	//向きが同じなので当たってない
	if (d1 > -epsilon)
	{
		return false;
	}

	//射影を求める
	float d2 = Vector4::dot(plane.normal, ray.start);

	//始点と平面の距離
	float dist = d2 - plane.distance;

	float t = dist / (-Vector4::dot(plane.normal, ray.dir));

	if (t < 0)
	{
		return false;
	}

	if (distance != nullptr)
	{

		*distance = t;

	}

	if (inter != nullptr)
	{
		*inter = ray.start + t * ray.dir;
	}

	return true;


}


bool Collison::CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance, Vector4* inter)
{

	//まず三角形が属する平面との当たり判定をとる
	Plane plane;

	Vector4 planeInter;

	//法線は三角形と同じ
	plane.normal = triangle.normal;

	//距離は代表の位置ベクトルと法線で求められる
	plane.distance = Vector4::dot(triangle.normal,triangle.p0);

	//平面にすら当たっていないならそもそも当たってない
	if (!CheckRay2Plane(ray, plane, distance, &planeInter))
	{
		return false;
	}

	//誤差を撮るためのやつ
	const float epsilon = 1.0e-5f;

	Vector4 p0p1 = triangle.p1 - triangle.p0;
	Vector4 interp0 = triangle.p0 - planeInter;

	Vector4 m0 = Vector4::cross(interp0, p0p1);

	//内側なら法線と同じ方向を向いているので別の方向なら当たってない
	if (Vector4::dot(m0, triangle.normal) <  -epsilon)
	{
		return false;
	}

	Vector4 p1p2 = triangle.p2 - triangle.p1;
	Vector4 interp1 = triangle.p1 - planeInter;

	Vector4 m1 = Vector4::cross(interp1, p1p2);

	//法線と逆方向を向いているなら当たってない
	if (Vector4::dot(m1, triangle.normal) < -epsilon)
	{
		return false;
	}

	Vector4 p2p0 = triangle.p0 - triangle.p2;
	Vector4 interp2 = triangle.p2 - planeInter;

	Vector4 m2 = Vector4::cross(interp2, p2p0);

	//法線と逆方向を向いているなら当たってない
	if (Vector4::dot(m2, triangle.normal) < -epsilon)
	{
		return false;
	}

	if (inter != nullptr)
	{

		*inter = planeInter;

	}

	return true;

}