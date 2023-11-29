#include "Collision.h"
#include <cmath>

//float abs(float a)
//{
//	if (a < 0)
//	{
//		a = -a;
//	}
//
//	return a;
//}


bool Collision::CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, Vector3* inter)
{

	float x = sphereB.center_.x - sphereA.center_.x;
	float y = sphereB.center_.y - sphereA.center_.y;
	float z = sphereB.center_.z - sphereA.center_.z;
	float r = sphereA.radius_ + sphereB.radius_;

	//条件に該当してないなら当たってない
	if ((x * x) + (y * y) + (z * z) > (r * r))
	{
		return false;
	}

	if (inter)
	{
		//接点は中心と中心のベクトルの半分進んだ距離にした
		Vector3 vec = sphereB.center_ - sphereA.center_;

		*inter = sphereA.center_ + vec / 2;

	}

	//当たったああああ
	return true;

}

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{

	float distV = Vector3::dot(sphere.center_, plane.normal_);

	float dist = distV - plane.distance_;

	if (abs(dist) > sphere.radius_) return false;

	if (inter)
	{

		*inter = -dist * plane.normal_ + sphere.center_;

	}

	return true;
	
}

void Collision::ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest)
{

	//pointがp0の外側の頂点領域の中にあるかどうかチェック

#pragma region p0チェック

	//triangleのp0とp1のベクトル
	Vector3 p0p1 = triangle.p1_ - triangle.p0_;

	//triangleのp0とp2のベクトル
	Vector3 p0p2 = triangle.p2_ - triangle.p0_;

	//triangleのp0とPointのベクトル
	Vector3 p0Po = point - triangle.p0_;

	//triangleのp0とp1のベクトルとtriangleのp0とPointのベクトルの内積
	float d1 = Vector3::dot(p0p1, p0Po);

	//triangleのp0とp2のベクトルとtriangleのp0とPointのベクトルの内積
	float d2 = Vector3::dot(p0p2, p0Po);

	//2つの内積が負の数ならp0が一番近い
	if (d1 <= 0.0f && d2 <= 0.0f)
	{

		*closest = triangle.p0_;
		return;

	}

#pragma endregion

	//pointがp1の外側の頂点領域の中にあるかどうかチェック

#pragma region p1チェック

	//triangleのp1とPointのベクトル
	Vector3 p1Po = point - triangle.p1_;

	//triangleのp0とp1のベクトルとtriangleのp1とPointのベクトルの内積
	float d3 = Vector3::dot(p0p1, p1Po);

	//triangleのp0とp2のベクトルとtriangleのp1とPointのベクトルの内積
	float d4 = Vector3::dot(p0p2, p1Po);


	
	if (d3 >= 0.0f && d4 <= d3)
	{

		*closest = triangle.p1_;
		return;

	}


	// pointがp0p1の辺領域の中にあるかどうかチェックし、あればpointのp0p1上に対する射影を返す

	//領域内チェックのための計算これが負の数なら三角形の外にいる
	float p0p1Projection = d1 * d4 - d3 * d2;

	if (p0p1Projection <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		*closest = triangle.p0_ + ((d1 / (d1 - d3)) * p0p1);
		return;
	}

#pragma endregion

	//pointがp2の外側の頂点領域の中にあるかどうかチェック

#pragma region p2チェック

	//triangleのp2とPointのベクトル
	Vector3 p2Po = point - triangle.p2_;

	//triangleのp0とp1のベクトルとtriangleのp2とPointのベクトルの内積
	float d5 = Vector3::dot(p0p1, p2Po);

	//triangleのp0とp2のベクトルとtriangleのp2とPointのベクトルの内積
	float d6 = Vector3::dot(p0p2, p2Po);


	
	if (d6 >= 0.0f && d5 <= d6)
	{

		*closest = triangle.p2_;
		return;

	}


	// pointがp0p2の辺領域の中にあるかどうかチェックし、あればpointのp0p2上に対する射影を返す

	//領域内チェックのための計算これが負の数なら三角形の外にいる
	float p0p2Projection = d2 * d5 - d6 * d1;

	if (p0p2Projection <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		*closest = triangle.p0_ + ((d2 / (d2 - d6)) * p0p2);
		return;
	}

	// pointがp1p2の辺領域の中にあるかどうかチェックし、あればpointのp1p2上に対する射影を返す

	//領域内チェックのための計算これが負の数なら三角形の外にいる
	float p1p2Projection = d3 * d6 - d5 * d4;

	if (p1p2Projection <= 0.0f && (d4-d3) >= 0.0f && (d5-d6) >= 0.0f)
	{

		*closest = triangle.p1_ + (((d4 - d3) / ((d4 - d3) + (d5 - d6))) * (triangle.p2_ -triangle.p1_));
		return;

	}

#pragma endregion

	//三角形の中にある場合
	float denom = 1.0f / (p1p2Projection + p0p2Projection + p0p1Projection);
	float v = p0p2Projection * denom;
	float w = p0p1Projection * denom;

	*closest = triangle.p0_ + p0p1 * v + p0p2 * w;

}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter)
{

	Vector3 p;

	Collision::ClosestPtPoint2Triangle(sphere.center_, triangle, &p);

	Vector3 v = p - sphere.center_;

	float vDot = Vector3::dot(v,v);

	if (vDot > sphere.radius_ * sphere.radius_)
	{
		return false;
	}

	if (inter)
	{
		*inter = p;
	}

	return true;
}

