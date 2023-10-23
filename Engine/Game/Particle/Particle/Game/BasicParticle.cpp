#include "BasicParticle.h"
#include "Easing.h"

BasicParticle::BasicParticle()
{

}

BasicParticle::~BasicParticle()
{

}

void BasicParticle::Initialize()
{
	Sprite_.initialize("basketballMan");
}

void BasicParticle::Initialize(const Vector2& position, const Vector2& velocity, float liveTime, float actionMaxTime, Vector2 startScale, Vector2 endScale, std::string textureName)
{
	Sprite_.initialize(textureName);
	//引数で受け取った初期座標をセット
	Sprite_.pos_ = position;
	Sprite_.scale_ = startScale;

	Velocity_ = velocity;
	liveTime_ = liveTime;
	liveMaxTime_ = liveTime;

	actionMaxTime_ = actionMaxTime;

	startScale_ = startScale;
	endScale_ = endScale;

}

void BasicParticle::Finalize()
{

}

void BasicParticle::Update()
{
	//移動するところ
	Sprite_.pos_ += Velocity_;
	Sprite_.rotate_ = easeInQuint(0.0f, 100.0f, actionTime_ / actionMaxTime_);
	Sprite_.scale_ = easeInQuint(startScale_, endScale_, actionTime_ / actionMaxTime_);

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

void BasicParticle::Draw()
{
	Sprite_.Draw();
}