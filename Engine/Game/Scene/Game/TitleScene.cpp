#include "TitleScene.h"
#include <imgui.h>
#include "Input.h"
#include "SceneManager.h"
#include "LightManager.h"
#include "Texture.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	Object3D::SetLight(&LightManager::GetInstance()->lightGroups_[0]);

	objobj3_.objDrawInit("Resources/obj/skydome/", "skydome.obj");
	objobj3_.SetScale({ 1000,1000,1000 });

	cameobj_ = cameraObj((float)WinApp::GetInstance()->getWindowSizeWidth(), (float)WinApp::GetInstance()->getWindowSizeHeight());

	cameobj_.pos_ = { 0,0,-50 };

	textureNum_ = Texture::GetInstance()->loadTexture("Resources/titleText.png","titleText");

	title_.initialize(textureNum_);

	title_.pos_ = { (float)WinApp::SWindowWidth_ / 2,(float)WinApp::SWindowHeight_ / 2 };
	title_.scale_ = { 1,1 };

	
	Camera::nowCamera = cameobj_.GetCameraP();

}

void TitleScene::Finalize()
{

}

void TitleScene::Update()
{

	cameobj_.upDate();

	if ((Input::GetInstance()->GetMouseButtonDown(0)and Input::GetInstance()->GetMouseInWindow()) or Input::GetInstance()->GetGamePadButton(XINPUT_GAMEPAD_A))
	{
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

	objobj3_.Update();

	title_.Update();

	if (debugMenu_)
	{

#pragma region check

		ImGui::Begin("check");

		ImGui::Text("%0.0fFPS", ImGui::GetIO().Framerate);;

		ImGui::End();

#pragma endregion
	}

	if (Input::GetInstance()->TriggerKey(DIK_F3))
	{
		debugMenu_ = !debugMenu_;
	}

}

void TitleScene::Draw()
{
	objobj3_.Draw();

	title_.Draw();
}