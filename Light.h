#pragma once

//D3Dコンパイラ
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<d3d12.h>

#pragma comment(lib,"d3d12.lib")

#include<DirectXMath.h>
#include <wrl.h>

class Light
{
private://エイリアス

	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferData
	{

		XMVECTOR lightv;
		XMFLOAT3 lightColor;

	};

private://静的メンバ変数

	//デバイス
	static ID3D12Device* dev;

public://静的メンバ関数

	static void StaticInitialize(ID3D12Device* device);

public:

	//インスタンス生成
	static Light* Create();

	//初期化
	void Init();

	//更新
	void Update();

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList,UINT rootParameterIndex);

	//定数バッファ転送
	void TransferConstBuffer();

	/// <summary>
	/// ライト方向をセット
	/// </summary>
	/// <param name="lightdir">ライト方向</param>
	void SetLightDir(const XMVECTOR& lightdir);

	/// <summary>
	/// ライト色をセット
	/// </summary>
	/// <param name="lightcolor">ライト色</param>
	void SetLightColor(const XMFLOAT3& lightcolor);

private://メンバ関数(privete)

	//定数バッファ
	void constantBuffGeneration(ID3D12Device* dev);

	//定数バッファ用のリソース設定関数
	D3D12_RESOURCE_DESC constBuffResourceGeneration(int size);


private://メンバ変数

	HRESULT result;

	//定数バッファ
	ComPtr<ID3D12Resource>constBuff = nullptr;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};

	//ライト光線方向(単位ベクトル)
	XMVECTOR lightDir = { 1,0,0,0 };

	//ライト色
	XMFLOAT3 lightColor = { 1,1,1 };

	//ダーティフラグ
	bool dirty = false;

};

