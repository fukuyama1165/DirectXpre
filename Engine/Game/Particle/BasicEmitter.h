#pragma once
#include "IObjEmitter.h"
#include "BasicParticle.h"
#include <list>

class BasicEmitter:public IObjEmitter
{
public:
	BasicEmitter();
	~BasicEmitter();

	//初期化
	void Initialize(const Vector3& pos, float ActiveTime = -1);

	//終了処理
	void Finalize();

	//毎フレーム更新
	void Update(const Camera& camera);

	//描画
	void Draw();

	void SetIsActive(bool flag) { isActive_ = flag; };

	bool GetIsActive() { return isActive_; };

	uint32_t GetParticleNum() { return (uint32_t)particles_.size(); };

	void SetCT(float ct) { maxCT_ = ct; };

	void SetParticleLiveTime(float livetime) { particleLiveTime_ = livetime; };

	void SetIsDraw(bool flag) { isDraw = flag; };

private:

	bool isActive_ = true;

	std::unique_ptr<AnimationModel> particleModel_ = nullptr;

	std::list<std::unique_ptr<BasicParticle>> particles_;

	float particleLiveTime_ = 10;

	float CT_ = 0;

	float maxCT_ = 1;

	Object3D obj_;

	std::unique_ptr<AnimationModel> emitterModel_ = nullptr;

	bool isDraw = false;

	float activeTime_ = 0;

};

