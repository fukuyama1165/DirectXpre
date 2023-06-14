#pragma once

//D3Dコンパイラ
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<d3d12.h>

#pragma comment(lib,"d3d12.lib")

#include<DirectXMath.h>
#include <wrl.h>

class DirectionalLight
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
		uint32_t active;

	};

private://静的メンバ変数

	//デバイス
	//static Microsoft::WRL::ComPtr<ID3D12Device> dev;

public://静的メンバ関数

	static void StaticInitialize();

public:

	//インスタンス生成
	//static DirectionalLight* Create();

	//初期化
	void Init();

	//更新
	void Update();

	//描画
	void Draw(uint32_t rootParameterIndex);

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

	//有効フラグをセット
	inline void SetIsActive(bool flag) { isActive_ = flag; };

	//有効フラグを取得
	inline bool GetIsActive() { return isActive_; };

	XMVECTOR GetLightDir() { return lightDir_; };

	XMFLOAT3 GetLightColor() { return lightColor_; };

private://メンバ関数(privete)

	//定数バッファ
	void constantBuffGeneration();

	//定数バッファ用のリソース設定関数
	D3D12_RESOURCE_DESC constBuffResourceGeneration(uint32_t size);


private://メンバ変数

	HRESULT result_;

	//有効フラグ
	bool isActive_ = false;

	//定数バッファ
	ComPtr<ID3D12Resource>constBuff_ = nullptr;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp_{};
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc_{};

	//ライト光線方向(単位ベクトル)
	XMVECTOR lightDir_ = { 1,0,0,0 };

	//ライト色
	XMFLOAT3 lightColor_ = { 1,1,1 };

	//ダーティフラグ
	bool dirty_ = false;

};

