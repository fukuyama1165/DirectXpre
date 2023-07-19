#pragma once
#include "Object3D.h"
#include "BulletCollision.h"
#include "SphereCollider.h"

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
	void Initlize(const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Camera& camera);

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
	SphereCollider Collider;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;

};

