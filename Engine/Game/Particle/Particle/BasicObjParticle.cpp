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

void BasicObjParticle::Initialize(const Vector3& position, const Vector3& velocity,float liveTime, float actionMaxTime, Vector3 startScale, Vector3 endScale)
{
	obj_.FBXInit();
	//引数で受け取った初期座標をセット
	obj_.Trans_ = position;
	obj_.Scale_ = startScale;

	Velocity_ = velocity;
	liveTime_ = liveTime;
	liveMaxTime_ = liveTime;

	actionMaxTime_ = actionMaxTime;

	startScale_ = startScale;
	endScale_ = endScale;
}

void BasicObjParticle::Finalize()
{

}

void BasicObjParticle::Update()
{
	//移動するところ
	obj_.Trans_ += Velocity_;
	obj_.Rotate_ = easeInQuint(Vector3(0,0,0), Vector3(100, 100, 100), actionTime_ / actionMaxTime_);
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

void BasicObjParticle::Draw(AnimationModel* model)
{
	obj_.FBXDraw(*model);
}