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

}

//終了処理
void IScene::Finalize()
{

}

//毎フレーム更新
void IScene::Update()
{

}
