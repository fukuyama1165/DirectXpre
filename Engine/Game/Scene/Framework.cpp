#include "Framework.h"

void Framework::Ran()
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
void Framework::Initialize()
{

	winApp->initialize();


	directXinit->Init();

	imGuiManager->Init();

	input->init();

}

//終了処理
void Framework::Finalize()
{

	imGuiManager->Finalize();
}

//毎フレーム更新
void Framework::Update()
{

	if (winApp->processMassage() or input->TriggerKey(DIK_ESCAPE))
	{

		endRequst_ = true;

	}

	input->update();

	imGuiManager->Begin();

	sceneManager->Update();

}
