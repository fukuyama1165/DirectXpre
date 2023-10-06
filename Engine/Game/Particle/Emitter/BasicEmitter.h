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
	void Initialize(const Vector3& pos,std::string particleType, std::string particleModel = "whiteBox", std::string emitterModel = "whiteBox",  float ActiveTime = -1)override;

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

};

