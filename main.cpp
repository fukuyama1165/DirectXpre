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

const float PI = 3.141592653589;

#include <DirectXTex.h>

#include "DrawingObj.h"


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

//背景色変更するために外に配置
float clearColor[] = { 0.1f,0.25f,0.5f,0.0f };//青っぽい色(画面クリアするときの色)

//画面のクリアカラー変更関数
void clearColorChange(float R, float G, float B, float A);

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


#pragma region DirectX初期化処理
//ここから	
#pragma region デバックレイヤーの有効化



#ifdef _DEBUG

	//デバックレイヤーをオンに
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}

#endif // DEBUG

#pragma endregion

#pragma region 初期化用変数宣言

	HRESULT result;
	ID3D12Device* dev = nullptr;
	IDXGIFactory6* dxgiFactory = nullptr;
	IDXGISwapChain4* swapchain = nullptr;
	ID3D12CommandAllocator* cmdAllocator = nullptr;
	ID3D12GraphicsCommandList* cmdList = nullptr;
	ID3D12CommandQueue* cmdQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeaps = nullptr;

#pragma endregion

#pragma region グラフィックアダプター設定


	
	//DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	//アダプターの列挙用
	std::vector<IDXGIAdapter1*> adapters;

	//ここに特定の名前を持つアダプターオブジェクトが入る
	IDXGIAdapter1* tmpAdapter = nullptr;
	for (int i = 0; dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);//動的配列に追加する
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc);//アダプターの情報を取得

		//ソフトウェアデバイスを回避
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		std::wstring strDesc = adesc.Description;//アダプター名

		//Intel UHD Graphice(オンボードグラフィック)を回避
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];//採用
			break;
		}

	}

	//採用したグラフィックデバイスを操作するためのオブジェクトを生成
	//※普通は一つしか作らないのでクラス化等する際に複数生成しないように

	//対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (int i = 0; i < _countof(levels); i++)
	{
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)//ちょくちょくresultの中身がS_OKであることを確認すること
		{
			//デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

#pragma endregion これのクラス化で複数生成に注意

#pragma region コマンドリスト辺

	//コマンドリストを生成するにはコマンドアロケータが必要
	//なので先にコマンドアロケータを生成

	//コマンドアロケータを生成
	result = dev->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator)
	);

	//コマンドリストを生成
	result = dev->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator,
		nullptr,
		IID_PPV_ARGS(&cmdList)
	);

#pragma endregion

#pragma region コマンドキュー辺

	//標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};//構造体の中身を{}で０でクリアしている(microsoftの標準が0のため)

	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

#pragma endregion

#pragma region スワップチェーン辺

	//各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = 1280;
	swapchainDesc.Height = 720;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色情報の書式
	swapchainDesc.SampleDesc.Count = 1;//マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;//バックバッファ用
	swapchainDesc.BufferCount = 2;//バッファ数を2つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue,
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&swapchain
	);

#pragma endregion

#pragma region レンダーターゲットビュー辺

	//レンダーターゲットビューはデスクリプタヒープから生成されるので
	//デスクリプタヒープを先に生成する

	//各種設定をしてデスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビュー
	heapDesc.NumDescriptors = 2;//裏表の２つ
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	//レンダーターゲットビューの生成部分

	//裏表の2つ分について
	std::vector<ID3D12Resource*> backBuffers(2);
	for (int i = 0; i < 2; i++)
	{
		//スワップチェーンからバッファを取得
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		//デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		//裏か表かでアドレスがずれる
		handle.ptr += i * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
		//レンダーターゲットビューの生成
		dev->CreateRenderTargetView(
			backBuffers[i],
			nullptr,
			handle
		);

	}

#pragma endregion バックバッファを描画キャンバスとして扱う為のオブジェクト

#pragma region フェンス辺

	//フェンスを生成
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVel = 0;

	result = dev->CreateFence(fenceVel, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

#pragma endregion CPUとGPUで同期をとるためのDirectXの仕組み

#pragma region DirectInoutオブジェクトの生成

	//DirectInputの初期化
	IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(
		w.hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput,
		nullptr
	);
	assert(SUCCEEDED(result));

#pragma endregion 全体の初期化であり入力デバイスが増えてもこのオブジェクトは増やさなくてもいい

#pragma region キーボードデバイスの生成

	//キーボードデバイスの生成
	IDirectInputDevice8* keyboard = nullptr;
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region ゲームパッドの入力

	//DIDEVICEINSTANCE* gamePadDID = {};
	//result = directInput->EnumDevices(
	//	DI8DEVTYPE_JOYSTICK,//JOYSTICKだと認識しないことがあるらしい(確認できるなら確認すること)
	//	DeviceFindCallBack,
	//	&gamePadDID,
	//	DIEDFL_ATTACHEDONLY
	//);
	//assert(SUCCEEDED(result));

	//IDirectInputDevice8* gamePadDirct = nullptr;
	//result = directInput->CreateDevice(gamePadDID->guidInstance, &gamePadDirct, NULL);
	//assert(SUCCEEDED(result));


#pragma endregion(中止)

#pragma region 入力データ形式のセット

	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));

