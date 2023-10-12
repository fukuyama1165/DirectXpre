#pragma once
#include "IEmitter.h"
#include "IObjEmitter.h"
#include "IEmitterFactory.h"


class EmitterManager
{
public:
	static EmitterManager* GetInstance();

	void AddObjEmitter(const Vector3& pos, std::string emitterType, std::string particleType, const float& liveTime, const float& actionTime = 10, const float& ActiveTime = -1, const Vector2& randRengeX = { -1,1 }, const Vector2& randRengeY = { -1,1 }, const Vector2& randRengeZ = { -1,1 }, std::string particleModel = "whiteBox", std::string emitterModel = "whiteBox");

	//中身全部消す
	void reset();

	//毎フレーム更新
	void Update();

	//描画
	void Draw();

private:

	EmitterManager() = default;
	~EmitterManager();

	EmitterManager(const EmitterManager&) = delete;
	EmitterManager& operator=(const EmitterManager&) = delete;


private:
	std::list<std::unique_ptr<IObjEmitter>> objEmitters_;
	std::list<std::unique_ptr<IEmitter>> emitters_;

	std::unique_ptr<IEmitterFactory> emitterFactory_ = nullptr;

	std::vector<Vector3> pos_;

	std::vector<float> maxCT_;
	std::vector<float> particleLiveTime_;
	std::vector<float> activeTime_;
	std::vector<float> particleActionTime_;

	std::vector<std::string> particleType_;
	std::vector<uint32_t> particleTypeNum_;

	std::vector<Vector2> randRangeX_;
	std::vector<Vector2> randRangeY_;
	std::vector<Vector2> randRangeZ_;


	float effectTestRandX[2] = { -10,10 };
	float effectTestRandY[2] = { 0.0f,1.0f };
	float effectTestRandZ[2] = { -10,10 };

	float particlePos[3] = { 0,0,0 };

	float effectTestliveTime = 1;
	float effectTestactionTime = 1;

	uint32_t emittrTypeNum = 0;
	uint32_t particleTypeNum = 0;

};

