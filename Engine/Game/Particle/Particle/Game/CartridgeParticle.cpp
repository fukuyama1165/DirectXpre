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

	collision = BulletCollision("Cartridge");

	Collider.SetObject(&collision);

	Collider.Initialize();

	CollisionManager::GetInstance()->AddCollider(&Collider);
}

void CartridgeParticle::Initialize(const Vector3& position, const Vector3& velocity, float liveTime)
{
	obj_.FBXInit();
	//引数で受け取った初期座標をセット
	obj_.Trans_ = position;

	velocity;

	Vector3 velo(Util::Rand(-1.0f, 1.0f), Util::Rand(-5.0f, 5.0f), Util::Rand(-1.0f, 1.0f));

	velo.normalize();

	Velocity_ = velo/10;
	liveTime_ = liveTime;

	collision = BulletCollision("Cartridge");

	Collider.SetObject(&collision);

	Collider.Initialize();

	CollisionManager::GetInstance()->AddCollider(&Collider);
}

void CartridgeParticle::Finalize()
{

}

void CartridgeParticle::Update()
{
	moveSpeed_ -= Velocity_.y;

	//移動するところ
	obj_.Trans_.x += Velocity_.x;
	obj_.Trans_.x += moveSpeed_;
	obj_.Trans_.z += Velocity_.z;
	if (obj_.Rotate_.x > 0)
	{
		obj_.Rotate_ -= {0.1f,0.1f,0.1f};
	}

	obj_.Update();

	if (liveTime_ > 0)
	{
		liveTime_--;
	}

	if (liveTime_ <= 0)
	{
		CollisionManager::GetInstance()->RemoveCollider(&Collider);
	}

	Collider.Update(obj_.GetWorldPos());

	if (collision.isHit)
	{
		hitCount_++;
		moveSpeed_ = 10.0f/ hitCount_;
		collision.isHit = false;
	}


}

void CartridgeParticle::Draw(AnimationModel* model)
{
	obj_.FBXDraw(*model);
}