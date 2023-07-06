#pragma once
#include "Object3D.h"
#include"Input.h"
#include "cameraObj.h"
class player
{
public:
	player();
	~player();

	void Init(const std::string& directoryPath, const char filename[]);
	void Update(const Camera& camera);
	void Draw();
	void Attack();

	bool GetAttackFlag() { return attackFlag_; };

	Object3D playerObj_;

	cameraObj playerCamera_;

private:

	
	float rotate_ = 0;
	

	Input* input_ =nullptr;

	Vector3 pos_ = {};

	Vector3 moveVec_;

	float moveSpeed_ = 0.1f;

	bool attackFlag_ = false;

	float attackTime_ = 3;
};

