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

#include "DrawingObj.h"

#include "DirectXInit.h"

#include <dxgidebug.h>

#pragma region ウィンドウプロシージャ


LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY://ウィンドウが破棄された
		PostQuitMessage(0);//OSに対して、終わったことを伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//標準の処理をする
}

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

#pragma region ウィンドウの初期化部分
	const int window_width = 1280;
	const int window_height = 720;


	WNDCLASSEX w = {};//ウィンドウクラスの設定

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)windowProc;//ウィンドウプロシージャ
	w.lpszClassName = L"DirectXGame";//ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr);//ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定

	//ウィンドウクラスをOSに登録
	RegisterClassEx(&w);

	//ウィンドウのサイズの構造体{x座標,y座標,横幅,縦幅}
	RECT wrc = { 0,0,window_width,window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//自動でサイズ補正

	//ウィンドウの構成要素？
	HWND hwnd = CreateWindow(w.lpszClassName,//クラス名
		L"DirectXGame",//タイトルバーの名前
		WS_OVERLAPPEDWINDOW,//標準的なウインドウスタイル
		CW_USEDEFAULT,//x座標(OSに任せる)
		CW_USEDEFAULT,//y座標(OSに任せる)
		wrc.right - wrc.left,//ウィンドウ横幅
		wrc.bottom - wrc.top,//ウィンドウ縦幅
		nullptr,//親ウィンドウハンドル
		nullptr,//メニューハンドル
		w.hInstance,//呼び出しアプリケーションハンドル
		nullptr);//オプション

	//ウィンドウ表示部分
	ShowWindow(hwnd, SW_SHOW);

	//これを書かないとウィンドウが一瞬で消えるため記述
	MSG msg{};

	

#pragma endregion
	
	DirectXInit* directXinit;

	directXinit = DirectXInit::GetInstance();

	directXinit->Init(w, hwnd,window_width,window_height);

	DrawingObj charactorObj(window_width, window_height);


	charactorObj.basicInit(directXinit->Getdev().Get());

	//パイプラインステート切り替え用フラグ
	bool PipeLineRuleFlag = true;

	//四角形に変更するときのフラグ
	bool ChangeSquareFlag = true;

	//全キーの入力情報を取得する為の変数
	/*BYTE key[256] = {};
	BYTE oldKey[256] = {};*/

	
	float Red = 1.0f;
	float Green = 1.0f;
	float Blue = 1.0f;

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
	Float3 pos={};


	//ゲームループ
	while (true)
	{
		//メッセージがある？
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);//キー入力メッセージの処理
			DispatchMessage(&msg);//プロシージャにメッセージを送る
		}

		//×ボタンで終了メッセージが来たらゲームループを抜ける
		if (msg.message == WM_QUIT)
		{
			break;
		}

#pragma region DirectX毎フレーム処理

		//DirectX毎フレーム処理　ここから
		
#pragma region キーボード情報の取得

		////キーボード情報の取得開始
		//directXinit.GetKeyBoard()->Acquire();

		////前フレームのキーボード入力を保存
		//for (int i = 0; i < 256; i++)
		//{
		//	oldKey[i] = key[i];
		//}

		////全キーの入力情報を取得する
		//directXinit.GetKeyBoard()->GetDeviceState(sizeof(key), key);

#pragma endregion

		directXinit->DrawStart();

#pragma region 描画コマンド

		// 4.描画コマンドここから
		
		//ビューポート設定
		//分割分のビューポートを用意(見にくいので減らした)
		D3D12_VIEWPORT viewport{};

		viewport.Width = window_width;//横幅
		viewport.Height = window_height;//縦幅
		viewport.TopLeftX = 0;//左上X
		viewport.TopLeftY = 0;//左上Y
		viewport.MinDepth = 0.1f;//最少深度(0でよい)
		viewport.MaxDepth = 1.0f;//最大深度(１でよい)

		//コマンドリストに追加
		directXinit->GetcmdList()->RSSetViewports(1, &viewport);
		

		//シザー矩形設定
		D3D12_RECT scissorrect{};

		scissorrect.left = 0;//切り抜き座標左
		scissorrect.right = scissorrect.left + window_width;//切り抜き座標右
		scissorrect.top = 0;//切り抜き座標上
		scissorrect.bottom = scissorrect.top + window_height;//切り抜き座標下

		directXinit->GetcmdList()->RSSetScissorRects(1, &scissorrect);

		//プリミティブ形状(三角形リスト)
		directXinit->GetcmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		
		charactorObj.Draw(directXinit->GetcmdList().Get(), PipeLineRuleFlag, ChangeSquareFlag);
		
		// 4.描画コマンドここまで

#pragma endregion

		directXinit->DrawEnd();

		//DirectX毎フレーム処理　ここまで

#pragma endregion

#pragma region 更新処理

		if (directXinit->TriggerKey(DIK_2))
		{
			PipeLineRuleFlag = !PipeLineRuleFlag;
		}

		if (directXinit->TriggerKey(DIK_1))
		{
			ChangeSquareFlag = !ChangeSquareFlag;
		}


		if (directXinit->PushKey(DIK_D) or directXinit->PushKey(DIK_A)or directXinit->PushKey(DIK_W)or directXinit->PushKey(DIK_S))
		{
			if (directXinit->PushKey(DIK_D))
			{
				angle += XMConvertToRadians(1.0f);
			}
			else if (directXinit->PushKey(DIK_A))
			{
				angle -= XMConvertToRadians(1.0f);
			}
			else if (directXinit->PushKey(DIK_W))
			{
				angleY += XMConvertToRadians(500.0f);
			}
			else if (directXinit->PushKey(DIK_S))
			{
				angleY -= XMConvertToRadians(500.0f);
			}

			charactorObj.matViewUpdata({ -400 * sinf(angle),angleY,-400 * cosf(angle) }, { 0,0,0 }, { 0,1,0 });
			

		}

		if (directXinit->PushKey(DIK_UP) or directXinit->PushKey(DIK_DOWN) or directXinit->PushKey(DIK_RIGHT) or directXinit->PushKey(DIK_LEFT))
		{
			if (directXinit->PushKey(DIK_UP))
			{
				pos.z += 1.0f;
			}
			else if (directXinit->PushKey(DIK_DOWN))
			{
				pos.z -= 1.0f;
			}
			else if (directXinit->PushKey(DIK_RIGHT))
			{
				pos.x += 1.0f;
			}
			else if (directXinit->PushKey(DIK_LEFT))
			{
				pos.x -= 1.0f;
			}

			charactorObj.SetTrans(pos);

			charactorObj.matWorldUpdata();

		}


		//色変更
		/*if (Red > 0 and Blue <= 0)
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
		}*/

#pragma endregion

#pragma region 描画処理

		

#pragma endregion

		

	}
	
	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);

	directXinit->instanceDelete();

	return 0;

	
}
