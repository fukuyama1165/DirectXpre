#include "PlayerBullet.h"
#include <cassert>
#include "CollisionManager.h"

PlayerBullet::PlayerBullet()
{
	
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initlize(const Vector3& position, const Vector3& velocity)
{

	
	obj_.FBXInit();		

	//引数で受け取った初期座標をセット
	obj_.Trans_ = position;

	Velocity_ = velocity;

	collision = BulletCollision("playerBullet");

	Collider.SetObject(&collision);

	CollisionManager::GetInstance()->AddCollider(&Collider);

}

void PlayerBullet::Update(const Camera& camera)
{
	//移動するところ
	obj_.Trans_ += Velocity_;
	obj_.Update(camera);

	//デスタイマーをひいて0以下になったらフラグを立てる
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

	Collider.Update(camera, obj_.GetWorldPos());

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
}

Vector3 PlayerBullet::GetWorldPosition()
{
	return obj_.GetWorldPos();
}