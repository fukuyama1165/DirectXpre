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


//������
void MobCollision::Initialize()
{
	collisionObj_.FBXInit();
}

//�I������
void MobCollision::Finalize()
{

}

//���t���[���X�V
void MobCollision::Update(const Vector3 pos)
{

	collisionObj_.SetPos(pos);
	collisionObj_.Update();
	
}

//�`��
void MobCollision::Draw(AnimationModel* model)
{
	//�k���|�`�F�b�N
	assert(model);
	if (isDraw_)
	{
		collisionObj_.FBXDraw(*model, false);
	}

}

void MobCollision::OnCollision(const CollisionInfo& info)
{

	if (info.object_->collisionObj_.GetWorldPos().x >= 16777215 or info.object_->collisionObj_.GetWorldPos().x <= -16777215)return;

	if ((info.object_->tag_ == "enemyBullet" ) && tag_ == "player")
	{
		isHit = true;
	}
	else if ((info.object_->tag_ == "playerBullet") && tag_ == "enemy")
	{
		isHit = true;
	}
	else if (info.object_->tag_ == "")
	{
		isHit = true;
	}

}