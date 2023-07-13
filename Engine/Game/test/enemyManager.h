#pragma once
#include "Enemy.h"
#include <list>
class EnemyManager
{

public:

	void PopEnemy(const Vector3& pos);

	void Init();

	void UpDate(const Camera& camera,const Vector3& playerPos);

	void EnemyAttack(Enemy enemy, const Vector3& playerPos);

	void Draw();

private:

public:
	std::list<std::unique_ptr<Enemy>> enemys_;

	std::unique_ptr<AnimationModel> bulletModel_;
	std::unique_ptr<AnimationModel> enemyModel_;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	uint32_t enemyCount_ = 0;

};

