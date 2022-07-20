#include "DirectXInit.h"

DirectXInit* DirectXInit::instance = nullptr;
using namespace Microsoft::WRL;


DirectXInit::~DirectXInit()
{
}

DirectXInit* DirectXInit::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new DirectXInit();
	}
	return instance;
}

void DirectXInit::instanceDelete()
{
	delete(instance);
}

void DirectXInit::Init(WNDCLASSEX w, HWND hwnd, const int win_width, const int win_height)
{

#pragma region デバックレイヤーの有効化



#ifdef _DEBUG

	//デバックレイヤーをオンに
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}

#endif // DEBUG

#pragma endregion

	
	

	//グラフィックアダプター
	GraphicAdapterGeneration();

	//コマンドリスト
	CommandListGeneration();

	//コマンドキュー
	CommandQueueGeneration();

	//スワップチェーン
	SwapChainGeneration(hwnd,win_width,win_height);

	DepthBuffGeneration(win_width, win_height);

	//レンダーターゲットビュー
	RTVGeneration();

	//フェンス
	FenceGeneration();

	//DirectInput(できたら専用クラスを作る)
	DirectInputGeneration(w, hwnd);
}

void DirectXInit::GraphicAdapterGeneration()
{
#pragma region グラフィックアダプター設定

	//DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	//アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter1>> adapters;

	//ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter1> tmpAdapter = nullptr;
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
			tmpAdapter = adapters[i].Get();//採用
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
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)//ちょくちょくresultの中身がS_OKであることを確認すること
		{
			//デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

#pragma endregion これのクラス化で複数生成に注意
}

void DirectXInit::CommandListGeneration()
{
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
		cmdAllocator.Get(),
		nullptr,
		IID_PPV_ARGS(&cmdList)
	);

#pragma endregion
}

void DirectXInit::CommandQueueGeneration()
{
#pragma region コマンドキュー辺

	//標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};//構造体の中身を{}で０でクリアしている(microsoftの標準が0のため)

	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

#pragma endregion
}

void DirectXInit::SwapChainGeneration(HWND hwnd, const int win_width, const int win_height)
{
#pragma region スワップチェーン辺

	//各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = win_width;
	swapchainDesc.Height = win_height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色情報の書式
	swapchainDesc.SampleDesc.Count = 1;//マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;//バックバッファ用
	swapchainDesc.BufferCount = 2;//バッファ数を2つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//comptrの変換用変数を定義
	ComPtr<IDXGISwapChain1> swapChain1;

	dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapChain1
	);

	//生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換
	swapChain1.As(&swapchain);

#pragma endregion
}

void DirectXInit::DepthBuffGeneration(const int win_width, const int win_height)
{

	//リソース設定
	D3D12_RESOURCE_DESC depthResourceDesc{};

	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = win_width;//レンダーターゲットに合わせる
	depthResourceDesc.Height = win_height;//レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//デプステンシル

	//深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;//深度値1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット

	//リソース生成
	
	result = dev->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff)
	);

	//深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//デプスステンシルビュー
	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	//深度ビュー生成
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);

}

void DirectXInit::RTVGeneration()
{
#pragma region レンダーターゲットビュー辺

	//レンダーターゲットビューはデスクリプタヒープから生成されるので
	//デスクリプタヒープを先に生成する

	//各種設定をしてデスクリプタヒープを生成
	
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビュー
	heapDesc.NumDescriptors = 2;//裏表の２つ
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	//レンダーターゲットビューの生成部分


	//裏表の2つ分について
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
			backBuffers[i].Get(),
			nullptr,
			handle
		);

	}

#pragma endregion バックバッファを描画キャンバスとして扱う為のオブジェクト
}


void DirectXInit::DrawStart()
{
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
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get();//バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;//表示から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;//描画
	cmdList->ResourceBarrier(1, &barrierDesc);

	// 2.描画先指定
	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
	//cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvHandle);

	// 3.画面クリア

	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}


void DirectXInit::DrawEnd()
{
	// 5.リソースバリアを戻す(書き込める状態だと描画できないので描画できるようにする)
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;//描画
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;//表示に
	cmdList->ResourceBarrier(1, &barrierDesc);

	//命令のクローズ(ここで描画の命令はそろったので実行にうつす)
	cmdList->Close();
	//コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { cmdList.Get()};
	cmdQueue->ExecuteCommandLists(1, cmdLists);

	//画面に表示するバッファをフリップ(裏表の入れ替え)
	swapchain->Present(1, 0);

	//コマンド完了待ち(これを書かないとすぐ処理に移行してしまうため)
	//コマンドの実行完了を待つ
	cmdQueue->Signal(fence.Get(), ++fenceVel);
	if (fence->GetCompletedValue() != fenceVel)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVel, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	cmdAllocator->Reset();
	cmdList->Reset(cmdAllocator.Get(), nullptr);
}

void DirectXInit::clearColorChange(float R, float G, float B, float A)
{
	clearColor[0] = R;
	clearColor[1] = G;
	clearColor[2] = B;
	clearColor[3] = A;
}

bool DirectXInit::PushKey(BYTE CheckKey)
{
	return key[CheckKey];
}

bool DirectXInit::TriggerKey(BYTE CheckKey)
{
	return key[CheckKey] and oldKey[CheckKey] == 0;
}


void DirectXInit::FenceGeneration()
{
#pragma region フェンス辺
	//フェンスを生成
	result = dev->CreateFence(fenceVel, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

#pragma endregion CPUとGPUで同期をとるためのDirectXの仕組み
}

void DirectXInit::DirectInputGeneration(WNDCLASSEX w, HWND hwnd)
{
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
}

//get関数群
ComPtr<ID3D12Device> DirectXInit::Getdev()
{
	return dev;
}

IDirectInputDevice8* DirectXInit::GetKeyBoard()
{
	return keyboard;
}

ComPtr<ID3D12GraphicsCommandList> DirectXInit::GetcmdList()
{
	return cmdList;
}
