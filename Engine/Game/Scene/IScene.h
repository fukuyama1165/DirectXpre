#pragma once
#include <memory>
#include "WinApp.h"
#include "DirectXInit.h"
#include "ImGuiManager.h"
#include "Input.h"

class IScene
{
public:

	virtual ~IScene() = default;
	
	//初期化
	virtual void Initialize();

	//終了処理
	virtual void Finalize();

	//毎フレーム更新
	virtual void Update();

	//描画
	virtual void Draw() = 0;

	//終了チェック
	virtual bool IsEndRequst() { return endRequst_; };

	//シーンの一連を実行
	void Ran();


	

private:

	//ゲームを終わらせるためのフラグ
	bool endRequst_ = false;

	//windowAPI
	WinApp* winApp = WinApp::GetInstance();

	//directXの初期化
	DirectXInit* directXinit = DirectXInit::GetInstance();

	ImGuiManager* imGuiManager = ImGuiManager::GetInstance();

	//入力の初期化
	Input* input = input->GetInstance();
	

};
