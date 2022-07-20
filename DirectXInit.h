#pragma once
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

#include <DirectXTex.h>

#include "DrawingObj.h"

//ComPtr用インクルード
#include <wrl.h>

#include <dxgidebug.h>


class DirectXInit
{
public:

	static DirectXInit* GetInstance();

	/*static void Create();

	static void Destroy()*/;
	
	//初期化
	
	void Init(WNDCLASSEX w, HWND hwnd,const int win_width, const int win_height);

	//グラフィックアダプター
	void GraphicAdapterGeneration();

	//コマンドリスト
	void CommandListGeneration();

	//コマンドキュー
	void CommandQueueGeneration();

	//スワップチェーン
	void SwapChainGeneration(HWND hwnd, const int win_width, const int win_height);

	//深度バッファ
	void DepthBuffGeneration(const int win_width, const int win_height);

	//レンダーターゲットビュー
	void RTVGeneration();

	//フェンス
	void FenceGeneration();

	//DirectInput(できたら専用クラスを作る)
	void DirectInputGeneration(WNDCLASSEX w, HWND hwnd);

	//devを返す関数
	Microsoft::WRL::ComPtr<ID3D12Device> Getdev();

	//keyboardを返す関数
	IDirectInputDevice8* GetKeyBoard();

	//cmdListを返す関数
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetcmdList();

	//描画の初めの部分
	void DrawStart();

	//描画終わりの部分
	void DrawEnd();


	//画面のクリアカラー変更関数
	void clearColorChange(float R, float G, float B, float A);

	//inputでの入力反応関数(inputClassを作ったらこれも移動する)

	//キーボードのキーを押したら反応する関数(長押しも反応)
	bool PushKey(BYTE CheckKey);

	//キーボードのキーを押した瞬間に反応する関数(長押し反応しない)
	bool TriggerKey(BYTE CheckKey);

	void instanceDelete();

private:

	DirectXInit()=default;
	~DirectXInit();

	DirectXInit(const DirectXInit&) = delete;
	DirectXInit& operator=(const DirectXInit&) = delete;

	static DirectXInit* instance;

private:

	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Device> dev = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff = nullptr;

	//バックバッファ
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>backBuffers{2};

	//デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};

	//キーボードデバイス
	IDirectInputDevice8* keyboard = nullptr;

	//フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVel = 0;

	//全キーの入力情報を取得する為の変数
	BYTE key[256] = {};
	BYTE oldKey[256] = {};

	//背景色変更するために外に配置
	float clearColor[4]={0.1f,0.25f,0.5f,0.0f};//青っぽい色(画面クリアするときの色)

	//リソースバリア
	D3D12_RESOURCE_BARRIER barrierDesc{};

	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> dsvHeap = nullptr;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

	IDXGIDebug* pDxgiDebug;

	Microsoft::WRL::ComPtr<ID3D12Debug> pD3dDebug;

};

