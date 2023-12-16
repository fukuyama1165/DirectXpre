#include "Enemy.h"
#include "EmitterManager.h"
#include "XAudio.h"

Enemy::Enemy()
{
}
Enemy::~Enemy()
{
	CollisionManager::GetInstance()->RemoveCollider(&Collider);
}

void Enemy::Init(std::string enemyType, Vector3 pos, Vector3 movePointPos, float moveSpeed, uint32_t bulletCT)
{
	
	enemyObj_.FBXInit();

	collision = MobCollision("enemy");

	Collider.SetObject(&collision);

	Collider.Initialize();

	Collider.size_ = { 2,2,2 };

	CollisionManager::GetInstance()->AddCollider(&Collider);

	enemyType_ = enemyType;

	enemyObj_.SetPos(pos);

	movePoint_ = movePointPos;

	moveStartPos_ = pos;

	speed_ = moveSpeed;

	bulletMaxCT_ = bulletCT;
	bulletCT_ = bulletCT;

}

void Enemy::Update(std::string soundH)
{
	if (isAlive_)
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


	if (bulletCT_ <= 0)
	{
		bulletCT_ = bulletMaxCT_;
		sindou = 0;
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

	EmitterManager::GetInstance()->AddObjEmitter(enemyObj_.GetWorldPos(), "BASIC", "Fall", 10,10, 20);

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