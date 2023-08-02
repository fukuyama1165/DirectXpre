#include "PlayerBullet.h"
#include <cassert>
#include "CollisionManager.h"

PlayerBullet::PlayerBullet()
{
	
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initlize(const Vector3& position, const Vector3& velocity)
{

	
	obj_.FBXInit();		

	//�����Ŏ󂯎�����������W���Z�b�g
	obj_.Trans_ = position;

	Velocity_ = velocity;

	collision = BulletCollision("playerBullet");

	

	Collider.SetObject(&collision);

	Collider.Initialize();

	CollisionManager::GetInstance()->AddCollider(&Collider);

}

void PlayerBullet::Update()
{
	//�ړ�����Ƃ���
	obj_.Trans_ += Velocity_;
	obj_.Update();

	//�f�X�^�C�}�[���Ђ���0�ȉ��ɂȂ�����t���O�𗧂Ă�
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
		CollisionManager::GetInstance()->RemoveCollider(&Collider);
	}

	Collider.Update(obj_.GetWorldPos());

	if (collision.isHit)
	{
		OnCollision();
	}

}

void PlayerBullet::Draw(AnimationModel* model)
{
	//�k���|�`�F�b�N
	assert(model);
	obj_.FBXDraw(*model);

	collision.Draw(model);
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
	CollisionManager::GetInstance()->RemoveCollider(&Collider);
}

Vector3 PlayerBullet::GetWorldPosition()
{
	return obj_.GetWorldPos();
}