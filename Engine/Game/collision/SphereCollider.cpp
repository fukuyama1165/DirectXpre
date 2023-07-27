#include "SphereCollider.h"

void SphereCollider::Initialize()
{
	collisionObject_->Initialize();
}

void SphereCollider::Update(const Camera& camera, const Vector3 pos)
{

	collisionObject_->Update(camera, pos);

	//���[���h�s�񂩂���W�𒊏o
	const Matrix4x4& matWorld = collisionObject_->collisionObj_.GetWorldMat();

	//���̃����o�ϐ����X�V
	Sphere::center_.x = matWorld.m[3][0] + offset_.x;
	Sphere::center_.y = matWorld.m[3][1] + offset_.y;
	Sphere::center_.z = matWorld.m[3][2] + offset_.z;

	Sphere::radius_ = radius_;

	

}