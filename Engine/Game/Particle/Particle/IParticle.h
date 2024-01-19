#pragma once

/**
 * @file IParticle.h
 * @brief スプライトパーティクルの基底クラス
 * @author フクヤマ
 * @date 2023_12/29
 */

#include "Sprite.h"

class IParticle
{
public:
	virtual ~IParticle() = default;

	//初期化
	virtual void Initialize() {};
	virtual void Initialize(const Vector2& position, const Vector2& velocity, float liveTime, float actionMaxTime, Vector2 startScale, Vector2 endScale,std::string textureName) = 0;

	//終了処理
	virtual void Finalize() = 0;

	//毎フレーム更新
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;



	virtual void SetliveTime(float time) = 0;

	virtual void SetActionTime(float time) = 0;

	virtual void SetStartScale(Vector2 scale) = 0;
	virtual void SetEndScale(Vector2 scale) = 0;

	virtual float GetliveTime() = 0;

	virtual float GetActionTime() = 0;

	virtual Vector2 GetStartScale() = 0;
	virtual Vector2 GetEndScale() = 0;

	virtual void SetObjColor(Vector4 color) = 0;


	virtual Sprite GetSprite() = 0;

private:

	Sprite Sprite_;

	//速度
	Vector2	Velocity_;

	Vector2 startScale_;
	Vector2 endScale_;

	float liveTime_ = 50;
	float liveMaxTime_ = liveTime_;

	float actionTime_ = 50;
	float actionMaxTime_ = liveTime_;
};

