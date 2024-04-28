#include "Enemy.h"
#include "EmitterManager.h"
#include "XAudio.h"
#include "Easing.h"


Enemy::Enemy()
{
}
Enemy::~Enemy()
{
	CollisionManager::GetInstance()->RemoveCollider(&Collider);
}

void Enemy::Init(std::string enemyType, Vector3 pos, Vector3 movePointPos, float moveSpeed, uint32_t bulletCT, std::string DeathParticleName)
{
	
	enemyObj_.FBXInit();

	enemyObj_.Scale_ = { 0.5f,0.5f,0.5f };

	collision = MobCollision("enemy");

	Collider.SetObject(&collision);

	Collider.Initialize();

	Collider.size_ = { 1,1,1 };

	CollisionManager::GetInstance()->AddCollider(&Collider);

	enemyType_ = enemyType;

	enemyObj_.SetPos(pos);

	movePoint_ = movePointPos;

	moveStartPos_ = pos;

	speed_ = moveSpeed;

	bulletMaxCT_ = bulletCT;
	bulletCT_ = bulletCT;

	particleName_ = DeathParticleName;

	if (ParticleManager::GetInstance()->ParticleSearch(DeathParticleName))
	{
		isCustomizedparticle = true;
		particleData = ParticleManager::GetInstance()->GetParticleData(DeathParticleName);
	}

}

void Enemy::Update(std::string soundH)
{
	if (!isActive_)
	{
		enemyObj_.Scale_ = DirectXpre::easeOutQuint(Vector3{ 0,0,0 }, Vector3{ 0.5f,0.5f ,0.5f }, startTime_ / startMaxTime_);

		Collider.size_ = DirectXpre::easeOutQuint(Vector3{ 0,0,0 }, Vector3{ 1,1 ,1 } , startTime_ / startMaxTime_);

		if (startTime_ < startMaxTime_)
		{
			startTime_++;
		}
		else
		{
			isActive_ = true;
		}

		enemyObj_.Update();
		Collider.Update(enemyObj_.GetWorldPos());
	}


	if (isAlive_ && isActive_)
	{
		
		if (moveEnd_ == false && enemyType_ == EnemyType::Attack)
		{
			ori_ = enemyObj_.pos_;
			moveEnd_ = true;
		}
		

		if ((enemyType_ == EnemyType::moveAttack && moveEnd_ == false) || enemyType_ == EnemyType::moveOnly)
		{
			Move();
		}

		if (moveEnd_ == true || enemyType_ == EnemyType::Attack)
		{
			enemyObj_.pos_.x = ori_.x + sinf(sindou) * sindoubai;
		}

		enemyObj_.Update();

		if ((enemyType_ == EnemyType::moveAttack && moveEnd_ == true) || enemyType_ == EnemyType::Attack)
		{
			Attack();

			if (bulletCT_ <= sindouTime)
			{
				sindou+= sindouAdd;
			}

			if (bulletCT_ <= RedTime)
			{
				//赤色に
				enemyObj_.SetColor({ 1.0f,0.0f ,0.0f ,1.0f });
			}

		}

		

		Collider.Update(enemyObj_.GetWorldPos());

		if (collision.isHit)
		{
			OnCollision();
			XAudio::GetInstance()->PlaySoundData(soundH);
		}

		if (enemyType_ == EnemyType::moveOnly && moveEnd_)
		{
			isAlive_ = false;			
		}
		
	}
}

void Enemy::Draw(AnimationModel* model)
{
	//ヌルポチェック
	assert(model);
	enemyObj_.FBXDraw(*model);
}

void Enemy::Attack()
{

	//攻撃のタイミングになったら
	if (bulletCT_ <= 0)
	{
		//CTを設定し直す
		bulletCT_ = bulletMaxCT_;
		//振動をやめる
		sindou = 0;
		//もとの色に戻す
		enemyObj_.SetColor({ 1.0f,1.0f ,1.0f ,1.0f });
	}
	else
	{
		bulletCT_--;
	}

	
}

void Enemy::OnCollision()
{

	isAlive_ = false;

	CollisionManager::GetInstance()->RemoveCollider(&Collider);

	if (!isCustomizedparticle)
	{
		EmitterManager::GetInstance()->AddObjEmitter(enemyObj_.GetWorldPos(), "BASIC", particleName_, 10, 10, 20, 1.0f, { -1,1 }, { -1,1 }, { -1,1 }, { 0.5f,0.5f,0.5f }, { 0.2f,0.2f,0.2f });
	}
	else
	{
		EmitterManager::GetInstance()->AddObjEmitter(enemyObj_.GetWorldPos(), "BASIC", particleData);
	}

}

void Enemy::Move()
{

	//とりあえずプレイヤーと同じように動かす
	Vector3 moveVec = { 0,0,0 };
	moveVec = nainavec3(movePoint_, moveStartPos_).normalize();

	enemyObj_.pos_ += moveVec*speed_;

	//位置の情報で移動の為の大きさによってはたどり着けない場合があるので幅を持たせるため
	if (((enemyObj_.GetPos().x <= movePoint_.x + speed_) &&
		(enemyObj_.GetPos().x >= movePoint_.x - speed_)) &&
		((enemyObj_.GetPos().y <= movePoint_.y + speed_) &&
		(enemyObj_.GetPos().y >= movePoint_.y - speed_)) &&
		((enemyObj_.GetPos().z <= movePoint_.z + speed_) &&
		(enemyObj_.GetPos().z >= movePoint_.z - speed_)))
	{
		moveEnd_ = true;
		ori_ = enemyObj_.pos_;
	}


}