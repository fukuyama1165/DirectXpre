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
				hitPos = enemyObj.GetPos();
				hitVec = hitVec.normalize();
				hitVec = -hitVec;
				isHitSet = true;
			}

			Vector3 a = lerp(hitPos,{ 500 + (hitVec.x * 300),500 + (hitVec.y * 300),500 + (hitVec.z * 300) }, (float)hitTime / 300);
			Vector3 b = lerp({ 500 + (hitVec.x * 300),500 + (hitVec.y * 300),500 + (hitVec.z * 300) }, {hitPos.x + (hitVec.x * 300),hitPos.y + (hitVec.y * 300),hitPos.z + (hitVec.y * 300)}, (float)hitTime / 300);

			Vector3 c = lerp(a, b, (float)hitTime / 300);

			enemyObj.SetPos(c);
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