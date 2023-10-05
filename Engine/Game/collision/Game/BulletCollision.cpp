#include "BulletCollision.h"

BulletCollision::BulletCollision()
{
	tag_ = "Bullet";
}

BulletCollision::BulletCollision(std::string tag)
{
	tag_ = tag;
}

BulletCollision::~BulletCollision()
{
}


//初期化
void BulletCollision::Initialize()
{
	collisionObj_.FBXInit();
}

//終了処理
void BulletCollision::Finalize()
{

}

//毎フレーム更新
void BulletCollision::Update(const Vector3 pos)
{

	collisionObj_.SetPos(pos);
	collisionObj_.Update();
	
}

//描画
void BulletCollision::Draw(AnimationModel* model)
{
	//ヌルポチェック
	assert(model);
	if (isDraw_)
	{
		collisionObj_.FBXDraw(*model,false);
	}

}

void BulletCollision::OnCollision(const CollisionInfo& info)
{

	if ((info.object_->tag_ == "enemy") && tag_ == "playerBullet")
	{
		isHit = true;
	}
	else if ((info.object_->tag_ == "player") && tag_ == "enemyBullet")
	{
		isHit = true;
	}
	else if (info.object_->tag_ == "Bullet" && (tag_ == "playerBullet" || tag_ == "enemyBullet"))
	{
		isHit = true;
	}
	else if (info.object_->tag_ == "Wall" && (tag_ == "enemyBullet"))
	{
		isHit = true;
	}
	else if (info.object_->tag_ == "Wall" && tag_ == "Cartridge")
	{
		isHit = true;
	}
	else if (info.object_->tag_ == "")
	{
		isHit = true;
	}

}