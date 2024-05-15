#include "SceneManager.h"

#include "SceneFactory.h"
#include "SceneChangeFactory.h"
//assert!!!!!!!
#include <cassert>
#include "EventPointManager.h"
#include <imgui.h>
#include "Input.h"



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
	DebugImguiChangeScene();

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
	isDebugMode_ = false;

	//次に移動するのがイベントエディタだった場合(この作品限定の動きなので汎用的ではない)
	if (sceneName == "EventEditor")
	{
		EventPointManager::GetInstance()->EditorMoveSave();
		isDebugMode_ = true;
	}

	SceneChange_ = sceneChangeFactory_->CreateSceneChange(sceneChangeName);
	SceneChange_->SetIsStart(true);

}

void SceneManager::DebugImguiChangeScene()
{
#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_F5, true))
	{
		isDebugMode_ = !isDebugMode_;
	}

	if (isDebugMode_)
	{
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
	}
#endif
}