#include "SphereCollider.h"


void SphereCollider::Update()
{

	//���[���h�s�񂩂���W�𒊏o
	const Matrix4x4& matWorld = object3d->GetWorldMat();

	//���̃����o�ϐ����X�V
	Sphere::center.x = matWorld.m[3][0] + offset.x;
	Sphere::center.y = matWorld.m[3][1] + offset.y;
	Sphere::center.z = matWorld.m[3][2] + offset.z;

	Sphere::radius = radius;


}