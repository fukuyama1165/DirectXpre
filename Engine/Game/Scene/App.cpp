#include "App.h"

void App::Initialize()
{
	
	Framework::Initialize();

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
#pragma region •`‰æƒRƒ}ƒ“ƒh

	DirectXInit::GetInstance()->DrawStart();
	
	

	SceneManager::GetInstance()->Draw();


	DirectXInit::GetInstance()->DrawEnd();

	

#pragma endregion

}

