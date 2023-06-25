#include "App.h"

void App::Initialize()
{
	
	Framework::Initialize();

	//最初のシーンをセット
	SceneManager::GetInstance()->SetNextScene(std::make_unique<GameScene>());

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

	DirectXInit::GetInstance()->DrawStart();
	
	//Framework::Draw();

	SceneManager::GetInstance()->Draw();


	DirectXInit::GetInstance()->DrawEnd();

	

#pragma endregion

}

