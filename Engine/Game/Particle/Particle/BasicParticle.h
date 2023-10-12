#pragma once
#include "IObjParticle.h"
#include "Object3D.h"


class BasicParticle:public IObjParticle
{
public:
	BasicParticle();
	~BasicParticle();

	//初期化
	void Initialize()override;
	void Initialize(const Vector3& position, const Vector3& velocity,float liveTime, float actionMaxTime)override;

	//終了処理
	void Finalize()override;

	//毎フレーム更新
	void Update()override;

	//描画
	void Draw(AnimationModel* model)override;

	Object3D GetObj()override { return obj_; };

	void SetliveTime(float time)override { liveTime_ = time; liveMaxTime_ = time; };

	float GetliveTime()override { return liveTime_; };

	void SetActionTime(float time)override { actionMaxTime_ = time; };

	float GetActionTime()override { return actionTime_; };

	void SetObjColor(Vector4 color)override { obj_.SetColor(color); };


private:

	Object3D obj_;

	//速度
	Vector3	Velocity_;

	float liveTime_ = 50;
	float liveMaxTime_ = liveTime_;

	float actionTime_ = 100;
	float actionMaxTime_ = actionTime_;

};