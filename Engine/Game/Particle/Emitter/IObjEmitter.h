#pragma once

/**
 * @file IObjEmitter.h
 * @brief オブジェクトエミッターの根底クラス
 * @author フクヤマ
 * @date 2023_12/29
 */

#include "FBXLoader.h"
#include "camera.h"
#include "IParticle.h"
#include "Vector3.h"
#include "Object3D.h"
#include <string>
#include <memory>

class IObjEmitter
{
public:
	virtual ~IObjEmitter() {};

	//初期化
	virtual void Initialize() = 0;
	virtual void Initialize(const Vector3& pos, std::string particleType, const float& liveTime = 10, const float& actionMaxTime = 10, const float& ActiveTime = -1, float ct = 1, const Vector2& randRengeX = { -1,1 }, const Vector2& randRengeY = { -1,1 }, const Vector2& randRengeZ = { -1,1 }, Vector3 startScale = { 1,1,1 }, Vector3 endScale = { 1,1,1 }, std::string particleModel = "whiteBox", std::string emitterModel = "whiteBox") = 0;

	//終了処理
	virtual void Finalize() = 0;

	//毎フレーム更新
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;

	virtual void SetIsActive(bool flag) = 0;
	virtual void SetIsEnd(bool flag) = 0;

	virtual void SetPos(Vector3 pos) = 0;

	virtual bool GetIsActive() = 0;
	virtual bool GetIsEnd() = 0;

	virtual uint32_t GetParticleNum() = 0;

	virtual void SetCT(float ct) = 0;

	virtual void SetParticleLiveTime(float livetime) = 0;

	virtual void SetParticleType(std::string type) = 0;

	virtual void SetIsDraw(bool flag) = 0;

	virtual void SetActiveTime(float time) = 0;

	virtual void SetRandRangeX(Vector2 Range) = 0;
	virtual void SetRandRangeY(Vector2 Range) = 0;
	virtual void SetRandRangeZ(Vector2 Range) = 0;

	virtual void SetParticleactionTime(float time) = 0;

private:

	bool isActive_ = true;
	bool isEnd_ = false;

	std::unique_ptr<AnimationModel> particleModel_ = nullptr;

	float CT_ = 0;

	float maxCT_ = 10;

	float particleLiveTime_ = 10;

	std::string particleType_ = "";

	Object3D obj_;

	bool isDraw = false;

	float activeTime_ = 0;

	Vector2 randRangeX_ = { -1,1 };
	Vector2 randRangeY_ = { -1,1 };
	Vector2 randRangeZ_ = { -1,1 };

	float particleactionTime_ = 10;

};