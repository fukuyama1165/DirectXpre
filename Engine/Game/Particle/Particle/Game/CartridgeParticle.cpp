#include "CartridgeParticle.h"
#include "Easing.h"
#include "CollisionManager.h"
#include <imgui.h>

CartridgeParticle::CartridgeParticle()
{

}

CartridgeParticle::~CartridgeParticle()
{

}

void CartridgeParticle::Initialize()
{
	obj_.FBXInit();

	
}

void CartridgeParticle::Initialize(const Vector3& position, const Vector3& velocity, float liveTime, float actionMaxTime, Vector3 startScale, Vector3 endScale)
{
	obj_.FBXInit();
	//引数で受け取った初期座標をセット
	obj_.Trans_ = position;
	obj_.Scale_ = startScale;

	Velocity_ = velocity;
	liveTime_ = liveTime;

	startScale_ = startScale;
	endScale_ = endScale;;

	actionMaxTime_ = actionMaxTime;
}

void CartridgeParticle::Finalize()
{

}

void CartridgeParticle::Update()
{
	//移動するところ
	Velocity_.y -= gravity_;
	obj_.Trans_ += Velocity_;
	//適当に回ってくれればいいので
	obj_.Rotate_ = lerp(Vector3(0, 0, 0), Vector3(100, 100, 100), actionTime_ / actionMaxTime_);
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

void CartridgeParticle::Draw(AnimationModel* model)
{
	obj_.FBXDraw(*model);
}