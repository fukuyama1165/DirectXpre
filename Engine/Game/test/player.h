#pragma once
#include "Object3D.h"
#include"Input.h"
#include "cameraObj.h"
#include "Sprite.h"
#include "PlayerBullet.h"
#include <list>

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

	//�}�E�X�̈ʒu���擾���Ă��̈ʒu�����e�B�N���p�̃I�u�W�F�N�g���ɓ����֐�
	void Reticle2DMouse(Camera camera);

	Object3D playerObj_;

	Object3D reticle3DObj_;

	//�I�u�W�F�N�g�Ƃ��ẴJ����
	cameraObj playerCamera_;

	//���߂�{��
	Camera playCamera_;

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

	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	std::unique_ptr<AnimationModel> bulletModel_;

	uint32_t bulletCT_;

};