#pragma endregion


#pragma region 排他制御レベルのセット

	result = keyboard->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND/*画面が手前にある場合のみ入力を受け付ける*/ |
		DISCL_NONEXCLUSIVE/*デバイスをこのアプリだけで占有しない*/ |
		DISCL_NOWINKEY/*windowキーを無効にする*/);
	assert(SUCCEEDED(result));

#pragma endregion


	//DirectX初期化処理　ここまで

#pragma endregion
	


	DrawingObj charactorObj(window_width, window_height, { 0.0f,100.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 100.0f,100.0f,0.0f }, { 100.0f,0.0f,0.0f });


	charactorObj.basicInit(dev);



	//パイプラインステート切り替え用フラグ
	bool PipeLineRuleFlag = true;

	//四角形に変更するときのフラグ
	bool ChangeSquareFlag = true;

	//全キーの入力情報を取得する為の変数
	BYTE key[256] = {};
	BYTE oldKey[256] = {};

	
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

		//キーボード情報の取得開始
		keyboard->Acquire();

		//前フレームのキーボード入力を保存
		for (int i = 0; i < 256; i++)
		{
			oldKey[i] = key[i];
		}

		//全キーの入力情報を取得する
		keyboard->GetDeviceState(sizeof(key), key);

#pragma endregion

		

		//リソースバリア辺

		//バックバッファの番号を取得(２つなので0番か1番)
		UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

		// 1.リソースバリアで書き込み可能に変更
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = backBuffers[bbIndex];//バックバッファを指定
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;//表示から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;//描画
		cmdList->ResourceBarrier(1, &barrierDesc);

		// 2.描画先指定
		//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
		cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);

		// 3.画面クリア
		
		cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

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
		cmdList->RSSetViewports(1, &viewport);
		

		//シザー矩形設定
		D3D12_RECT scissorrect{};

		scissorrect.left = 0;//切り抜き座標左
		scissorrect.right = scissorrect.left + window_width;//切り抜き座標右
		scissorrect.top = 0;//切り抜き座標上
		scissorrect.bottom = scissorrect.top + window_height;//切り抜き座標下

		cmdList->RSSetScissorRects(1, &scissorrect);

		//プリミティブ形状(三角形リスト)
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		
		charactorObj.Draw(cmdList, PipeLineRuleFlag, ChangeSquareFlag);
		
		// 4.描画コマンドここまで

#pragma endregion

		// 5.リソースバリアを戻す(書き込める状態だと描画できないので描画できるようにする)
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;//描画
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;//表示に
		cmdList->ResourceBarrier(1, &barrierDesc);

		//命令のクローズ(ここで描画の命令はそろったので実行にうつす)
		cmdList->Close();
		//コマンドリストの実行
		ID3D12CommandList* cmdLists[] = { cmdList };
		cmdQueue->ExecuteCommandLists(1, cmdLists);

		//画面に表示するバッファをフリップ(裏表の入れ替え)
		swapchain->Present(1, 0);

		//コマンド完了待ち(これを書かないとすぐ処理に移行してしまうため)
		//コマンドの実行完了を待つ
		cmdQueue->Signal(fence, ++fenceVel);
		if (fence->GetCompletedValue() != fenceVel)
		{
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			fence->SetEventOnCompletion(fenceVel, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}

		cmdAllocator->Reset();
		cmdList->Reset(cmdAllocator, nullptr);


		//DirectX毎フレーム処理　ここまで

#pragma endregion

#pragma region 更新処理

		X1 = 0.0f;
		Y1 = 0.0f;
		rotate = 0.0f;
		scaleX = 1.0f;
		scaleY = 1.0f;

		if (key[DIK_D])
		{
			OutputDebugStringA("Hit D\n");
		}

		if (key[DIK_2] and oldKey[DIK_2] == 0)
		{
			PipeLineRuleFlag = !PipeLineRuleFlag;
			OutputDebugStringA("Hit W\n");
		}

		if (key[DIK_W] )
		{
			X2 += 0.001f;
		}

		if (key[DIK_S] )
		{
			X2 -= 0.001f;
		}

		if (key[DIK_SPACE])
		{
			clearColorChange(0.9f, 0.2f, 0.5f, 0.0f);
		}
		else
		{
			clearColorChange(0.1f, 0.25f, 0.5f, 0.0f);
		}

		if (key[DIK_1] and oldKey[DIK_1] == 0)
		{
			ChangeSquareFlag = !ChangeSquareFlag;
		}

		if (key[DIK_Z] )
		{
			
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

	//ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);

	

	return 0;

	
}

void clearColorChange(float R, float G, float B, float A)
{
	clearColor[0] = R;
	clearColor[1] = G;
	clearColor[2] = B;
	clearColor[3] = A;
}

