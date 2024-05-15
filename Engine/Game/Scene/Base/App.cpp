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
	
}

void App::Draw()
{
#pragma region 描画コマンド

	
	
	Framework::Draw();

	

#pragma endregion

}

