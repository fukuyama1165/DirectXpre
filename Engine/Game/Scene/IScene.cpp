#include "IScene.h"

void IScene::Ran()
{

	//初期化
	Initialize();

	//ゲームループ
	while (true)
	{

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
void IScene::Initialize()
{

	winApp->initialize();


	directXinit->Init(winApp->getW(), winApp->getHwnd(), winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());

	imGuiManager->Init();

	input->init(winApp->getW(), winApp->getHwnd());

}

//終了処理
void IScene::Finalize()
{

	imGuiManager->Finalize();
}

//毎フレーム更新
void IScene::Update()
{

	if (winApp->processMassage() or input->TriggerKey(DIK_ESCAPE))
	{

		endRequst_ = true;

	}

	input->update();

	imGuiManager->Begin();

}
