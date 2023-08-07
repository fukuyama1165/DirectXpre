#pragma once
#include "Object3D.h"
#include"Input.h"
#include "cameraObj.h"
#include "Sprite.h"
#include "PlayerBullet.h"
#include <list>
#include "MobCollision.h"

class Player
{
public:
	Player();
	~Player();

	void Init(const std::string& directoryPath, const char filename[]);
	void Update();
	void Draw();
	void Attack();

	bool GetAttackFlag() { return attackFlag_; };

	//�}�E�X�̈ʒu���擾���Ă��̈ʒu�����e�B�N���p�̃I�u�W�F�N�g���ɓ����֐�
	void Reticle2DMouse();

	Object3D playerObj_;

	Object3D reticle3DObj_;

	//�I�u�W�F�N�g�Ƃ��ẴJ����
	cameraObj playerCamera_;

	//���߂�{��
	Camera playCamera_;

	Sprite reticle_;

	float time_ = 0;

	bool cameraCheng_ = false;

	float bulletMaxCT_ = 10;

	float bulletSpeed_ = 1.0f;

	bool isDebugShot_ = false;

	float muzzleFlashTime_ = 2.0f;

	float muzzleFlashMaxTime_ = 2.0f;

	float hp_ = 3;

private:

	void HideRightWall();

	void HideDownWall();

private:

	
	
	float rotate_ = 0;
	

	Input* input_ =nullptr;

	Vector3 pos_ = {};

	Vector3 moveVec_;

	bool moveEventStart_ = false;

	float moveSpeed_ = 0.1f;

	//�����_���炩�Ȉړ��̂���
	float maxTime_ = 500;
	//�J�����̎n�_�̈ړ�����Ƃ��̎���
	float maxMoveTime_ = maxTime_/15;

	bool attackFlag_ = false;

	float attackTime_ = 3;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	AnimationModel* bulletModel_=nullptr;
	AnimationModel* gunModel_=nullptr;

	float bulletCT_ = 0;

	const float bulletMaxNum_ = 6;
	float bulletNum_ = 6;

	//�����蔻��̏���
	MobCollision collision;

	//�����蔻��̖{��
	SphereCollider Collider;

	Sprite hp1Sprote_;
	Sprite hp2Sprote_;
	Sprite hp3Sprote_;

	std::vector<Sprite>bulletSprite_;
	
	
};

