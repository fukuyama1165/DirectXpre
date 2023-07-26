#pragma once
#include "FBXLoader.h"
#include "camera.h"
#include "IParticle.h"

class IObjEmitter
{
public:
	virtual ~IObjEmitter() {};

	//初期化
	virtual void Initialize() {};

	//終了処理
	virtual void Finalize() {};

	//毎フレーム更新
	virtual void Update(const Camera& camera) = 0;

	//描画
	virtual void Draw() {};

	virtual void SetIsActive(bool flag) { isActive_=flag; };

	virtual bool GetIsActive() { return isActive_; };


	virtual void SetCT(float ct) { maxCT_ = ct; };

private:

	bool isActive_ = true;

	std::unique_ptr<AnimationModel> particleModel_ = nullptr;

	float CT_ = 0;

	float maxCT_ = 10;
};