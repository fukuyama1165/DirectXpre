#include "App.h"
#include "SceneFactory.h"

void App::Initialize()
{
	
	Framework::Initialize();

	//最初のシーンをセット
	
	//SceneManager::GetInstance()->SetNextScene(std::make_unique<GameScene>());
	SceneManager::GetInstance()->ChangeScene("TITLE","NONE");
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

