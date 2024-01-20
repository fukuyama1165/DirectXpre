#include "MobCollision.h"

MobCollision::MobCollision()
{
	tag_ = "Mob";
}

MobCollision::MobCollision(std::string tag)
{
	tag_ = tag;
}

MobCollision::~MobCollision()
{
}


//初期化
void MobCollision::Initialize()
{
	collisionObj_.FBXInit();
}

//終了処理
void MobCollision::Finalize()
{

}

//毎フレーム更新
void MobCollision::Update(const Vector3 pos)
{

	collisionObj_.SetPos(pos);
	collisionObj_.Update();
	
}

//描画
void MobCollision::Draw(AnimationModel* model)
{
	//ヌルポチェック
	assert(model);
	if (isDraw_)
	{
		collisionObj_.FBXDraw(*model, false);
	}

}

void MobCollision::OnCollision(const CollisionInfo& info)
{
	//とんでもない所で当たることは無いのでそもそも除外
	if (info.object_->collisionObj_.GetWorldPos().x >= 16777215 || info.object_->collisionObj_.GetWorldPos().x <= -16777215)return;

	if ((info.object_->tag_ == "enemyBullet" ) && tag_ == "player")
	{
		isHit = true;
	}
	else if ((info.object_->tag_ == "playerBullet") && tag_ == "enemy")
	{
		isHit = true;
	}
	else if ((info.object_->tag_ == "playerBullet") && tag_ == "explosionObj")
	{
		isHit = true;
	}
	else if ((info.object_->tag_ == "explosion") && tag_ == "enemy")
	{
		isHit = true;
	}
	else if (info.object_->tag_ == "")
	{
		isHit = true;
	}

}