#include "ExplosionParticle.h"
#include "Easing.h"
#include <imgui.h>

ExplosionParticle::ExplosionParticle()
{

}

ExplosionParticle::~ExplosionParticle()
{

}

void ExplosionParticle::Initialize()
{
	obj_.FBXInit();
	obj_.billboardMode_ = BillboardMode::YBillboard;


}

void ExplosionParticle::Initialize(const Vector3& position, const Vector3& velocity, float liveTime, float actionMaxTime, Vector3 startScale, Vector3 endScale)
{
	obj_.FBXInit();
	obj_.billboardMode_ = BillboardMode::YBillboard;
	//引数で受け取った初期座標をセット
	obj_.pos_ = position;
	obj_.Scale_ = startScale;

	Velocity_ = velocity;
	liveTime_ = liveTime;

	startScale_ = startScale;
	endScale_ = endScale;;

	actionMaxTime_ = actionMaxTime;
}

void ExplosionParticle::Finalize()
{

}

void ExplosionParticle::Update()
{
	if (actionTime_ <= actionMaxTime_)
	{

		obj_.Scale_ = DirectXpre::easeInQuint(startScale_, endScale_, actionTime_ / actionMaxTime_);

	}
	else
	{
		//最大の爆風の1.2倍大きくする
		obj_.Scale_ = DirectXpre::easeInQuint(endScale_, endScale_*1.2f, exActionTime_ / exActionMaxTime_);

	}

	obj_.Update();

	if (liveTime_ > 0 && exActionTime_ > exActionMaxTime_)
	{
		liveTime_--;
	}

	if (actionTime_ <= actionMaxTime_)
	{
		actionTime_++;
	}
	else if(actionTime_ > actionMaxTime_ && exActionTime_ <= exActionMaxTime_)
	{
		exActionTime_++;
	}

}

void ExplosionParticle::Draw(AnimationModel* model)
{
	obj_.FBXDraw(*model);
}