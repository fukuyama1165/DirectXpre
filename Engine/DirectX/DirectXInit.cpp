﻿#include "DirectXInit.h"
#include "WinApp.h"


using namespace Microsoft::WRL;


DirectXInit::~DirectXInit()
{
}

DirectXInit* DirectXInit::GetInstance()
{
	static DirectXInit instance;
	return &instance;
}



void DirectXInit::Init()
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

	fps.initialize();


	//グラフィックアダプター
	GraphicAdapterGeneration();

	//コマンドリスト
	CommandListGeneration();

	//コマンドキュー
	CommandQueueGeneration();

	//スワップチェーン
	SwapChainGeneration();

	DepthBuffGeneration();

	//レンダーターゲットビュー
	RTVGeneration();

	//フェンス
	FenceGeneration();

}

void DirectXInit::GraphicAdapterGeneration()
{
#pragma region グラフィックアダプター設定

	//DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory_));

	//アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter1>> adapters;

	//ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter1> tmpAdapter = nullptr;
	for (uint16_t i = 0; dxgiFactory_->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);//動的配列に追加する
	}

	for (uint16_t i = 0; i < adapters.size(); i++)
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

	for (uint16_t i = 0; i < _countof(levels); i++)
	{
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev_));
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
	result = dev_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator_)
	);

	//コマンドリストを生成
	result = dev_->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator_.Get(),
		nullptr,
		IID_PPV_ARGS(&cmdList_)
	);

#pragma endregion
}

void DirectXInit::CommandQueueGeneration()
{
#pragma region コマンドキュー辺

	//標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};//構造体の中身を{}で０でクリアしている(microsoftの標準が0のため)

	dev_->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue_));

#pragma endregion
}

void DirectXInit::SwapChainGeneration()
{
#pragma region スワップチェーン辺

	//各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = WinApp::GetInstance()->getWindowSizeWidth();
	swapchainDesc.Height = WinApp::GetInstance()->getWindowSizeHeight();
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色情報の書式
	swapchainDesc.SampleDesc.Count = 1;//マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;//バックバッファ用
	swapchainDesc.BufferCount = 2;//バッファ数を2つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//comptrの変換用変数を定義
	ComPtr<IDXGISwapChain1> swapChain1;

	dxgiFactory_->CreateSwapChainForHwnd(
		cmdQueue_.Get(),
		WinApp::GetInstance()->getHwnd(),
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapChain1
	);

	//生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換
	swapChain1.As(&swapchain_);

#pragma endregion
}

void DirectXInit::DepthBuffGeneration()
{

	//リソース設定
	D3D12_RESOURCE_DESC depthResourceDesc{};

	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = WinApp::GetInstance()->getWindowSizeWidth();//レンダーターゲットに合わせる
	depthResourceDesc.Height = WinApp::GetInstance()->getWindowSizeHeight();//レンダーターゲットに合わせる
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

	result = dev_->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_)
	);

	//深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//デプスステンシルビュー
	result = dev_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));

	//深度ビュー生成
	dsvDesc_.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc_.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev_->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc_,
		dsvHeap_->GetCPUDescriptorHandleForHeapStart()
	);

}

void DirectXInit::RTVGeneration()
{
#pragma region レンダーターゲットビュー辺

	//レンダーターゲットビューはデスクリプタヒープから生成されるので
	//デスクリプタヒープを先に生成する

	//各種設定をしてデスクリプタヒープを生成

	heapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビュー
	heapDesc_.NumDescriptors = 2;//裏表の２つ
	dev_->CreateDescriptorHeap(&heapDesc_, IID_PPV_ARGS(&rtvHeaps_));

	//レンダーターゲットビューの生成部分


	//裏表の2つ分について
	for (uint16_t i = 0; i < 2; i++)
	{
		//スワップチェーンからバッファを取得
		result = swapchain_->GetBuffer(i, IID_PPV_ARGS(&backBuffers_[i]));
		//デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps_->GetCPUDescriptorHandleForHeapStart();
		//裏か表かでアドレスがずれる
		handle.ptr += i * dev_->GetDescriptorHandleIncrementSize(heapDesc_.Type);

		//レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};

		//シェーダーの計算結果をSRGBに変換して書き込む
		renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		//レンダーターゲットビューの生成
		dev_->CreateRenderTargetView(
			backBuffers_[i].Get(),
			&renderTargetViewDesc,
			handle
		);

	}

#pragma endregion バックバッファを描画キャンバスとして扱う為のオブジェクト
}


