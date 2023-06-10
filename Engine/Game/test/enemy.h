#pragma once
#include "Object3D.h"
#include"Input.h"
class enemy
{
public:
	enemy();
	~enemy();

	void Init(const std::string directoryPath, const char filename[]);
	void Update(Camera camera,Vector3 playerPos);
	void Draw();

	Object3D enemyObj_;

	bool isAlive_ = true;
	bool isHit_ = false;

private:

	bool isHitSet_ = false;

	Vector3 hitVec_;

	uint32_t hitTime_ = 0;

	Input* input_=nullptr;

	Vector3 pos_ = {};
};
