#include "enemy.h"


enemy::enemy()
{

}
enemy::~enemy()
{

}

void enemy::Init(const std::string directoryPath, const char filename[])
{
	input_ = input_->GetInstance();
	enemyObj_.objDrawInit(directoryPath, filename);

}

void enemy::Update(Camera camera,Vector3 playerPos)
{
	if (isAlive_)
	{
		if (isHit_ == false)
		{

			Vector3 playerEnemyVec = enemyObj_.GetPos() - playerPos;

			playerEnemyVec = playerEnemyVec.normalize();

			enemyObj_.SetPos(enemyObj_.GetPos() - (playerEnemyVec * 2));

		}
		else
		{
			if (isHitSet_ == false)
			{
				hitVec_ = enemyObj_.GetPos() - playerPos;
				hitVec_ = hitVec_.normalize();
				hitVec_ = -hitVec_;
				isHitSet_ = true;
			}

			enemyObj_.SetPos(enemyObj_.GetPos() - (hitVec_ * 10));
			hitTime_++;
		}

		enemyObj_.Update(camera);

		if (hitTime_ > 300)
		{
			isAlive_ = false;
		}
	}
}

void enemy::Draw()
{
	enemyObj_.Draw();
}