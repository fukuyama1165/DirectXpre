#include "enemyManager.h"


void enemyManager::PopEnemy(const Vector3& pos)
{
	std::unique_ptr<enemy> newEnemy = std::make_unique<enemy>();

	newEnemy->Init("Resources/obj/enemy/", "enemy.obj");

	newEnemy->enemyObj_.SetPos(pos);


	//“GƒLƒƒƒ‰‚ð“o˜^
	enemys_.push_back(std::move(newEnemy));
}

void enemyManager::UpDate(const Camera& camera,const Vector3& playerPos)
{
	for (std::unique_ptr<enemy>& enem : enemys_)
	{
		if (enem->isAlive_ == false)
		{
			enemyCount_++;
		}
	}
	enemys_.remove_if([](std::unique_ptr<enemy>& enem)
	{
		return !enem->isAlive_;
	});
	
	for (std::unique_ptr<enemy>& enem : enemys_)
	{
		enem->Update(camera, playerPos);
	}


}

void enemyManager::Draw()
{

	for (std::unique_ptr<enemy>& enem : enemys_)
	{
		enem->Draw();
	}

}