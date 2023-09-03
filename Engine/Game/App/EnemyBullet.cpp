#include "EnemyBullet.h"
#include <cassert>

EnemyBullet::EnemyBullet()
{

}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Initlize(const Vector3& position, const Vector3& velocity)
{


	obj_.FBXInit();

	//�����Ŏ󂯎�����������W���Z�b�g
	obj_.Trans_ = position;

	Velocity_ = velocity;

	collision = BulletCollision("enemyBullet");



	Collider.SetObject(&collision);

	Collider.Initialize();

	Collider.size_ = { 0.8f,0.8f,0.8f };

	CollisionManager::GetInstance()->AddCollider(&Collider);


}

void EnemyBullet::Update()
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

void EnemyBullet::Draw(AnimationModel* model)
{
	//�k���|�`�F�b�N
	assert(model);
	obj_.FBXDraw(*model);
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
	CollisionManager::GetInstance()->RemoveCollider(&Collider);
}

Vector3 EnemyBullet::GetWorldPosition()
{
	return obj_.GetWorldPos();
}