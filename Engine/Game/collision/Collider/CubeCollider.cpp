#include "CubeCollider.h"

void CubeCollider::Initialize()
{
	collisionObject_->Initialize();
}

void CubeCollider::Update(const Vector3 pos)
{

	collisionObject_->Update(pos);

	//ワールド行列から座標を抽出
	const Matrix4x4& matWorld = collisionObject_->collisionObj_.GetWorldMat();

	//立方体のメンバ変数を更新
	Cube::center_.x = matWorld.m[3][0];
	Cube::center_.y = matWorld.m[3][1];
	Cube::center_.z = matWorld.m[3][2];

	Cube::size_ = size_;



}