#include "EnemyManager.h"


void EnemyManager::PopEnemy(const Vector3& pos)
{
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

	newEnemy->Init();

	newEnemy->enemyObj_.SetPos(pos);


	//敵キャラを登録
	enemys_.push_back(std::move(newEnemy));
}

void EnemyManager::Init()
{
	bulletModel_ = std::make_unique<AnimationModel>();

	bulletModel_->Load("testGLTFBall", "gltf", "white1x1");

	enemyModel_ = std::make_unique<AnimationModel>();

	enemyModel_->Load("testGLTFBall", "gltf", "white1x1");
}

void EnemyManager::UpDate(const Camera& camera,const Vector3& playerPos)
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
		enem->Update(camera);

		if (enem->GetCT() <= 0)
		{
			EnemyAttack(*enem, playerPos);
		}
	}

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update(camera);
	}


}

void EnemyManager::Draw()
{

	for (std::unique_ptr<Enemy>& enem : enemys_)
	{
		enem->Draw(enemyModel_.get());
	}

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(bulletModel_.get());
	}

}

void EnemyManager::EnemyAttack(Enemy enemy, const Vector3& playerPos)
{
	//発射地点の為に自キャラの座標をコピー
	Vector3 position = enemy.GetObjWorldPos();
	position.z += 2;

	//移動量を追加
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, 0);
	velocity = playerPos - enemy.GetObjWorldPos();
	velocity = velocity.normalize() * kBulletSpeed;

	//速度ベクトルを自機の向きに合わせて回転する
	velocity = VectorMat(velocity, enemy.enemyObj_.GetWorldMat());

	//弾の生成と初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initlize(position, velocity);

	//弾を登録
	bullets_.emplace_back(std::move(newBullet));
}