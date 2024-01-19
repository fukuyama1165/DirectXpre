#pragma once
#include "Object3D.h"
#include "MobCollision.h"
#include "CubeCollider.h"

class ExplosionObj
{
public:
	ExplosionObj();
	~ExplosionObj();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos">位置</param>
	/// /// <param name="explosioneventNum">爆発イベントを起こすイベント位置(何個目のイベントか)</param>
	/// <param name="size">当たり判定の大きさ</param>
	/// <param name="explosionSize">爆発範囲</param>
	/// <param name="explosionTime">爆発時間</param>
	void Init(Vector3 pos, int32_t explosioneventNum, Vector3 size = { 1,1,1 }, Vector3 explosionSize = { 1,1,1 }, float explosionTime = 1.0f);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="soundH">死んだときの音のハンドル</param>
	/// <param name="eventNum">現在のイベント数(位置)</param>
	void Update(std::string soundH,int32_t eventNum);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="model">描画用モデル</param>
	void Draw(AnimationModel* model);

	Vector3 GetObjWorldPos() { return obj_.GetWorldPos(); };

	

	//当たった時の処理
	void OnCollision();

	//本体
	Object3D obj_;

	//生きているか
	bool isAlive_ = true;

	//当たったか
	bool isHit_ = false;

	//爆発したか
	bool isExplosion_ = false;

private:

	//当たり判定の処理
	MobCollision collision;

	//当たり判定の本体
	CubeCollider Collider;

	//爆発するイベント
	int32_t explosionEventNum_ = 0;

	//当たり判定の大きさ
	Vector3 size_ = { 1,1,1 };
	//爆発範囲
	Vector3 explosionSize_ = { 1,1,1 };

	//爆発時間
	float explosionTime_ = 1.0f;

	float explosionTimeBuff_ = 1.0f;

};

