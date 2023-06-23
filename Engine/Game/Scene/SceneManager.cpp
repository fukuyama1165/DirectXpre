#include "SceneManager.h"


SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

SceneManager::~SceneManager()
{
	
}

void SceneManager::Update()
{

	if (nextScene_)
	{
		if (nowScene_)
		{
			nowScene_->Finalize();
			nowScene_.release();
		}

		nowScene_ = std::move(nextScene_);
		nowScene_->Initialize();

	}


	nowScene_->Update();

}

void SceneManager::Draw()
{

	nowScene_->Draw();

}

void SceneManager::Finalize()
{
	if (nowScene_)
	{
		nowScene_->Finalize();
		nowScene_.release();
	}
}