#pragma once
#include "IParticle.h"

class BasicParticle :public IParticle
{
public:
	BasicParticle();
	~BasicParticle();

	//初期化
	void Initialize();
	void Initialize(const Vector2& position, const Vector2& velocity, float liveTime, float actionMaxTime, Vector2 startScale, Vector2 endScale, std::string textureName);

	//終了処理
	void Finalize();

	//毎フレーム更新
	void Update();

	//描画
	void Draw();

	Sprite GetSprite()override { return Sprite_; };

	void SetliveTime(float time)override { liveTime_ = time; liveMaxTime_ = time; };

	float GetliveTime()override { return liveTime_; };

	void SetActionTime(float time)override { actionMaxTime_ = time; };

	float GetActionTime()override { return actionTime_; };

	void SetObjColor(Vector4 color)override { Sprite_.setColor(color); };

	void SetStartScale(Vector2 scale)override { startScale_ = scale; };
	void SetEndScale(Vector2 scale)override { endScale_ = scale; };

	Vector2 GetStartScale()override { return startScale_; };
	Vector2 GetEndScale()override { return endScale_; };


private:

	Sprite Sprite_;

	//速度
	Vector2	Velocity_;

	Vector2 startScale_;
	Vector2 endScale_;

	float liveTime_ = 50;
	float liveMaxTime_ = liveTime_;

	float actionTime_ = 0;
	float actionMaxTime_ = 100;

};