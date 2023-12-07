#include "FallParticle.h"
#include "Easing.h"

FallParticle::FallParticle()
{

}

FallParticle::~FallParticle()
{

}

void FallParticle::Initialize()
{
	obj_.FBXInit();
}

void FallParticle::Initialize(const Vector3& position, const Vector3& velocity, float liveTime, float actionMaxTime, Vector3 startScale, Vector3 endScale)
{
	obj_.FBXInit();
	//引数で受け取った初期座標をセット
	obj_.pos_ = position;
	obj_.Scale_ = startScale;

	Velocity_ = velocity;
	liveTime_ = liveTime;
	liveMaxTime_ = liveTime;

	actionMaxTime_ = actionMaxTime;

	startScale_ = startScale;
	endScale_ = endScale;

}

void FallParticle::Finalize()
{

}

void FallParticle::Update()
{
	//移動するところ
	Velocity_.y -= gravity_;
	obj_.pos_ += Velocity_;
	obj_.Rotate_ = easeInQuint(Vector3(0, 0, 0), Vector3(100, 100, 100), actionTime_ / actionMaxTime_);
	obj_.Scale_ = easeInQuint(startScale_, endScale_, actionTime_ / actionMaxTime_);

	obj_.Update();

	if (liveTime_ > 0)
	{
		liveTime_--;
	}

	if (actionTime_ <= actionMaxTime_)
	{
		actionTime_++;
	}


}

void FallParticle::Draw(AnimationModel* model)
{
	obj_.FBXDraw(*model);
}