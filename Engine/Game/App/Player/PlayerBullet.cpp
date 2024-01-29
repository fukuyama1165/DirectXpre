#include "PlayerBullet.h"
#include <cassert>
#include "CollisionManager.h"

PlayerBullet::PlayerBullet()
{
	
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initlize(const Vector3& position, const Vector3& velocity,const Vector3& size)
{

	
	obj_.FBXInit();		

	//引数で受け取った初期座標をセット
	obj_.pos_ = position;
	obj_.Scale_ = size;

	Velocity_ = velocity;

	collision = BulletCollision("playerBullet");

	collision.collisionObj_.Scale_ = size;

	Collider.SetObject(&collision);

	Collider.Initialize();

	CollisionManager::GetInstance()->AddCollider(&Collider);

}

void PlayerBullet::Update()
{
	//移動するところ
	obj_.pos_ += Velocity_;
	obj_.Update();

	//デスタイマーをひいて0以下になったらフラグを立てる
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
		CollisionManager::GetInstance()->RemoveCollider(&Collider);
	}

	Collider.Update(obj_.GetWorldPos());

	if (collision.isHit)
	{
		OnCollision();
	}

}

void PlayerBullet::Draw(AnimationModel* model)
{
	//ヌルポチェック
	assert(model);
	obj_.FBXDraw(*model);

	collision.Draw(model);
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
	Collider.isActive_ = false;
	CollisionManager::GetInstance()->RemoveCollider(&Collider);
}

Vector3 PlayerBullet::GetWorldPosition()
{
	return obj_.GetWorldPos();
}