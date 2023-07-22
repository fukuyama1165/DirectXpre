#include "Enemy.h"


Enemy::Enemy()
{
}
Enemy::~Enemy()
{

}

void Enemy::Init()
{
	
	enemyObj_.FBXInit();

	collision = MobCollision("enemy");

	Collider.SetObject(&collision);

	Collider.Initialize();

	CollisionManager::GetInstance()->AddCollider(&Collider);

}

void Enemy::Update(const Camera& camera)
{
	if (isAlive_)
	{
		

		enemyObj_.Update(camera);

		Attack();

		Collider.Update(camera, enemyObj_.GetWorldPos());

		if (collision.isHit)
		{
			OnCollision();
		}
		
	}
}

void Enemy::Draw(AnimationModel* model)
{
	//ヌルポチェック
	assert(model);
	enemyObj_.FBXDraw(*model);
}

void Enemy::Attack()
{

	if (bulletCT_ <= 0)
	{
		bulletCT_ = bulletMaxCT_;
	}
	else
	{
		bulletCT_--;
	}

	
}

void Enemy::OnCollision()
{

	isAlive_ = false;

	CollisionManager::GetInstance()->RemoveCollider(&Collider);

}