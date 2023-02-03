#pragma once
#include "Object3D.h"
#include"Input.h"
class enemy
{
public:
	enemy();
	~enemy();

	void Init(ID3D12Device* dev, const std::string directoryPath, const char filename[]);
	void Update(Camera camera,Vector3 playerPos);
	void Draw(ID3D12GraphicsCommandList* cmdList);

	Object3D enemyObj;

	bool isAlive = true;
	bool isHit = false;

private:

	bool isHitSet = false;

	Vector3 hitVec;

	Vector3 hitPos;

	int hitTime = 0;

	Input* input;

	Vector3 pos = {};
};

