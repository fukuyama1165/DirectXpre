#pragma once
#include "enemy.h"
#include <list>
class enemyManager
{

public:

	void PopEnemy(const Vector3& pos);

	void Init();

	void UpDate(const Camera& camera,const Vector3& playerPos);

	void Draw();

private:

public:
	std::list<std::unique_ptr<enemy>> enemys_;

	std::unique_ptr<AnimationModel> bulletModel_;

	uint32_t enemyCount_ = 0;

};

