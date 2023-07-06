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
	
	playerObj_.SetPos({ 0,0,0 });
	playerObj_.SetScale({ 0.05f,0.05f,0.05f });

	playerCamera_.pos_ = { 0,0,-200 };

}

void player::Update(const Camera& camera)
{
	moveVec_ = { 1,0,0 };


	if (input_->PushKey(DIK_SPACE))
	{
		attackFlag_ = true;
	}

	//ˆÚ“®

	if (moveVec_.x != 0 || moveVec_.z != 0)
	{

		playerObj_.Trans_ += moveVec_ * moveSpeed_;
		playerCamera_.pos_ += moveVec_ * moveSpeed_;
	}
	
	
	playerObj_.Update(camera);

	if (playerObj_.GetWorldPos().x > 50 || playerObj_.GetWorldPos().x < -50)
	{
		moveSpeed_ = -moveSpeed_;
	}

	//Attack();

	
}

void player::Draw()
{
	playerObj_.Draw();
}

void player::Attack()
{
	
	if (attackFlag_)
	{



	}



}