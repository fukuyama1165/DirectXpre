#pragma once
/**
 * @file EmitterManager.h
 * @brief エミッターを管理するクラス
 * @author フクヤマ
 * @date 2023_12/29
 */

#include "IEmitter.h"
#include "IObjEmitter.h"
#include "IEmitterFactory.h"


class EmitterManager
{
public:
	static EmitterManager* GetInstance();

	void AddObjEmitter(const Vector3& pos, std::string emitterType, std::string particleType, const float& liveTime, const float& actionTime = 10, const float& ActiveTime = -1, float ct = 1, const Vector2& randRengeX = { -1,1 }, const Vector2& randRengeY = { -1,1 }, const Vector2& randRengeZ = { -1,1 }, Vector3 startScale = { 1,1,1 }, Vector3 endScale = { 1,1,1 }, std::string particleModel = "whiteBox", std::string emitterModel = "whiteBox");
	void AddSpriteEmitter(const Vector2& pos, std::string emitterType, std::string particleType, const float& liveTime = 10, const float& actionTime = 10, const float& ActiveTime = -1, float ct = 1, const Vector2& randRengeX = { -1.0f,1.0f }, const Vector2& randRengeY = { -1.0f,1.0f }, Vector2 startScale = { 1,1 }, Vector2 endScale = { 1,1 }, std::string particleTexture = "basketballMan", std::string emitterTexture = "white1x1");

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

	//デバック用の生成したオブジェクトパーティクルの中身を編集する用(releaseでは中身は0)
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

	//デバック用のオブジェクトパーティクルの設定用変数
	float effectTestRandX[2] = { -1.0f,1.0f };
	float effectTestRandY[2] = { -1.0f,1.0f };
	float effectTestRandZ[2] = { -1.0f,1.0f };

	float particlePos[3] = { 0,0,0 };

	float effectTestliveTime = 1;
	float effectTestactionTime = 1;
	float emitterCt = 1;

	uint32_t emittrTypeNum = 0;
	uint32_t particleTypeNum = 0;

	//デバック用の生成したスプライトパーティクルの中身を編集する用(releaseでは中身は0)
	std::vector<Vector2> spritePos_;

	std::vector<float> spriteMaxCT_;
	std::vector<float> spriteParticleLiveTime_;
	std::vector<float> spriteActiveTime_;
	std::vector<float> spriteParticleActionTime_;

	std::vector<std::string> spriteParticleType_;
	std::vector<uint32_t> spriteParticleTypeNum_;

	std::vector<Vector2> spriteRandRangeX_;
	std::vector<Vector2> spriteRandRangeY_;

	//デバック用のスプライトパーティクルの設定用変数
	float spriteEffectTestRandX[2] = { -1.0f,1.0f };
	float spriteEffectTestRandY[2] = { -1.0f,1.0f };

	float spriteParticlePos[2] = { 0,0 };

	float spriteEffectTestliveTime = 1;
	float spriteEffectTestactionTime = 1;
	float spriteEmitterCt = 1;

	uint32_t spriteEmittrTypeNum = 0;
	uint32_t spriteParticleTypeNum = 1;

};

