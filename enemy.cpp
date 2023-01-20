#include "enemy.h"


enemy::enemy()
{

}
enemy::~enemy()
{

}

void enemy::Init(ID3D12Device* dev, const std::string directoryPath, const char filename[])
{
	input = input->GetInstance();
	enemyObj.objDrawInit(dev, directoryPath, filename);

}

void enemy::Update(Camera camera,Vector3 playerPos)
{
	if (isAlive)
	{
		if (isHit == false)
		{

			Vector3 playerEnemyVec = enemyObj.GetPos() - playerPos;

			playerEnemyVec = playerEnemyVec.normalize();

			enemyObj.SetPos(enemyObj.GetPos() - (playerEnemyVec * 2));

		}
		else
		{
			if (isHitSet == false)
			{
				hitVec = enemyObj.GetPos() - playerPos;
				hitVec = hitVec.normalize();
				hitVec = -hitVec;
				isHitSet = true;
			}

			enemyObj.SetPos(enemyObj.GetPos() - (hitVec * 10));
			hitTime++;
		}

		enemyObj.Update(camera);

		if (hitTime > 300)
		{
			isAlive = false;
		}
	}
}

void enemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	enemyObj.Draw(cmdList);
}