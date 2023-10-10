#pragma once
#include "FBXLoader.h"
#include "camera.h"
#include "IParticle.h"
#include "Vector3.h"
#include <string>
#include <memory>

class IObjEmitter
{
public:
	virtual ~IObjEmitter() {};

	//初期化
	virtual void Initialize() = 0;
	virtual void Initialize(const Vector3& pos, std::string particleType, const float& liveTime = 10, const float& ActiveTime = -1, const Vector2& randRengeX = { -100,100 }, const Vector2& randRengeY = { -100,100 }, const Vector2& randRengeZ = { -100,100 }, std::string particleModel = "whiteBox", std::string emitterModel = "whiteBox") = 0;

	//終了処理
	virtual void Finalize() = 0;

	//毎フレーム更新
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;

	virtual void SetIsActive(bool flag) = 0;

	virtual bool GetIsActive() = 0;

	virtual uint32_t GetParticleNum() = 0;

	virtual void SetCT(float ct) = 0;

private:

	bool isActive_ = true;

	std::unique_ptr<AnimationModel> particleModel_ = nullptr;

	float CT_ = 0;

	float maxCT_ = 10;
};