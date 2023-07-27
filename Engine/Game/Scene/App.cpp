#include "App.h"
#include "SceneFactory.h"

void App::Initialize()
{
	
	Framework::Initialize();

	//�ŏ��̃V�[�����Z�b�g
	
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
#pragma region �`��R�}���h

	
	
	Framework::Draw();

	

#pragma endregion

}

