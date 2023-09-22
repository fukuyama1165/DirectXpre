#pragma once
#include "Object3D.h"
#include"Input.h"
#include "cameraObj.h"
#include "Sprite.h"
#include "PlayerBullet.h"
#include <list>
#include "MobCollision.h"
#include "CubeCollider.h"

class Player
{
public:
	Player();
	~Player();

	void Init(const std::string& directoryPath, const char filename[]);
	void Update();
	void Draw();
	void Attack();
	void Damage();

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
	CubeCollider Collider;

	Sprite hp1Sprote_;
	Sprite hp2Sprote_;
	Sprite hp3Sprote_;
	Sprite damageSprote_;

	std::vector<Sprite>bulletSprite_;

	std::string gunShotSount_;
	std::string gunReloadSount_;
	std::string damageSound_;
	

	bool isUseKeybord_ = true;

	//�ǂɉB�ꂽ�肷��p�̊�ʒu
	Vector3 originalPos_ = {};

	Vector3 test = {};

	//�_���[�W���󂯂���
	bool isDamage_ = false;

	//�_���[�W���󂯂����̗h���͈�
	Vector3 shakeVecSize_ = {90,0,0};

	//�h��鋭��
	Vector3 shakePow_ = { 0.1f,1.0f,1.0f };

	//�_���[�W���󂯂����̃V�F�C�N�p�^�C�}�[
	float damageTimer_ = 0;
	float damageMaxTimer_ = 10;
	
	//�_���[�W���󂯂����̉��o�p�̃^�C�}�[
	float damageEffectMaxTime_ = 90;
	float damageEffectTimer_ = damageEffectMaxTime_;

};

