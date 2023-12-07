#include "EnemyBullet.h"
#include "EventPointManager.h"
#include <cassert>

EnemyBullet::EnemyBullet()
{

}

EnemyBullet::~EnemyBullet()
{
	CollisionManager::GetInstance()->RemoveCollider(&Collider);
}

void EnemyBullet::Initlize(const Vector3& position, const Vector3& velocity, const Vector3& size)
{


	obj_.FBXInit();

	//引数で受け取った初期座標をセット
	obj_.pos_ = position;
	obj_.Scale_ = size;

	Velocity_ = velocity;

	collision = BulletCollision("enemyBullet");



	Collider.SetObject(&collision);

	Collider.Initialize();

	Collider.size_ = { 0.8f,0.8f,0.8f };

	CollisionManager::GetInstance()->AddCollider(&Collider);


}

void EnemyBullet::Update()
{
	//移動するところ
	obj_.pos_ += Velocity_;
	obj_.Update();

	//デスタイマーをひいて0以下になったらフラグを立てるのと自機が移動中なら弾を殺す
	if (--deathTimer_ <= 0 || EventPointManager::GetInstance()->GetPEventPoint()->GetEventType()==EventType::moveEvent)
	{
		isDead_ = true;
		
	}

	Collider.Update(obj_.GetWorldPos());

	if (collision.isHit)
	{
		OnCollision();
	}

}

void EnemyBullet::Draw(AnimationModel* model)
{
	//ヌルポチェック
	assert(model);
	obj_.FBXDraw(*model);
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPosition()
{
	return obj_.GetWorldPos();
}