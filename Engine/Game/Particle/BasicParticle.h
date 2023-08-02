#pragma once
#include "IObjParticle.h"
#include "Object3D.h"


class BasicParticle
{
public:
	BasicParticle();
	~BasicParticle();

	//初期化
	void Initialize();
	void Initialize(const Vector3& position, const Vector3& velocity,float liveTime);

	//終了処理
	void Finalize();

	//毎フレーム更新
	void Update();

	//描画
	void Draw(AnimationModel* model);

	Object3D GetObj() { return obj_; };

	void SetliveTime(float time) { liveTime_ = time; liveMaxTime_ = time; };

	float GetliveTime() { return liveTime_; };

private:

	Object3D obj_;

	//速度
	Vector3	Velocity_;

	float liveTime_ = 50;
	float liveMaxTime_ = liveTime_;

};