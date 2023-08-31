#pragma once
#include "Object3D.h"
#include"Input.h"
#include "EnemyBullet.h"
#include <list>
#include "MobCollision.h"
#include "CubeCollider.h"

enum EnemyType
{
	moveOnly,
	moveAttack,
	Attack
};

class Enemy
{
public:
	Enemy();
	~Enemy();

	void Init(uint16_t enemyType,Vector3 pos,Vector3 movePointPos);
	void Update(std::string soundH);
	void Draw(AnimationModel* model);

	void Move();
	void Attack();

	Vector3 GetObjWorldPos() { return enemyObj_.GetWorldPos(); };

	uint32_t GetCT() { return bulletCT_; };

	void OnCollision();

	Object3D enemyObj_;

	bool isAlive_ = true;
	bool isHit_ = false;

private:

	uint32_t bulletCT_ = 200;

	uint32_t bulletMaxCT_ = 200;

	AnimationModel* bulletModel_=nullptr;

	//ìñÇΩÇËîªíËÇÃèàóù
	MobCollision collision;

	//ìñÇΩÇËîªíËÇÃñ{ëÃ
	CubeCollider Collider;

	uint16_t enemyType_ = moveOnly;

	bool moveEnd_ = false;

	Vector3 moveStartPos_ = { 0,0,0 };

	Vector3 movePoint_ = { 0,0,0 };
	
};

