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

void BasicParticle::Initialize(const Vector3& position, const Vector3& velocity,float liveTime)
{
	obj_.FBXInit();
	//引数で受け取った初期座標をセット
	obj_.Trans_ = position;

	Velocity_ = velocity;
	liveTime_ = liveTime;
	liveMaxTime_ = liveTime;
}

void BasicParticle::Finalize()
{

}

void BasicParticle::Update(const Camera& camera)
{
	//移動するところ
	obj_.Trans_ += Velocity_;
	obj_.Scale_ = easeInQuint(Vector3(0,0,0), Vector3(1, 1, 1), liveTime_ / liveMaxTime_);

	obj_.Update(camera);

	if (liveTime_ > 0)
	{
		liveTime_--;
	}

}

void BasicParticle::Draw(AnimationModel* model)
{
	obj_.FBXDraw(*model);
}