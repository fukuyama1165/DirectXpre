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

	textureNum_ = Texture::GetInstance()->loadTexture("Resources/titleText.png");

	title_.initialize(SpriteCommon::GetInstance(), textureNum_);

	title_.pos_ = { WinApp::SWindowWidth_ / 2,WinApp::SWindowHeight_ / 2 };
	title_.scale_ = { 10,5 };

}

void TitleScene::Finalize()
{

}

void TitleScene::Update()
{
	/*ImGui::Begin("check");

	ImGui::Text("Title");
	ImGui::Text("next:m");

	ImGui::End();*/

	cameobj_.upDate();

	if (Input::GetInstance()->GetMouseButtonDown(0))
	{
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

	objobj3_.Update(cameobj_.GetCamera());

	title_.Update();

}

void TitleScene::Draw()
{
	objobj3_.Draw();

	title_.Draw();
}