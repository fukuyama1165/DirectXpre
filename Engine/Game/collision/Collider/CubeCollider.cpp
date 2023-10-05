#include "CubeCollider.h"

void CubeCollider::Initialize()
{
	collisionObject_->Initialize();
}

void CubeCollider::Update(const Vector3 pos)
{

	collisionObject_->Update(pos);

	//���[���h�s�񂩂���W�𒊏o
	const Matrix4x4& matWorld = collisionObject_->collisionObj_.GetWorldMat();

	//�����̂̃����o�ϐ����X�V
	Cube::center_.x = matWorld.m[3][0];
	Cube::center_.y = matWorld.m[3][1];
	Cube::center_.z = matWorld.m[3][2];

	Cube::size_ = size_;



}