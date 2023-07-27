#pragma once
#include "Enemy.h"
#include <list>
class EnemyManager
{

public:

	static EnemyManager* GetInstance();

	void PopEnemy(const Vector3& pos);

	void Init();

	void UpDate(const Camera& camera,const Vector3& playerPos);

	void EnemyAttack(Enemy enemy, const Vector3& playerPos);

	void Draw();

private:

	EnemyManager() = default;
	~EnemyManager();

	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;
public:
	std::list<std::unique_ptr<Enemy>> enemys_;

	std::unique_ptr<AnimationModel> bulletModel_ = nullptr;
	std::unique_ptr<AnimationModel> enemyModel_ = nullptr;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	uint32_t enemyCount_ = 0;

	bool isDebugShot_ = false;

	//�ړ��ʂ�ǉ�
	float bulletSpeed_ = 1.0f;

};
