#include "TitleScene.h"
#include <imgui.h>
#include "Input.h"
#include "SceneManager.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{

}

void TitleScene::Finalize()
{

}

void TitleScene::Update()
{
	ImGui::Begin("check");

	ImGui::Text("Title");
	ImGui::Text("next:m");

	ImGui::End();


	if (Input::GetInstance()->TriggerKey(DIK_M))
	{
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

}

void TitleScene::Draw()
{

}