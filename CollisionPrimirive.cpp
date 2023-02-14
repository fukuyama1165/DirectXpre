#include "CollisionPrimirive.h"


void Triangle::ComputeNormal()
{

	Vector3 p0p1 = p1 - p0;
	Vector3 p0p2 = p2 - p0;

	//外積によって法線を求める
	normal = Vector3::cross(p0p1, p0p2);
	normal = Vector3::normalize(normal);

}