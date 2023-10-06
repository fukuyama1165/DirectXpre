#pragma once
#include "Enemy.h"
#include <list>
class EnemyManager
{

public:

	static EnemyManager* GetInstance();

	void PopEnemy(std::string enemyType, Vector3 pos, Vector3 movePoint = { 0,0,0 }, float moveSpeed=1.0f);

	void Init();

	void UpDate(const Vector3& playerPos);

	void EnemyAttack(Enemy enemy, const Vector3& playerPos);

	void Draw();

	void Reset() { enemys_.clear(); bullets_.clear(); };

private:

	EnemyManager() = default;
	~EnemyManager();

	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;
public:
	std::list<std::unique_ptr<Enemy>> enemys_;

	AnimationModel* bulletModel_ = nullptr;
	AnimationModel* enemyModel_ = nullptr;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	uint32_t enemyCount_ = 0;

	bool isDebugShot_ = false;

	bool isDebugShotStop_ = false;

	//移動量を追加
	float bulletSpeed_ = 1.0f;

	std::string enemyDownSound_;

};
