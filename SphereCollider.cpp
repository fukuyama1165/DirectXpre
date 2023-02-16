#include "SphereCollider.h"


void SphereCollider::Update()
{

	//ワールド行列から座標を抽出
	const Matrix4x4& matWorld = object3d->GetWorldMat();

	//球のメンバ変数を更新
	Sphere::center.x = matWorld.m[3][0] + offset.x;
	Sphere::center.y = matWorld.m[3][1] + offset.y;
	Sphere::center.z = matWorld.m[3][2] + offset.z;

	Sphere::radius = radius;


}