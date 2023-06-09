#pragma once
#include "Object3D.h"
#include"Input.h"
#include "cameraObj.h"
class player
{
public:
	player();
	~player();

	void Init(const std::string directoryPath, const char filename[]);
	void Update(cameraObj camera);
	void Draw();
	void Attack();

	bool GetAttackFlag() { return attackFlag_; };

	Object3D playerObj_;

	Object3D attackObj_;

private:

	
	float rotate_ = 0;
	

	Input* input_ =nullptr;

	Vector3 pos_ = {};

	Vector3 moveVec_;

	float moveSpeed_ = 5;

	bool attackFlag_ = false;

	float attackTime_ = 3;
};

