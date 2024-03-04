#include "Framework.h"
#include "LightManager.h"
#include "camera.h"
#include <imgui.h>

void Framework::Ran()
{

	//初期化
	Initialize();

	//ゲームループ
	while (true)
	{
		imGuiManager_->Begin();
		//毎フレーム更新
		Update();

		//終了リクエストが来たら終了
		if (IsEndRequst())
		{
			break;
		}
		
		Draw();

	}

	Finalize();

}

//初期化
void Framework::Initialize()
{

	winApp_->initialize();

	directXinit_->Init();

	imGuiManager_->Init();

	input_->init();

	xAudio_->Init();
	
	Texture::GetInstance()->loadTexture("Resources/white1x1.png", "white1x1");

	postEffect_ = std::make_shared<PostEffect>();

	postEffect_->Initialize();

	LightGroup::Staticlnitialize();

	LightManager::GetInstance()->CreateLightGroup();

}

//終了処理
void Framework::Finalize()
{
	sceneManager_->Finalize();
	XAudio::GetInstance()->Final();
	imGuiManager_->Finalize();
}

//毎フレーム更新
void Framework::Update()
{

	if (winApp_->processMassage() || input_->TriggerKey(DIK_ESCAPE))
	{

		endRequst_ = true;

	}

	input_->update();

	sceneManager_->Update();

	Camera::nowCamera->upDate();

	postEffect_->Update();

	
}

void Framework::Draw()
{

	postEffect_->PreDrawScene();

	sceneManager_->Draw();

	postEffect_->PostDrawScene();

	DirectXInit::GetInstance()->DrawStart();

	postEffect_->Draw();

	imGuiManager_->End();
	ImGuiManager::GetInstance()->Draw();

	DirectXInit::GetInstance()->DrawEnd();

}
