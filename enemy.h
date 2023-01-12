#pragma once
#include "Object3D.h"
#include"Input.h"
class enemy
{
public:
	enemy();
	~enemy();

	void Init(ID3D12Device* dev, const std::string directoryPath, const char filename[]);
	void Update(Camera camera,Float3 playerPos);
	void Draw(ID3D12GraphicsCommandList* cmdList);

	Object3D enemyObj;

	bool isAlive = true;
	bool isHit = false;

private:

	bool isHitSet = false;

	Float3 hitVec;

	int hitTime = 0;

	Input* input;

	Float3 pos = {};
};