void DirectXInit::DrawStart()
{



	//リソースバリア辺

	//バックバッファの番号を取得(２つなので0番か1番)
	uint32_t bbIndex = swapchain_->GetCurrentBackBufferIndex();

	// 1.リソースバリアで書き込み可能に変更
	barrierDesc_.Transition.pResource = backBuffers_[bbIndex].Get();//バックバッファを指定
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;//表示から
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;//描画
	cmdList_->ResourceBarrier(1, &barrierDesc_);

	// 2.描画先指定
	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeaps_->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * dev_->GetDescriptorHandleIncrementSize(heapDesc_.Type);
	//cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
	cmdList_->OMSetRenderTargets(1, &rtvH, false, &dsvHandle);

	// 3.画面クリア

	cmdList_->ClearRenderTargetView(rtvH, clearColor_, 0, nullptr);
	cmdList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//ビューポート設定
	//分割分のビューポートを用意(見にくいので減らした)
	D3D12_VIEWPORT viewport{};

	viewport.Width = (float)WinApp::GetInstance()->getWindowSizeWidth();//横幅
	viewport.Height = (float)WinApp::GetInstance()->getWindowSizeHeight();//縦幅
	viewport.TopLeftX = 0;//左上X
	viewport.TopLeftY = 0;//左上Y
	viewport.MinDepth = 0.1f;//最少深度(0でよい)
	viewport.MaxDepth = 1.0f;//最大深度(１でよい)

	//コマンドリストに追加
	DirectXInit::GetInstance()->GetcmdList()->RSSetViewports(1, &viewport);


	//シザー矩形設定
	D3D12_RECT scissorrect{};

	scissorrect.left = 0;//切り抜き座標左
	scissorrect.right = scissorrect.left + WinApp::GetInstance()->getWindowSizeWidth();//切り抜き座標右
	scissorrect.top = 0;//切り抜き座標上
	scissorrect.bottom = scissorrect.top + WinApp::GetInstance()->getWindowSizeHeight();//切り抜き座標下

	DirectXInit::GetInstance()->GetcmdList()->RSSetScissorRects(1, &scissorrect);

}


void DirectXInit::DrawEnd()
{
	// 5.リソースバリアを戻す(書き込める状態だと描画できないので描画できるようにする)
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;//描画
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;//表示に
	cmdList_->ResourceBarrier(1, &barrierDesc_);

	//命令のクローズ(ここで描画の命令はそろったので実行にうつす)
	cmdList_->Close();
	//コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { cmdList_.Get() };
	cmdQueue_->ExecuteCommandLists(1, cmdLists);

	//画面に表示するバッファをフリップ(裏表の入れ替え)
	swapchain_->Present(1, 0);

	//コマンド完了待ち(これを書かないとすぐ処理に移行してしまうため)
	HRESULT endResult;
	//コマンドの実行完了を待つ
	endResult = cmdQueue_->Signal(fence_.Get(), ++fenceVel_);
	if (fence_->GetCompletedValue() != fenceVel_)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		if (event != 0)
		{
			fence_->SetEventOnCompletion(fenceVel_, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}

	fps.update();

	cmdAllocator_->Reset();
	cmdList_->Reset(cmdAllocator_.Get(), nullptr);
}

void DirectXInit::clearColorChange(float R, float G, float B, float A)
{
	clearColor_[0] = R;
	clearColor_[1] = G;
	clearColor_[2] = B;
	clearColor_[3] = A;
}

void DirectXInit::FenceGeneration()
{
#pragma region フェンス辺
	//フェンスを生成
	result = dev_->CreateFence(fenceVel_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));

#pragma endregion CPUとGPUで同期をとるためのDirectXの仕組み
}



//get関数群
ComPtr<ID3D12Device> DirectXInit::Getdev()
{
	return dev_;
}

ComPtr<ID3D12GraphicsCommandList> DirectXInit::GetcmdList()
{
	return cmdList_;
}