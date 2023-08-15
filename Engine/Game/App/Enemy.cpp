#include "Enemy.h"
#include "EmitterManager.h"

Enemy::Enemy()
{
}
Enemy::~Enemy()
{

}

void Enemy::Init(uint16_t enemyType, Vector3 pos, Vector3 movePointPos)
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

}

void Enemy::Update()
{
	if (isAlive_)
	{
		

		

		if ((enemyType_ == EnemyType::moveAttack and moveEnd_ == false) or enemyType_ == EnemyType::moveOnly)
		{
			Move();
		}

		enemyObj_.Update();

		if ((enemyType_ == EnemyType::moveAttack and moveEnd_ == true) or enemyType_ == EnemyType::Attack)
		{
			Attack();
		}

		

		Collider.Update(enemyObj_.GetWorldPos());

		if (collision.isHit)
		{
			OnCollision();
		}

		if (enemyType_ == moveOnly and moveEnd_)
		{
			isAlive_ = false;

			CollisionManager::GetInstance()->RemoveCollider(&Collider);
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

	EmitterManager::GetInstance()->AddObjEmitter(enemyObj_.GetWorldPos(),"BASIC", "BASIC", 20);

}

void Enemy::Move()
{

	//とりあえずプレイヤーと同じように動かす
	Vector3 moveVec = { 0,0,0 };
	moveVec = nainavec3(movePoint_, moveStartPos_).normalize();

	enemyObj_.Trans_ += moveVec;

	//位置の情報で移動の為の大きさによってはたどり着けない場合があるので幅を持たせるため
	if (((enemyObj_.GetPos().x <= movePoint_.x + 1) and 
		(enemyObj_.GetPos().x >= movePoint_.x - 1)) and 
		((enemyObj_.GetPos().y <= movePoint_.y + 1) and
		(enemyObj_.GetPos().y >= movePoint_.y - 1)) and
		((enemyObj_.GetPos().z <= movePoint_.z + 1) and
		(enemyObj_.GetPos().z >= movePoint_.z - 1)))
	{
		moveEnd_ = true;
	}


}