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

void EnemyManager::PopEnemy(std::string enemyType, Vector3 pos, Vector3 movePoint, float moveSpeed)
{
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

	newEnemy->Init(enemyType, pos, movePoint,moveSpeed);
	//敵キャラを登録
	enemys_.push_back(std::move(newEnemy));
}

void EnemyManager::Init()
{
	ModelManager::GetInstance()->Load("testGLTFBall", "gltf", "whiteBall", "white1x1");
	ModelManager::GetInstance()->Load("enemy", "gltf", "Enemy", "enemy");

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

	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)//ifの中で簡易的な関数を生成してる->[](引数)
	{
		return bullet->IsDead();
	});
	
	for (std::unique_ptr<Enemy>& enem : enemys_)
	{
		enem->Update(enemyDownSound_);

		if ((enem->GetCT() <= 0 or isDebugShot_)and !isDebugShotStop_)
		{
			EnemyAttack(*enem, playerPos);
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
	//発射地点の為に自キャラの座標をコピー
	Vector3 position = enemy.GetObjWorldPos();
	position.z += 2;

	
	Vector3 velocity(0, 0, 0);
	velocity = playerPos - enemy.GetObjWorldPos();
	velocity = velocity.normalize() * bulletSpeed_;

	

	//速度ベクトルを自機の向きに合わせて回転する
	velocity = VectorMat(velocity, enemy.enemyObj_.GetWorldMat());

	//弾の生成と初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initlize(position, velocity, { 0.3f,0.3f,0.3f });

	//弾を登録
	bullets_.emplace_back(std::move(newBullet));
}