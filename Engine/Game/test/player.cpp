#include "player.h"
#include <imgui.h>
#include "Easing.h"

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
	
	playerObj_.SetPos({ -20,0,0 });
	playerObj_.SetScale({ 0.05f,0.05f,0.05f });

	playerCamera_.pos_ = { 0,0,-200 };
	hogeca.eye_ = { 0,0,-3 };
}

void player::Update(const Camera& camera)
{
	moveVec_ = { 0,0,0 };
	

	if (input_->PushKey(DIK_SPACE) && time_<=20)
	{
		attackFlag_ = true;
		
	}

	if (input_->ReleaseKey(DIK_SPACE))
	{
		attackFlag_ = false;
		//Ç∑ÇÆêUÇËå¸Ç≠ÇÃÇñhé~
		/*if (time_ <= 20 && time_>15)
		{
			time_ += 20;
		}*/

		
	}
	if (cameraCheng_)
	{
		HideRightWall();
	}
	else
	{
		HideDownWall();
	}

	hogeca.upDate();
	playerCamera_.SetCamera(hogeca);

	

	//à⁄ìÆ

	if (moveVec_.x != 0 || moveVec_.z != 0)
	{

		playerObj_.Trans_ += moveVec_ * moveSpeed_;
		//playerCamera_.pos_ += moveVec_ * moveSpeed_;
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

void player::HideRightWall()
{
	if (attackFlag_)
	{
		if (time_ < maxTime_/15)
		{
			time_++;
		}
		hogeca.eye_ = easeOutQuint(Vector3{playerCamera_.pos_.x, playerCamera_.pos_.y, playerCamera_.pos_.z}, Vector3{ playerCamera_.pos_.x+5, playerCamera_.pos_.y, playerCamera_.pos_.z }, time_ / maxMoveTime_);
		hogeca.target_ = easeOutQuint(Vector3{ hogeca.eye_.x,hogeca.eye_.y,hogeca.eye_.z+1 }, Vector3{ hogeca.eye_.x-100,hogeca.eye_.y,hogeca.eye_.z}, time_ / maxTime_);
		
	}
	else
	{
		if (time_ > 0)
		{
			time_--;
		}
		/*hogeca.eye_ = easeOutQuint(Vector3{ playerCamera_.pos_.x + 5, playerCamera_.pos_.y, playerCamera_.pos_.z }, Vector3{ playerCamera_.pos_.x, playerCamera_.pos_.y, playerCamera_.pos_.z }, 1.0f - (time_ / maxTime_));
		hogeca.target_ = easeOutQuint(Vector3{ hogeca.eye_.x - 100,hogeca.eye_.y,hogeca.eye_.z }, Vector3{ hogeca.eye_.x,hogeca.eye_.y,hogeca.eye_.z + 1 }, 1.0f - (time_ / maxTime_));*/
		
		hogeca.eye_ = easeOutQuint(Vector3{ playerCamera_.pos_.x, playerCamera_.pos_.y, playerCamera_.pos_.z }, Vector3{ playerCamera_.pos_.x + 5, playerCamera_.pos_.y, playerCamera_.pos_.z }, time_ / maxMoveTime_);
		hogeca.target_ = easeOutQuint(Vector3{ hogeca.eye_.x,hogeca.eye_.y,hogeca.eye_.z + 1 }, Vector3{ hogeca.eye_.x - 100,hogeca.eye_.y,hogeca.eye_.z }, time_ / maxTime_);

	}
}

void player::HideDownWall()
{
	if (attackFlag_)
	{
		if (time_ < maxTime_)
		{
			time_++;
		}
		
	}
	else
	{
		time_ = 0;
	}

	//hogeca.target_ = easeOutQuint(Vector3{ 0,0,1 }, Vector3{ 0,0,1 }, time_ / maxTime_);
	hogeca.eye_ = easeOutQuint(Vector3{ playerCamera_.pos_.x, playerCamera_.pos_.y, playerCamera_.pos_.z }, Vector3{ playerCamera_.pos_.x, playerCamera_.pos_.y-10, playerCamera_.pos_.z }, time_ / maxMoveTime_);

	playerCamera_.cameobj_.matWorldGeneration();

	Vector3 forward = { 0.0f, 0.0f, 1.0f };

	forward = VectorMat(forward, playerObj_.GetWorldMat());

	hogeca.target_ = hogeca.eye_ + forward;
}