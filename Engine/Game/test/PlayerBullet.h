#pragma once
#include "Object3D.h"
#include "Texture.h"
#include "camera.h"
#include "FBXLoader.h"


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
	/// <param name="model">モデル</param>
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
	void Draw(AnimationModel* model);

	bool IsDead()const { return isDead_; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	Vector3 GetWorldPosition();

private:

	//弾の本体
	Object3D obj_;
	

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	

	//速度
	Vector3	Velocity_;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 100;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;

};