bool Collision::CheckCube2CubeAABB(const Cube& cube1, const Cube& cube2)
{

	Vector3 min1 = cube1.center_ - cube1.size_;
	Vector3 max1 = cube1.center_ + cube1.size_;

	Vector3 min2 = cube2.center_ - cube2.size_;
	Vector3 max2 = cube2.center_ + cube2.size_;

	//中央からサイズ分の位置内にもう片方があるか
	if ((min1 <= max2) && (max1 >= min2))
	{
		//当たってますよ
		return true;
	}

	//当たってないよ
	return false;

}

bool Collision::CheckCube2BoxAABB(const Cube& cube, const Rectangular& Box)
{
	Vector3 min1 = cube.center_ - cube.size_ ;
	Vector3 max1 = cube.center_ + cube.size_ ;

	Vector3 min2 = Box.center_ - Box.sizeMin_;
	Vector3 max2 = Box.center_ + Box.sizeMax_;

	//中央からサイズ分の位置内にもう片方があるか
	if ((min1 <= max2) && (max1 >= min2))
	{
		//当たってますよ
		return true;
	}

	//当たってないよ
	return false;
}

bool Collision::CheckCube2BoxAABB(const Rectangular& Box, const Cube& cube)
{
	return CheckCube2BoxAABB(cube, Box);
}

bool Collision::CheckBox2BoxAABB(const Rectangular& Box1, const Rectangular& Box2)
{
	Vector3 min1 = Box1.center_ - Box1.sizeMin_;
	Vector3 max1 = Box1.center_ + Box1.sizeMax_;

	Vector3 min2 = Box2.center_ - Box2.sizeMin_;
	Vector3 max2 = Box2.center_ + Box2.sizeMax_;

	//中央からサイズ分の位置内にもう片方があるか
	if ((min1 <= max2) && (max1 >= min2))
	{
		//当たってますよ
		return true;
	}

	//当たってないよ
	return false;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, Vector3* inter)
{

	//誤差を撮るためのやつ
	const float epsilon = 1.0e-5f;

	//内積
	float d1 = Vector3::dot(plane.normal_, ray.dir_);

	//向きが同じなので当たってない
	if (d1 > -epsilon)
	{
		return false;
	}

	//射影を求める
	float d2 = Vector3::dot(plane.normal_, ray.start_);

	//始点と平面の距離
	float dist = d2 - plane.distance_;

	float t = dist / -d1;

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
		*inter = ray.start_ + t * ray.dir_;
	}

	return true;


}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance, Vector3* inter)
{

	//まず三角形が属する平面との当たり判定をとる
	Plane plane;

	Vector3 planeInter;

	//法線は三角形と同じ
	plane.normal_ = triangle.normal_;

	//距離は代表の位置ベクトルと法線で求められる
	plane.distance_ = Vector3::dot(triangle.normal_,triangle.p0_);

	//平面にすら当たっていないならそもそも当たってない
	if (!CheckRay2Plane(ray, plane, distance, &planeInter))
	{
		return false;
	}

	//誤差を撮るためのやつ
	const float epsilon = 1.0e-5f;

	Vector3 p0p1 = triangle.p1_ - triangle.p0_;
	Vector3 interp0 = triangle.p0_ - planeInter;

	Vector3 m0 = Vector3::cross(interp0, p0p1);

	//内側なら法線と同じ方向を向いているので別の方向なら当たってない
	if (Vector3::dot(m0, triangle.normal_) <  -epsilon)
	{
		return false;
	}

	Vector3 p1p2 = triangle.p2_ - triangle.p1_;
	Vector3 interp1 = triangle.p1_ - planeInter;

	Vector3 m1 = Vector3::cross(interp1, p1p2);

	//法線と逆方向を向いているなら当たってない
	if (Vector3::dot(m1, triangle.normal_) < -epsilon)
	{
		return false;
	}

	Vector3 p2p0 = triangle.p0_ - triangle.p2_;
	Vector3 interp2 = triangle.p2_ - planeInter;

	Vector3 m2 = Vector3::cross(interp2, p2p0);

	//法線と逆方向を向いているなら当たってない
	if (Vector3::dot(m2, triangle.normal_) < -epsilon)
	{
		return false;
	}

	if (inter != nullptr)
	{

		*inter = planeInter;

	}

	return true;

}

bool Collision::CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance, Vector3* inter)
{

	//球の中心からレイの始点へのベクトルを作る
	Vector3 m = ray.start_ - sphere.center_;
	float b = Vector3::dot(m, ray.dir_);
	float c = Vector3::dot(m, m) - (sphere.radius_ * sphere.radius_);

	//rayの始点がsphereの外側にあり(c>0)、
	//rayがsphereから離れていく方向を指している場合(b>0)、
	//当たらない
	if (c > 0.0f && b > 0.0f)
	{

		return false;

	}

	float d = (b * b) - c;

	//交点が存在しないので当たってない
	if (d < 0.0f)
	{
		return false;
	}

	//交点の一番近いところを計算(淵ではないなら2つあるため)
	float t = -b - sqrtf(d);

	//tが負の数ならレイが球の内側にいるらしいので0にする
	if (t < 0.0f)
	{
		t = 0.0f;
	}

	if (distance != nullptr)
	{

		*distance = t;

	}

	if (inter != nullptr)
	{

		*inter = ray.start_ + t * ray.dir_;

	}

	return true;

}