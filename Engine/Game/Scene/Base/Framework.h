#pragma once
#include <memory>
#include "WinApp.h"
#include "DirectXInit.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "SceneManager.h"
#include "PostEffect.h"
#include "XAudio.h"

class Framework
{
public:

	virtual ~Framework() = default;
	
	//初期化
	virtual void Initialize();

	//終了処理
	virtual void Finalize();

	//毎フレーム更新
	virtual void Update();

	//描画
	virtual void Draw();

	//終了チェック
	virtual bool IsEndRequst() { return endRequst_; };

	//シーンの一連を実行
	void Ran();


	

private:

	//ゲームを終わらせるためのフラグ
	bool endRequst_ = false;

	//windowAPI
	WinApp* winApp_ = WinApp::GetInstance();

	//directXの初期化
	DirectXInit* directXinit_ = DirectXInit::GetInstance();

	ImGuiManager* imGuiManager_ = ImGuiManager::GetInstance();

	SceneManager* sceneManager_ = SceneManager::GetInstance();

	//入力の初期化
	Input* input_ = Input::GetInstance();
	
	std::shared_ptr<PostEffect> postEffect_ = nullptr;

	XAudio* xAudio_ = XAudio::GetInstance();


};
