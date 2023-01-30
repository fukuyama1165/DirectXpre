#include "player.h"
#include <imgui.h>

player::player()
{

}
player::~player()
{

}



void player::Init(ID3D12Device* dev, const std::string directoryPath, const char filename[])
{
	input = input->GetInstance();
	playerObj.objDrawInit(dev, directoryPath, filename,true);
	attackObj.objDrawInit(dev, directoryPath, filename);
	
	//attackObj.SetParent(&playerObj);
	attackObj.SetPos({ -50,0,0 });
	playerObj.SetPos({ 50,0,0 });
	
}

void player::Update(cameraObj camera)
{
	moveVec = { 0,0,0 };

	

	/*if (input->PushKey(DIK_UP))
	{
		moveVec.z = 1;
	}
	if (input->PushKey(DIK_DOWN))
	{
		moveVec.z = -1;
	}
	if (input->PushKey(DIK_RIGHT))
	{
		moveVec.x = 1;
	}
	if (input->PushKey(DIK_LEFT))
	{
		moveVec.x = -1;
	}*/

	if (input->PushKey(DIK_SPACE))
	{
		attackFlag = true;
	}

	//ˆÚ“®

	playerObj.SetRotate({ 0,rotate,0 });
	attackObj.SetRotate({ 0,rotate,0 });

	rotate+=0.01;

	/*float pPos = atan2(moveVec.x, moveVec.z);
	float cVec = atan2(camera.forward.x, camera.forward.z);

	playerObj.SetRotate({playerObj.GetRotate().x, (pPos + cVec),playerObj.GetRotate().z});

	Float3 mae = { 0,0,1.0f };

	mae = VectorMat(mae, playerObj.GetWorldMat());

	mae.normalize();*/

	if (moveVec.x != 0 || moveVec.z != 0)
	{

		playerObj.Trans_ += moveVec * moveSpeed;
	}
	
	playerObj.Update(camera.GetCamera());
	attackObj.Update(camera.GetCamera());

	//Attack();

	if (playerObj.GetWorldPos().x < -1200)
	{
		playerObj.SetPos({ -1200, playerObj.GetWorldPos().y,playerObj.GetWorldPos().z });
	}

	if (playerObj.GetWorldPos().x > 2000)
	{
		playerObj.SetPos({ 2000, playerObj.GetWorldPos().y, playerObj.GetWorldPos().z });
	}

	if (playerObj.GetWorldPos().z < -600)
	{
		playerObj.SetPos({ playerObj.GetWorldPos().x, playerObj.GetWorldPos().y, -600 });
	}

	if (playerObj.GetWorldPos().z > 2000)
	{
		playerObj.SetPos({ playerObj.GetWorldPos().x, playerObj.GetWorldPos().y, 2000 });
	}
}

void player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	playerObj.Draw(cmdList);
	attackObj.Draw(cmdList);
}

void player::Attack()
{
	
	if (attackFlag)
	{

		if (attackTime < 9.5)
		{

			attackObj.SetPos({ sinf(attackTime)*120 ,0,cosf(attackTime)*120 });
			attackTime+=/*0.45*/0.05;
		}
		else
		{
			attackObj.SetPos({ 0,0,-120 });
			attackFlag = false;
			attackTime = 3;
		}

	}



}