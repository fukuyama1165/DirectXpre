#pragma once
#include "Object3D.h"
#include"Input.h"
#include "EnemyBullet.h"
#include <list>
class Enemy
{
public:
	Enemy();
	~Enemy();

	void Init();
	void Update(const Camera& camera);
	void Draw(AnimationModel* model);

	void Attack();

	Vector3 GetObjWorldPos() { return enemyObj_.GetWorldPos(); };

	uint32_t GetCT() { return bulletCT_; };

	Object3D enemyObj_;

	bool isAlive_ = true;
	bool isHit_ = false;

private:

	uint32_t bulletCT_ = 0;

	uint32_t bulletMaxCT_ = 100;

	AnimationModel* bulletModel_=nullptr;

	
};

