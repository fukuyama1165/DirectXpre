#pragma once
#include "Object3D.h"
#include"Input.h"
#include "EnemyBullet.h"
#include <list>
#include "MobCollision.h"
#include "CubeCollider.h"

class EnemyType
{
public:
	static inline std::string moveOnly = "moveOnly";
	static inline std::string moveAttack = "moveAttack";
	static inline std::string Attack = "Attack";
};

class Enemy
{
public:
	Enemy();
	~Enemy();

	void Init(std::string enemyType, Vector3 pos, Vector3 movePointPos, float moveSpeed, uint32_t bulletCT = 200);
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

	int32_t bulletCT_ = 200;

	int32_t bulletMaxCT_ = 200;

	int32_t sindouTime = 10;
	int32_t RedTime = 5;

	float sindouAdd = 1.0f;
	float sindoubai = 1.0f;

	float sindou = 0;

	AnimationModel* bulletModel_=nullptr;

	//当たり判定の処理
	MobCollision collision;

	//当たり判定の本体
	CubeCollider Collider;

	std::string enemyType_ = EnemyType::moveOnly;

	bool moveEnd_ = false;

	Vector3 moveStartPos_ = { 0,0,0 };

	Vector3 movePoint_ = { 0,0,0 };

	Vector3 ori_ = { 0,0,0 };

	float speed_ = 1.0f;
	
};

