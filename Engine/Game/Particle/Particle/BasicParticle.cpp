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
	obj_.FBXInit();
}

void BasicParticle::Initialize(const Vector3& position, const Vector3& velocity,float liveTime, float actionMaxTime)
{
	obj_.FBXInit();
	//引数で受け取った初期座標をセット
	obj_.Trans_ = position;

	Velocity_ = velocity;
	liveTime_ = liveTime;
	liveMaxTime_ = liveTime;

	actionMaxTime_ = actionMaxTime;
	actionTime_ = actionMaxTime;
}

void BasicParticle::Finalize()
{

}

void BasicParticle::Update()
{
	//移動するところ
	obj_.Trans_ += Velocity_;
	obj_.Rotate_ = easeInQuint(Vector3(0,0,0), Vector3(100, 100, 100), actionTime_ / actionMaxTime_);
	obj_.Scale_ = easeInQuint(Vector3(0,0,0), Vector3(1, 1, 1), actionTime_ / actionMaxTime_);

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

void BasicParticle::Draw(AnimationModel* model)
{
	obj_.FBXDraw(*model);
}