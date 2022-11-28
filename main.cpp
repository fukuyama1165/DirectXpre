#include<Windows.h>

//D3Dコンパイラ
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<vector>
#include<string>

#include<d3d12.h>
#include<dxgi1_6.h>

//数学ライブラリ(最適化されすぎて使いにくいらしいのであとで自作しろって)
#include<DirectXMath.h>
using namespace DirectX;

//#pragma commentとは、オブジェクトファイルに、
//リンカでリンクするライブラリの名前を記述するもの
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

//キーボードやコントローラーなどの入力するヘッダとライブラリのリンク
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

const float PI = 3.141592653589f;

#include <DirectXTex.h>

//#include "DrawingObj.h"

#include "WinApp.h"

#include "DirectXInit.h"

#include <dxgidebug.h>

#include "Input.h"

//#include "DrawOBJ.h"
#include "Object3D.h"

#include "Sprite.h"

#pragma region ウィンドウプロシージャ


//LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
//{
//	switch (msg)
//	{
//	case WM_DESTROY://ウィンドウが破棄された
//		PostQuitMessage(0);//OSに対して、終わったことを伝える
//		return 0;
//	}
//	return DefWindowProc(hwnd, msg, wparam, lparam);//標準の処理をする
//}

#pragma endregion


//デバイス発見時の実行される
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}


