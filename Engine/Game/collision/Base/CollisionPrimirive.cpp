#include "CollisionPrimirive.h"


void Triangle::ComputeNormal()
{

	Vector3 p0p1 = p1_ - p0_;
	Vector3 p0p2 = p2_ - p0_;

	//外積によって法線を求める
	normal_ = Vector3::cross(p0p1, p0p2);
	normal_ = Vector3::normalize(normal_);

}