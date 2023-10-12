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

void FallParticle::Initialize(const Vector3& position, const Vector3& velocity, float liveTime, float actionMaxTime)
{
	obj_.FBXInit();
	//引数で受け取った初期座標をセット
	obj_.Trans_ = position;

	Velocity_ = velocity;
	liveTime_ = liveTime;
	liveMaxTime_ = liveTime;

	actionTime_ = actionMaxTime;
	actionMaxTime_ = actionMaxTime;
}

void FallParticle::Finalize()
{

}

void FallParticle::Update()
{
	//移動するところ
	Velocity_.y -= gravity_;
	obj_.Trans_ += Velocity_;
	obj_.Rotate_ = easeInQuint(Vector3(0, 0, 0), Vector3(100, 100, 100), actionTime_ / actionMaxTime_);
	obj_.Scale_ = easeInQuint(Vector3(0, 0, 0), Vector3(1, 1, 1), actionTime_ / actionMaxTime_);

	obj_.Update();

	if (liveTime_ > 0)
	{
		liveTime_--;
	}

	if (actionTime_ > 0)
	{
		actionTime_--;
	}


}

void FallParticle::Draw(AnimationModel* model)
{
	obj_.FBXDraw(*model);
}