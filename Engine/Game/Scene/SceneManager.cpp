#include "SceneManager.h"

#include "SceneFactory.h"
//assert!!!!!!!
#include <cassert>



SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;

	if (instance.sceneFactory_ == nullptr)
	{
		instance.sceneFactory_ = std::make_unique<SceneFactory>();
	}
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
			//現在のシーンを終わらせる
			nowScene_->Finalize();
			//nowScene_.release();
		}

		//現在を変更後のシーンに
		nowScene_ = std::move(nextScene_);
		nextScene_ = nullptr;

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
		nowScene_ = nullptr;
	}

	nextScene_ = nullptr;
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	//次のシーンを作る
	nextScene_ = sceneFactory_->CreateScene(sceneName);

}