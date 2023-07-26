#include "BasicSceneChange.h"
#include "Easing.h"
#include "WinApp.h"
#include <imgui.h>

BasicSceneChange::BasicSceneChange()
{
}

BasicSceneChange::~BasicSceneChange()
{
}

void BasicSceneChange::Initialize()
{
	chengSprite_.initialize(SpriteCommon::GetInstance(),1);
	chengSprite_.pos_ = WinApp::GetInstance()->GetWindowCenter();

	isStart_ = false;
	isClose_ = false;
	isOpenStart_ = false;
	isEnd_ = false;
	angle_ = 0;
	scale_ = 1;
	scaleTimer_ = 0;
	afterTimer = afterMaxTime;

}

void BasicSceneChange::Finalize()
{

}

void BasicSceneChange::Update()
{
	if (isStart_ and !isEnd_)
	{
		if (!isClose_)
		{
			scale_ = easeOutQuad(1.0f, 100.0f, scaleTimer_ / scaleMaxTime_);

			chengSprite_.scale_ = Vector2(scale_, scale_);
			chengSprite_.rotate_ = angle_;
			chengSprite_.Update();

			
			if (scaleTimer_ < scaleMaxTime_)
			{
				scaleTimer_++;
				angle_ += rotSpeed_;
			}
			else
			{
				isClose_ = true;
				
			}
			

		}

		if (isOpenStart_)
		{
			alpha_ = easeInQuint(0.0f, 1.0f, afterTimer / afterMaxTime);

			chengSprite_.setColor(Vector4(1, 1, 1, alpha_));
			chengSprite_.Update();

			if (afterTimer > 0)
			{
				afterTimer--;
			}
			else
			{
				isEnd_ = true;
			}
		}

	}

	ImGui::Begin("check");

	ImGui::Text("alpha:%0.1f", chengSprite_.rotate_);

	ImGui::End();
}

void BasicSceneChange::Draw()
{
	if (!isEnd_)
	{
		chengSprite_.Draw();
	}

	
}