/// <summary>
/// 二次元のアフィン変換をする関数
/// </summary>
/// <param name="box">頂点データ(X,Y)</param>
/// <param name="box">図形の中の原点(X,Y)</param>
/// <param name="moveX">x方向の移動量</param>
/// <param name="moveY">y方向の移動量</param>
/// <param name="rotate">回転角度(度数法)</param>
/// <param name="scaleX">x方向の拡大率</param>
/// <param name="scaleY">y方向の拡大率</param>
XMFLOAT3 Afin(XMFLOAT3 box, XMFLOAT3 box2, float moveX, float moveY, float rotate, float scaleX, float scaleY);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//windowAPI
	WinApp* winApp;

	winApp = new WinApp();

	winApp->initialize();

	//directXの初期化
	DirectXInit* directXinit;

	directXinit = DirectXInit::GetInstance();

	directXinit->Init(winApp->getW(), winApp->getHwnd(), winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());

	//入力の初期化
	Input* input = new Input();

	input->init(winApp->getW(), winApp->getHwnd());

	//obj
	Object3D charactorObj;
	Object3D charactorObj2;

	//.objのオブジェクト
	//DrawOBJ test(winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());


	charactorObj.colorChangeInit(directXinit->Getdev().Get(), winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());
	charactorObj2.basicInit(directXinit->Getdev().Get(), winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());

	int texname;

	texname = charactorObj.loadTexture("Resources/hokehoke.png");
	//test.basicInit((directXinit->Getdev().Get()));

	SpriteCommon spritecommon;

	spritecommon.initialize(directXinit->Getdev().Get());

	Sprite sprite;

	sprite.initialize(directXinit->Getdev().Get());

	//パイプラインステート切り替え用フラグ
	bool PipeLineRuleFlag = true;

	//四角形に変更するときのフラグ
	bool ChangeSquareFlag = false;

	bool ChangeTexure = false;


	float Red = 1.0f;
	float Green = 0.0f;
	float Blue = 0.0f;

	float X1 = 0.0f;
	float X2 = 0.0f;
	float Y1 = 0.0f;
	float Y2 = 0.0f;
	float rotate = 0;
	float scaleX = 1;
	float scaleY = 1;

	float angle = 0.0f;//カメラの回転角
	float angleY = 0.0f;//カメラの回転角

	//座標
	Float3 pos = {};

	//作ったのが小さかったので
	/*test.SetScale({ 20,20,0 });
	test.obj3DUpdate();*/


	//ゲームループ
	while (true)
	{

		if (winApp->processMassage() or input->TriggerKey(DIK_ESCAPE))
		{

			break;

		}

#pragma region DirectX毎フレーム処理

		//DirectX毎フレーム処理　ここから

#pragma region キーボード情報の取得

		input->update();

#pragma endregion

		directXinit->DrawStart();

#pragma region 描画コマンド

		// 4.描画コマンドここから

		//ビューポート設定
		//分割分のビューポートを用意(見にくいので減らした)
		D3D12_VIEWPORT viewport{};

		viewport.Width = winApp->getWindowSizeWidth();//横幅
		viewport.Height = winApp->getWindowSizeHeight();//縦幅
		viewport.TopLeftX = 0;//左上X
		viewport.TopLeftY = 0;//左上Y
		viewport.MinDepth = 0.1f;//最少深度(0でよい)
		viewport.MaxDepth = 1.0f;//最大深度(１でよい)

		//コマンドリストに追加
		directXinit->GetcmdList()->RSSetViewports(1, &viewport);


		//シザー矩形設定
		D3D12_RECT scissorrect{};

		scissorrect.left = 0;//切り抜き座標左
		scissorrect.right = scissorrect.left + winApp->getWindowSizeWidth();//切り抜き座標右
		scissorrect.top = 0;//切り抜き座標上
		scissorrect.bottom = scissorrect.top + winApp->getWindowSizeHeight();//切り抜き座標下

		directXinit->GetcmdList()->RSSetScissorRects(1, &scissorrect);

		//プリミティブ形状(三角形リスト)
		directXinit->GetcmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		//charactorObj.Draw(directXinit->GetcmdList().Get(), PipeLineRuleFlag, ChangeSquareFlag,true);
		charactorObj2.Draw(directXinit->GetcmdList().Get(), texname, PipeLineRuleFlag);

		//test.Draw(directXinit->GetcmdList().Get(), PipeLineRuleFlag, true, true);

		//sprite.Draw(directXinit->GetcmdList().Get(), spritecommon, PipeLineRuleFlag);

		// 4.描画コマンドここまで

#pragma endregion

		directXinit->DrawEnd();

		//DirectX毎フレーム処理　ここまで

#pragma endregion

#pragma region 更新処理

		if (input->TriggerKey(DIK_2))
		{
			PipeLineRuleFlag = !PipeLineRuleFlag;
		}

		if (input->TriggerKey(DIK_1))
		{
			ChangeSquareFlag = !ChangeSquareFlag;
		}

		if (input->TriggerKey(DIK_3))
		{
			ChangeTexure = !ChangeTexure;
		}


		if (input->PushKey(DIK_D) or input->PushKey(DIK_A) or input->PushKey(DIK_W) or input->PushKey(DIK_S))
		{
			if (input->PushKey(DIK_D))
			{
				angle += XMConvertToRadians(1.0f);
			}
			else if (input->PushKey(DIK_A))
			{
				angle -= XMConvertToRadians(1.0f);
			}
			else if (input->PushKey(DIK_W))
			{
				angleY -= XMConvertToRadians(500.0f);
			}
			else if (input->PushKey(DIK_S))
			{
				angleY += XMConvertToRadians(500.0f);
			}

			//charactorObj.matViewUpdata({ -400 * sinf(angle),angleY,-400 * cosf(angle) }, { 0,0,0 }, { 0,1,0 });
			charactorObj.matViewUpdata({ -400 * sinf(angle),angleY,  -400 * cosf(angle) }, { 0,0,0 }, { 0,1,0 });
			charactorObj2.matViewUpdata({ -400 * sinf(angle),angleY,  -400 * cosf(angle) }, { 0,0,0 }, { 0,1,0 });

		}

		if (input->PushKey(DIK_UP) or input->PushKey(DIK_DOWN) or input->PushKey(DIK_RIGHT) or input->PushKey(DIK_LEFT))
		{
			if (input->PushKey(DIK_UP))
			{
				pos.z += 1.0f;
			}
			else if (input->PushKey(DIK_DOWN))
			{
				pos.z -= 1.0f;
			}
			else if (input->PushKey(DIK_RIGHT))
			{
				pos.x += 1.0f;
			}
			else if (input->PushKey(DIK_LEFT))
			{
				pos.x -= 1.0f;
			}

			charactorObj.SetPos(pos);

			charactorObj.Update();

		}




		//色変更
		if (Red > 0 and Blue <= 0)
		{
			Red -= 0.05f;
			Green += 0.05f;
		}
		if (Green > 0 and Red <= 0)
		{
			Green -= 0.05f;
			Blue += 0.05f;
		}
		if (Blue > 0 and Green <= 0)
		{
			Blue -= 0.05f;
			Red += 0.05f;
		}

#pragma endregion

#pragma region 描画処理

		//charactorObj.colorMap(Red, Green, Blue);

#pragma endregion



	}

	delete winApp;

	//texture.hを読み込んでいるobjctが増えてもdelteするのは一回にすること
	charactorObj.deleteTexture();

	delete input;

	directXinit->instanceDelete();

	return 0;


}

