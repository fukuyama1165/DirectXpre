#pragma once
#include "Object3D.h"
#include"Input.h"
#include "cameraObj.h"
#include "Sprite.h"
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

	void Reticle2DMouseAttack(Camera camera);

	Object3D playerObj_;

	Object3D reticle3DObj_;

	cameraObj playerCamera_;

	Camera hogeca;

	Sprite reticle_;

	float time_ = 0;

	bool cameraCheng_ = false;

private:

	void HideRightWall();

	void HideDownWall();

private:

	
	
	float rotate_ = 0;
	

	Input* input_ =nullptr;

	Vector3 pos_ = {};

	Vector3 moveVec_;

	float moveSpeed_ = 0.1f;

	
	float maxTime_ = 500;
	float maxMoveTime_ = maxTime_/15;

	bool attackFlag_ = false;

	float attackTime_ = 3;
};

