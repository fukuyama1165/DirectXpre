#include "player.h"
#include <imgui.h>

player::player()
{

}
player::~player()
{

}



void player::Init(const std::string& directoryPath, const char filename[])
{
	input_ = input_->GetInstance();
	playerObj_.objDrawInit(directoryPath, filename,true);
	attackObj_.objDrawInit(directoryPath, filename);
	
	//attackObj.SetParent(&playerObj_);
	attackObj_.SetPos({ -50,0,0 });
	playerObj_.SetPos({ 50,0,0 });

}

void player::Update(const cameraObj& camera)
{
	moveVec_ = { 0,0,0 };


	if (input_->PushKey(DIK_SPACE))
	{
		attackFlag_ = true;
	}

	//à⁄ìÆ

	if (moveVec_.x != 0 || moveVec_.z != 0)
	{

		playerObj_.Trans_ += moveVec_ * moveSpeed_;
	}

	//Ç»ÇÒÇ©íºÇ≈ÉJÉÅÉâÇ™éÊÇ¡ÇƒÇ±ÇÍÇ»Ç©Ç¡ÇΩÇΩÇﬂ
	cameraObj came = camera;
	
	playerObj_.Update(came.GetCamera());
	attackObj_.Update(came.GetCamera());

	//Attack();

	if (playerObj_.GetWorldPos().x < -1200)
	{
		playerObj_.SetPos({ -1200, playerObj_.GetWorldPos().y,playerObj_.GetWorldPos().z });
	}

	if (playerObj_.GetWorldPos().x > 2000)
	{
		playerObj_.SetPos({ 2000, playerObj_.GetWorldPos().y, playerObj_.GetWorldPos().z });
	}

	if (playerObj_.GetWorldPos().z < -600)
	{
		playerObj_.SetPos({ playerObj_.GetWorldPos().x, playerObj_.GetWorldPos().y, -600 });
	}

	if (playerObj_.GetWorldPos().z > 2000)
	{
		playerObj_.SetPos({ playerObj_.GetWorldPos().x, playerObj_.GetWorldPos().y, 2000 });
	}
}

void player::Draw()
{
	playerObj_.Draw();
	attackObj_.Draw();
}

void player::Attack()
{
	
	if (attackFlag_)
	{

		if (attackTime_ < 9.5)
		{

			attackObj_.SetPos({ sinf(attackTime_)*120 ,0,cosf(attackTime_)*120 });
			attackTime_ +=/*0.45*/0.05f;
		}
		else
		{
			attackObj_.SetPos({ 0,0,-120 });
			attackFlag_ = false;
			attackTime_ = 3;
		}

	}



}