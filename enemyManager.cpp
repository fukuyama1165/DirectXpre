#include "enemyManager.h"


void enemyManager::PopEnemy(ID3D12Device* dev, Float3 pos)
{
	std::unique_ptr<enemy> newEnemy = std::make_unique<enemy>();

	newEnemy->Init(dev, "Resources/obj/enemy/", "enemy.obj");

	newEnemy->enemyObj.SetPos(pos);


	//“GƒLƒƒƒ‰‚ð“o˜^
	enemys.push_back(std::move(newEnemy));
}

void enemyManager::UpDate(Camera camera, Float3 playerPos)
{
	for (std::unique_ptr<enemy>& enem : enemys)
	{
		if (enem->isAlive == false)
		{
			enemyCount++;
		}
	}
	enemys.remove_if([](std::unique_ptr<enemy>& enem)
	{
		return !enem->isAlive;
	});
	
	for (std::unique_ptr<enemy>& enem : enemys)
	{
		enem->Update(camera, playerPos);
	}


}

void enemyManager::Draw(ID3D12GraphicsCommandList* cmdList)
{

	for (std::unique_ptr<enemy>& enem : enemys)
	{
		enem->Draw(cmdList);
	}

}