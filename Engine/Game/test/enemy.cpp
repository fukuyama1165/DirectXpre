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

}

void Enemy::Update(const Camera& camera)
{
	if (isAlive_)
	{
		

		enemyObj_.Update(camera);

		Attack();
		
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