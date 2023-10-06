#pragma once
#include "FBXLoader.h"
#include "camera.h"
#include "Object3D.h"
#include "Vector4.h"

class IObjParticle
{
public:
	virtual ~IObjParticle() {};

	//初期化
	virtual void Initialize() {};
	virtual void Initialize(const Vector3& position, const Vector3& velocity, float liveTime) = 0;

	//終了処理
	virtual void Finalize() = 0;

	//毎フレーム更新
	virtual void Update() = 0;

	//描画
	virtual void Draw(AnimationModel* model) = 0;

	

	virtual void SetliveTime(float time) =0;

	virtual void SetActionTime(float time) =0;

	virtual float GetliveTime() = 0;

	virtual float GetActionTime() = 0;

	virtual void SetObjColor(Vector4 color) =0;


	virtual Object3D GetObj() = 0;

private:

	Object3D obj_;

	//速度
	Vector3	Velocity_;

	float liveTime_ = 50;
	float liveMaxTime_ = liveTime_;

	float actionTime_ = 50;
	float actionMaxTime_ = liveTime_;



};