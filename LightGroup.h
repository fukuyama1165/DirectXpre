#pragma once
//D3Dコンパイラ
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include <DirectXMath.h>
#include "DirectionalLight.h"

class LightGroup
{
private:

	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	static void Staticlnitialize(ID3D12Device* device);

	//インスタンス生成
	static LightGroup* Create();

	void Init();

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList,UINT rootParameterIndex);

	//定数バッファ転送
	void TransferConstBuffer();

	void SetAmbientColor(const XMFLOAT3& color);

	void SetDirLightActive(int index, bool active);

	void SetDirLightDir(int index, const XMVECTOR& lightdir);

	void SetDirLightColor(int index, const XMFLOAT3& lightcolor);

	void DefaultLightSetting();

private:

	//定数バッファ
	void constantBuffGeneration(ID3D12Device* dev);

	//定数バッファ用のリソース設定関数
	D3D12_RESOURCE_DESC constBuffResourceGeneration(int size);

public:

	static const int DirLightNum = 3;

public:

	struct ConstBufferData
	{
		//環境光の色
		XMFLOAT3 ambientColor;
		float pad1;
		//平行光源用
		DirectionalLight::ConstBufferData dirLights[DirLightNum];

	}; 

private:

	HRESULT result;

	static ID3D12Device* dev;


	

	//ComPtr<ID3D12Resource> constBuff;

	XMFLOAT3 ambientColor = { 1,1,1 };

	DirectionalLight dirLights[DirLightNum];

	bool dirty = false;

	//定数バッファ
	ComPtr<ID3D12Resource>constBuff = nullptr;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};

};

