#pragma once

/**
 * @file DirectionalLight.h
 * @brief 環境光
 * @author フクヤマ
 * @date 2023_12/29
 */

//D3Dコンパイラ
#include<d3dcompiler.h>

#include<d3d12.h>

#include<DirectXMath.h>
#include <wrl.h>

#include "Vector3.h"
#include "Vector4.h"

class DirectionalLight
{
private://エイリアス

	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	

public://サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferData
	{

		Vector4 lightv;
		Vector3 lightColor;
		float pad;
		float active;
		float pad1;
		float pad2;
		float pad3;

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
	void SetLightDir(const Vector4& lightdir);

	/// <summary>
	/// ライト色をセット
	/// </summary>
	/// <param name="lightcolor">ライト色</param>
	void SetLightColor(const Vector3& lightcolor);

	//有効フラグをセット
	void SetIsActive(bool flag) { isActive_ = flag; };

	//有効フラグを取得
	bool GetIsActive() { return isActive_; };

	Vector4 GetLightDir() { return lightDir_; };

	Vector3 GetLightColor() { return lightColor_; };

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
	Vector4 lightDir_ = { 1,0,0,0 };

	//ライト色
	Vector3 lightColor_ = { 1,1,1 };

	//ダーティフラグ
	bool dirty_ = false;

};

