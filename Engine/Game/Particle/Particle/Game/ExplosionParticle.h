#pragma once
/**
 * @file ExplosionParticle.h
 * @brief 爆発用のパーティクルの処理部分
 * @author フクヤマ
 * @date 2023_12/29
 */

#include "IObjParticle.h"
#include "Object3D.h"

class ExplosionParticle :public IObjParticle
{
public:
	ExplosionParticle();
	~ExplosionParticle();

	//初期化
	void Initialize()override;
	void Initialize(const Vector3& position, const Vector3& velocity, float liveTime, float actionMaxTime, Vector3 startScale, Vector3 endScale)override;

	//終了処理
	void Finalize()override;

	//毎フレーム更新
	void Update()override;

	//描画
	void Draw(AnimationModel* model)override;

	Object3D GetObj()override { return obj_; };

	void SetliveTime(float time)override { liveTime_ = time; };

	float GetliveTime()override { return liveTime_; };

	void SetActionTime(float time)override { actionTime_ = time; };

	float GetActionTime()override { return actionTime_; };

	void SetObjColor(Vector4 color)override { obj_.SetColor(color); };


private:

	Object3D obj_;

	//速度
	Vector3	Velocity_;

	Vector3 startScale_ = { 1,1,1 };
	Vector3 endScale_ = { 1,1,1 };

	float liveTime_ = 50;

	float actionTime_ = 0;
	float actionMaxTime_ = 100;

	
	float exActionTime_ = 0;
	float exActionMaxTime_ = 100;

};