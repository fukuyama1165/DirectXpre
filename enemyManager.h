#pragma once
#include "enemy.h"
#include <list>
class enemyManager
{

public:

	void PopEnemy(Vector3 pos);

	void UpDate(Camera camera, Vector3 playerPos);

	void Draw();

private:

public:
	std::list<std::unique_ptr<enemy>> enemys_;

	uint32_t enemyCount_ = 0;

};

