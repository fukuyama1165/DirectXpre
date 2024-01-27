#pragma once

/**
 * @file PlayerBullet.h
 * @brief 自キャラの弾のクラス
 * @author フクヤマ
 * @date 2023_11/10
 */

#include "Object3D.h"
#include "BulletCollision.h"
#include "SphereCollider.h"
#include "CubeCollider.h"

/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet
{
public:
	PlayerBullet();
	~PlayerBullet();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="size">大きさ</param>
	void Initlize(const Vector3& position,const Vector3& velocity, const Vector3& size={1.0f,1.0f,1.0f });

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="model">モデル</param>
	void Draw(AnimationModel* model);

	bool IsDead()const { return isDead_; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	Vector3 GetWorldPosition();

private:

	//弾の本体
	Object3D obj_;
	
	//速度
	Vector3	Velocity_;

	//当たり判定の処理
	BulletCollision collision;

	//当たり判定の本体
	CubeCollider Collider;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;

};

