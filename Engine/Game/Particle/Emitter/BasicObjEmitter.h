#pragma once
#include "IObjEmitter.h"
#include "ParticleFactory.h"
#include <list>

class BasicObjEmitter:public IObjEmitter
{
public:
	BasicObjEmitter();
	~BasicObjEmitter();

	//初期化
	void Initialize()override;
	void Initialize(const Vector3& pos, std::string particleType, const float& liveTime = 10, const float& actionMaxTime = 10, const float& ActiveTime = -1, float ct = 1, const Vector2& randRengeX = { -1,1 }, const Vector2& randRengeY = { -1,1 }, const Vector2& randRengeZ = { -1,1 }, Vector3 startScale = { 1,1,1 }, Vector3 endScale = { 1,1,1 }, std::string particleModel = "whiteBox", std::string emitterModel = "whiteBox")override;

	//終了処理
	void Finalize()override;

	//毎フレーム更新
	void Update()override;

	//描画
	void Draw()override;

	void SetIsActive(bool flag)override { isActive_ = flag; };
	void SetIsEnd(bool flag)override { isEnd_ = flag; };


	void SetPos(Vector3 pos)override { obj_.Trans_ = pos; };

	bool GetIsActive()override { return isActive_; };
	bool GetIsEnd()override { return isEnd_; };

	uint32_t GetParticleNum()override { return (uint32_t)particles_.size(); };

	void SetCT(float ct)override { maxCT_ = ct; };

	void SetParticleLiveTime(float livetime)override { particleLiveTime_ = livetime; };

	void SetParticleType(std::string type)override { particleType_ = type; };

	void SetIsDraw(bool flag)override { isDraw = flag; };

	void SetActiveTime(float time)override { activeTime_ = time; };

	void SetRandRangeX(Vector2 Range)override { randRangeX_ = Range; };
	void SetRandRangeY(Vector2 Range)override { randRangeY_ = Range; };
	void SetRandRangeZ(Vector2 Range)override { randRangeZ_ = Range; };

	void SetParticleactionTime(float time)override { particleactionTime_ = time; };

private:

	bool isActive_ = true;
	bool isEnd_ = false;

	AnimationModel* particleModel_ = nullptr;

	std::list<std::unique_ptr<IObjParticle>> particles_;

	float particleLiveTime_ = 10;

	float CT_ = 0;

	float maxCT_ = 1;

	Object3D obj_;

	AnimationModel* emitterModel_ = nullptr;

	bool isDraw = false;

	float activeTime_ = -1;

	std::unique_ptr<ParticleFactory> particleFactory_ = nullptr;

	std::string particleType_ = "";

	Vector2 randRangeX_ = { -1,1 };
	Vector2 randRangeY_ = { -1,1 };
	Vector2 randRangeZ_ = { -1,1 };

	Vector3 startScale_ = { 1,1,1 };
	Vector3 endScale_ = { 1,1,1 };

	float particleactionTime_ = 10;

};

