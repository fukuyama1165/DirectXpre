#pragma once
#include "Object3D.h"
#include"Input.h"
#include "EnemyBullet.h"
#include <list>
#include "MobCollision.h"

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

	void Init();
	void Update();
	void Draw(AnimationModel* model);

	void Attack();

	Vector3 GetObjWorldPos() { return enemyObj_.GetWorldPos(); };

	uint32_t GetCT() { return bulletCT_; };

	void OnCollision();

	Object3D enemyObj_;

	bool isAlive_ = true;
	bool isHit_ = false;

private:

	uint32_t bulletCT_ = 0;

	uint32_t bulletMaxCT_ = 200;

	AnimationModel* bulletModel_=nullptr;

	//ìñÇΩÇËîªíËÇÃèàóù
	MobCollision collision;

	//ìñÇΩÇËîªíËÇÃñ{ëÃ
	SphereCollider Collider;

	
};

