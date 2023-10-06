#include "SceneManager.h"

#include "SceneFactory.h"
#include "SceneChangeFactory.h"
//assert!!!!!!!
#include <cassert>



SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;

	if (instance.sceneFactory_ == nullptr)
	{
		instance.sceneFactory_ = std::make_unique<SceneFactory>();
	}

	if (instance.sceneChangeFactory_ == nullptr)
	{
		instance.sceneChangeFactory_ = std::make_unique<SceneChangeFactory>();
	}

	return &instance;
}

SceneManager::~SceneManager()
{
	
}

void SceneManager::Update()
{
	SceneChange_->Update();

	if (nextScene_)
	{

		if (SceneChange_->GetIsClose())
		{
			if (nowScene_)
			{
				//現在のシーンを終わらせる
				nowScene_->Finalize();
				//unique_ptrのreleaseはここで使おうとするとメモリリークが起きるので気を付けよう
				//nowScene_.release();
			}

			//現在を変更後のシーンに
			nowScene_ = std::move(nextScene_);
			nextScene_ = nullptr;

			nowScene_->Initialize();

			SceneChange_->SetIsOpenStart(true);
		}

	}

	if (nowScene_ != nullptr)
	{
		nowScene_->Update();
	}

}

void SceneManager::Draw()
{
	
	if (nowScene_ != nullptr)
	{
		nowScene_->Draw();
	}

	if (SceneChange_ != nullptr)
	{
		SceneChange_->Draw();
	}
	

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

void SceneManager::ChangeScene(const std::string& sceneName, const std::string& sceneChangeName)
{
	if (nextScene_ != nullptr) return;

	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	//次のシーンを作る
	nextScene_ = sceneFactory_->CreateScene(sceneName);

	SceneChange_ = sceneChangeFactory_->CreateSceneChange(sceneChangeName);
	SceneChange_->SetIsStart(true);

}