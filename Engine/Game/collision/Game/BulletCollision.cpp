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


//������
void BulletCollision::Initialize()
{
	collisionObj_.FBXInit();
}

//�I������
void BulletCollision::Finalize()
{

}

//���t���[���X�V
void BulletCollision::Update(const Vector3 pos)
{

	collisionObj_.SetPos(pos);
	collisionObj_.Update();
	
}

//�`��
void BulletCollision::Draw(AnimationModel* model)
{
	//�k���|�`�F�b�N
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