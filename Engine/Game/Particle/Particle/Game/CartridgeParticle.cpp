#include "CartridgeParticle.h"
#include "Easing.h"
#include "CollisionManager.h"

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

	actionTime_ = actionMaxTime;
}

void CartridgeParticle::Finalize()
{

}

void CartridgeParticle::Update()
{
	Velocity_.y -= moveSpeed_;

	//移動するところ
	obj_.Trans_ += Velocity_;
	if (obj_.Rotate_.x > 0)
	{
		obj_.Rotate_ -= {0.1f,0.1f,0.1f};
	}

	obj_.Update();

	if (liveTime_ > 0)
	{
		liveTime_--;
	}

	

	


}

void CartridgeParticle::Draw(AnimationModel* model)
{
	obj_.FBXDraw(*model);
}