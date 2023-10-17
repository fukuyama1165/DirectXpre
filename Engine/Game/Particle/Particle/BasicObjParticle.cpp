#include "BasicObjParticle.h"
#include "Easing.h"

BasicObjParticle::BasicObjParticle()
{

}

BasicObjParticle::~BasicObjParticle()
{

}

void BasicObjParticle::Initialize()
{
	obj_.FBXInit();
}

void BasicObjParticle::Initialize(const Vector3& position, const Vector3& velocity,float liveTime, float actionMaxTime)
{
	obj_.FBXInit();
	//引数で受け取った初期座標をセット
	obj_.Trans_ = position;

	Velocity_ = velocity;
	liveTime_ = liveTime;
	liveMaxTime_ = liveTime;

	actionMaxTime_ = actionMaxTime;
}

void BasicObjParticle::Finalize()
{

}

void BasicObjParticle::Update()
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

	if (actionTime_ <= actionMaxTime_)
	{
		actionTime_++;
	}


}

void BasicObjParticle::Draw(AnimationModel* model)
{
	obj_.FBXDraw(*model);
}