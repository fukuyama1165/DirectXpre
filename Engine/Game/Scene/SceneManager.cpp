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
				//���݂̃V�[�����I��点��
				nowScene_->Finalize();
				//unique_ptr��release�͂����Ŏg�����Ƃ���ƃ��������[�N���N����̂ŋC��t���悤
				//nowScene_.release();
			}

			//���݂�ύX��̃V�[����
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
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	//���̃V�[�������
	nextScene_ = sceneFactory_->CreateScene(sceneName);

	SceneChange_ = sceneChangeFactory_->CreateSceneChange(sceneChangeName);
	SceneChange_->SetIsStart(true);

}