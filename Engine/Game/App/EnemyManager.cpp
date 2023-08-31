#include "EnemyManager.h"
#include <imgui.h>
#include "ModelManager.h"
#include "XAudio.h"


EnemyManager* EnemyManager::GetInstance()
{
	static EnemyManager instance;

	if (instance.enemyModel_ == nullptr and instance.bulletModel_ == nullptr)
	{
		instance.Init();
	}

	return &instance;
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::PopEnemy(uint16_t enemyType, Vector3 pos, Vector3 movePoint)
{
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

	newEnemy->Init(enemyType, pos, movePoint);
	//�G�L������o�^
	enemys_.push_back(std::move(newEnemy));
}

void EnemyManager::Init()
{
	ModelManager::GetInstance()->Load("testGLTFBall", "gltf", "whiteBall", "white1x1");
	ModelManager::GetInstance()->Load("enemy", "gltf", "Enemy", "white1x1");

	enemyDownSound_ = XAudio::GetInstance()->SoundLoadWave("Resources/sound/enemydown.wav");

	bulletModel_ = ModelManager::GetInstance()->SearchModelData("whiteBall");
	enemyModel_ = ModelManager::GetInstance()->SearchModelData("Enemy");
}

void EnemyManager::UpDate(const Vector3& playerPos)
{
	
	enemys_.remove_if([](std::unique_ptr<Enemy>& enem)
	{
		return !enem->isAlive_;
	});

	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)//if�̒��ŊȈՓI�Ȋ֐��𐶐����Ă�->[](����)
	{
		return bullet->IsDead();
	});
	
	for (std::unique_ptr<Enemy>& enem : enemys_)
	{
		enem->Update(enemyDownSound_);

		if ((enem->GetCT() <= 0 or isDebugShot_)and !isDebugShotStop_)
		{
			EnemyAttack(*enem, playerPos);
			//playerPos;
		}
	}

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	enemyCount_ = (uint32_t)enemys_.size();
	
}

void EnemyManager::Draw()
{

	for (std::unique_ptr<Enemy>& enem : enemys_)
	{
		enem->Draw(enemyModel_);
	}

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(bulletModel_);
	}

}

void EnemyManager::EnemyAttack(Enemy enemy, const Vector3& playerPos)
{
	//���˒n�_�ׂ̈Ɏ��L�����̍��W���R�s�[
	Vector3 position = enemy.GetObjWorldPos();
	position.z += 2;

	
	Vector3 velocity(0, 0, 0);
	velocity = playerPos - enemy.GetObjWorldPos();
	velocity = velocity.normalize() * bulletSpeed_;

	

	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]����
	velocity = VectorMat(velocity, enemy.enemyObj_.GetWorldMat());

	/*ImGui::Begin("enemy");

	ImGui::Text("velocity:%0.5f,%0.5f,%0.5f", velocity.x, velocity.y, velocity.z);

	ImGui::End();*/

	//�e�̐����Ə�����
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initlize(position, velocity);

	//�e��o�^
	bullets_.emplace_back(std::move(newBullet));
}