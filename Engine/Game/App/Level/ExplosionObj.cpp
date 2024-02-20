#include "ExplosionObj.h"
#include "XAudio.h"
#include "EmitterManager.h"
#include "Easing.h"

ExplosionObj::ExplosionObj()
{
}

ExplosionObj::~ExplosionObj()
{
	CollisionManager::GetInstance()->RemoveCollider(&Collider);
}

void ExplosionObj::Init(Vector3 pos, int32_t explosioneventNum, Vector3 size, Vector3 explosionSize, float explosionTime)
{

	obj_.FBXInit();

	collision = MobCollision("explosionObj");

	Collider.SetObject(&collision);

	Collider.Initialize();

	Collider.size_ = size;

	Collider.isActive_ = false;

	CollisionManager::GetInstance()->AddCollider(&Collider);

	obj_.SetPos(pos);

	
	explosionEventNum_ = explosioneventNum;

	size_ = size;
	explosionSize_ = explosionSize;

	explosionTime_ = explosionTime;

}

void ExplosionObj::Update(std::string soundH, int32_t eventNum)
{
	if (isAlive_ && !isExplosion_)
	{
		if (eventNum == explosionEventNum_)
		{
			Collider.isActive_ = true;	
		}

		obj_.Update();

		Collider.Update(obj_.GetWorldPos());

		if (collision.isHit)
		{
			OnCollision();
			XAudio::GetInstance()->PlaySoundData(soundH);
			
		}

	}
	else if(isAlive_)
	{

		//obj_.Scale_ = DirectXpre::easeOutCirc(size_, size_ + explosionSize_, explosionTimeBuff_ / explosionTime_);
		Collider.size_ = DirectXpre::easeOutCirc(size_, size_ + explosionSize_, explosionTimeBuff_ / explosionTime_);

		if (explosionTimeBuff_ < explosionTime_)
		{
			explosionTimeBuff_++;
		}
		else
		{
			isAlive_ = false;
			CollisionManager::GetInstance()->RemoveCollider(&Collider);
		}

		obj_.Update();
		Collider.Update(obj_.GetWorldPos());
	}
	else
	{
		obj_.Update();
	}
}

void ExplosionObj::Draw(AnimationModel* model)
{
	//ヌルポチェック
	assert(model);
	if (!isExplosion_)
	{
		obj_.FBXDraw(*model);
	}
}

void ExplosionObj::OnCollision()
{

	//isAlive_ = false;

	//CollisionManager::GetInstance()->RemoveCollider(&Collider);
	//爆発するオブジェクトから爆発の判定に変更
	collision.tag_ = "explosion";

	//仕様上当たったかフラグは戻ることがない状態なので戻しておく
	collision.isHit = false;

	isExplosion_ = true;

	explosionTimeBuff_ = 0;

	EmitterManager::GetInstance()->AddObjEmitter(obj_.GetWorldPos(), "BASIC", "Explosion", 10, 10, 10, 1.0f, { -1,1 }, { 0,1 }, { -1,1 }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f });

}