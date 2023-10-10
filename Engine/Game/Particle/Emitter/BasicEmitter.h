#pragma once
#include "IObjEmitter.h"
#include "BasicParticle.h"
#include "ObjParticleFactory.h"
#include <list>

class BasicEmitter:public IObjEmitter
{
public:
	BasicEmitter();
	~BasicEmitter();

	//初期化
	void Initialize()override;
	void Initialize(const Vector3& pos, std::string particleType, const float& liveTime = 10, const float& ActiveTime = -1, const Vector2& randRengeX = { -100,100 }, const Vector2& randRengeY = { -100,100 }, const Vector2& randRengeZ = { -100,100 }, std::string particleModel = "whiteBox", std::string emitterModel = "whiteBox")override;

	//終了処理
	void Finalize()override;

	//毎フレーム更新
	void Update()override;

	//描画
	void Draw()override;

	void SetIsActive(bool flag)override { isActive_ = flag; };

	bool GetIsActive()override { return isActive_; };

	uint32_t GetParticleNum()override { return (uint32_t)particles_.size(); };

	void SetCT(float ct)override { maxCT_ = ct; };

	void SetParticleLiveTime(float livetime){ particleLiveTime_ = livetime; };

	void SetIsDraw(bool flag){ isDraw = flag; };

private:

	bool isActive_ = true;

	AnimationModel* particleModel_ = nullptr;

	std::list<std::unique_ptr<IObjParticle>> particles_;

	float particleLiveTime_ = 10;

	float CT_ = 0;

	float maxCT_ = 1;

	Object3D obj_;

	AnimationModel* emitterModel_ = nullptr;

	bool isDraw = false;

	float activeTime_ = 0;

	std::unique_ptr<ObjParticleFactory> particleFactory_ = nullptr;

	std::string particleType_ = "";

	Vector2 randRangeX_ = { -100,100 };
	Vector2 randRangeY_ = { -100,100 };
	Vector2 randRangeZ_ = { -100,100 };

};

