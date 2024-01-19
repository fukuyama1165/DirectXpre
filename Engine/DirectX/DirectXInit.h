#pragma once
/**
 * @file DirectXInit.h
 * @brief DirectXの初期化に使用
 * @author フクヤマ
 * @date 2023_12/29
 */

//D3Dコンパイラ
#include<d3dcompiler.h>

#include<vector>
#include<string>

#include<d3d12.h>
#include<dxgi1_6.h>

//ComPtr用インクルード
#include <wrl.h>

#include <dxgidebug.h>

#include "FPS.h"


class DirectXInit
{
public:

	static DirectXInit* GetInstance();

	/*static void Create();
	static void Destroy()*/;

	//初期化

	void Init();

	//グラフィックアダプター
	void GraphicAdapterGeneration();

	//コマンドリスト
	void CommandListGeneration();

	//コマンドキュー
	void CommandQueueGeneration();

	//スワップチェーン
	void SwapChainGeneration();

	//深度バッファ
	void DepthBuffGeneration();

	//レンダーターゲットビュー
	void RTVGeneration();

	//フェンス
	void FenceGeneration();

	//devを返す関数
	Microsoft::WRL::ComPtr<ID3D12Device> Getdev();

	//keyboardを返す関数
	//IDirectInputDevice8* GetKeyBoard();

	//cmdListを返す関数
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetcmdList();

	size_t GetBackBufferCount()const { return backBuffers_.size(); };

	//描画の初めの部分
	void DrawStart();

	//描画終わりの部分
	void DrawEnd();


	//画面のクリアカラー変更関数
	void clearColorChange(float R, float G, float B, float A);

private:

	DirectXInit() = default;
	~DirectXInit();

	DirectXInit(const DirectXInit&) = delete;
	DirectXInit& operator=(const DirectXInit&) = delete;

	//static DirectXInit* instance;

private:

	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Device> dev_ = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory_ = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeaps_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff_ = nullptr;

	//バックバッファ
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>backBuffers_{ 2 };

	//デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc_{};

	//フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_ = nullptr;
	UINT64 fenceVel_ = 0;

	//背景色変更するために外に配置
	float clearColor_[4] = { 0.1f,0.25f,0.5f,0.0f };//青っぽい色(画面クリアするときの色)

	//リソースバリア
	D3D12_RESOURCE_BARRIER barrierDesc_{};

	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> dsvHeap_ = nullptr;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_ = {};

	IDXGIDebug* pDxgiDebug_;

	Microsoft::WRL::ComPtr<ID3D12Debug> pD3dDebug_;

	FPS fps;

};