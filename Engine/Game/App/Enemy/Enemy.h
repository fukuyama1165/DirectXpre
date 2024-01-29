#pragma once

/**
 * @file Enemy.h
 * @brief ゲーム内の敵
 * @author フクヤマ
 * @date 2023_11/10
 */

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

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="enemyType">種類</param>
	/// <param name="pos">生み出される位置</param>
	/// <param name="movePointPos">移動先</param>
	/// <param name="moveSpeed">移動速度</param>
	/// <param name="bulletCT">発射間隔</param>
	void Init(std::string enemyType, Vector3 pos, Vector3 movePointPos, float moveSpeed, uint32_t bulletCT = 200);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="soundH">死んだときの音のハンドル</param>
	void Update(std::string soundH);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="model">描画用モデル</param>
	void Draw(AnimationModel* model);

	//移動処理
	void Move();

	//攻撃処理
	void Attack();

	Vector3 GetObjWorldPos() { return enemyObj_.GetWorldPos(); };

	uint32_t GetCT() { return bulletCT_; };

	//当たった時の処理
	void OnCollision();

	//本体
	Object3D enemyObj_;

	//生きているか
	bool isAlive_ = true;

	//当たったか
	bool isHit_ = false;

private:

	//弾の発射間隔を計測用の変数
	int32_t bulletCT_ = 200;

	//発射間隔の待ち時間
	int32_t bulletMaxCT_ = 200;

	//揺れる時間
	int32_t sindouTime = 10;

	//赤くなる開始時間
	int32_t RedTime = 5;

	//振動の加算値(速度)
	float sindouAdd = 1.0f;

	//振動の大きさ
	float sindoubai = 1.0f;

	//振動の値
	float sindou = 0;

	//当たり判定の処理
	MobCollision collision;

	//当たり判定の本体
	CubeCollider Collider;

	//種類
	std::string enemyType_ = EnemyType::moveOnly;

	//移動終わりか
	bool moveEnd_ = false;

	//移動開始位置
	Vector3 moveStartPos_ = { 0,0,0 };

	//移動位置
	Vector3 movePoint_ = { 0,0,0 };

	//実際の位置(振動に使用)
	Vector3 ori_ = { 0,0,0 };

	//移動速度
	float speed_ = 1.0f;
	
};

