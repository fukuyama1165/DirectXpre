#include "App.h"
#include "SceneFactory.h"

void App::Initialize()
{
	
	Framework::Initialize();

	//最初のシーンをセット
	//デバックならエディタを起動
#ifdef _DEBUG
	SceneManager::GetInstance()->ChangeScene("EventEditor", "NONE");
#else
	SceneManager::GetInstance()->ChangeScene("TITLE","NONE");
#endif
}

void App::Finalize()
{

	Framework::Finalize();
}

void App::Update()
{
	
	Framework::Update();
#ifdef _DEBUG

	ImGui::Begin("SceneCheng");

	const char* SceneChar[] = { "TITLE","GAMEPLAY","EventEditor" };

	//intしか使えん許さん
	ImGui::Combo("SceneType", (int*)&sceneTypeNum_, SceneChar, IM_ARRAYSIZE(SceneChar));

	switch (sceneTypeNum_)
	{
	case 0:
		sceneType_ = "TITLE";
		break;
	case 1:
		sceneType_ = "GAMEPLAY";
		break;
	case 2:
		sceneType_ = "EventEditor";
		break;
	default:
		sceneType_ = "TITLE";
		break;
	}

	if (ImGui::Button("SceneCheng"))
	{
		SceneManager::GetInstance()->ChangeScene(sceneType_);
	}

	ImGui::End();
#endif
	
}

void App::Draw()
{
#pragma region 描画コマンド

	
	
	Framework::Draw();

	

#pragma endregion

}

