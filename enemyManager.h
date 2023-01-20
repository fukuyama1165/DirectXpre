#pragma once
#include "enemy.h"
#include <list>
class enemyManager
{

public:

	void PopEnemy(ID3D12Device* dev, Vector3 pos);

	void UpDate(Camera camera, Vector3 playerPos);

	void Draw(ID3D12GraphicsCommandList* cmdList);

private:

public:
	std::list<std::unique_ptr<enemy>> enemys;

	int enemyCount = 0;

};

