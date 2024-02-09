#include "FallSpriteParticle.h"
#include "Easing.h"

FallSpriteParticle::FallSpriteParticle()
{

}

FallSpriteParticle::~FallSpriteParticle()
{

}

void FallSpriteParticle::Initialize()
{
	Sprite_.initialize("basketballMan");
}

void FallSpriteParticle::Initialize(const Vector2& position, const Vector2& velocity, float liveTime, float actionMaxTime, Vector2 startScale, Vector2 endScale, std::string textureName)
{
	Sprite_.initialize(textureName);
	//引数で受け取った初期座標をセット
	Sprite_.pos_ = position;

	Velocity_ = velocity;
	liveTime_ = liveTime;
	liveMaxTime_ = liveTime;

	actionMaxTime_ = actionMaxTime;

	startScale_ = startScale;
	endScale_ = endScale;
}

void FallSpriteParticle::Finalize()
{

}

void FallSpriteParticle::Update()
{
	//移動するところ
	Velocity_.y += gravity_;
	Sprite_.pos_ += Velocity_;
	Sprite_.rotate_ = DirectXpre::lerp(0.0f, 100.0f, actionTime_ / actionMaxTime_);

	Sprite_.Update();

	if (liveTime_ > 0)
	{
		liveTime_--;
	}

	if (actionTime_ <= actionMaxTime_)
	{
		actionTime_++;
	}


}

void FallSpriteParticle::Draw()
{
	Sprite_.Draw();
}