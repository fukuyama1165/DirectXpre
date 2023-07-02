#pragma once
//D3Dコンパイラ
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include <DirectXMath.h>
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

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

	static void Staticlnitialize();

	//インスタンス生成
	//static LightGroup* Create();

	void Init();

	void Update();

	void Draw(uint32_t rootParameterIndex);

	//定数バッファ転送
	void TransferConstBuffer();

	void SetAmbientColor(const XMFLOAT3& color);

	void SetDirLightActive(uint32_t index, bool active);

	void SetDirLightDir(uint32_t index, const XMVECTOR& lightdir);

	void SetDirLightColor(uint32_t index, const XMFLOAT3& lightcolor);

	void SetPointLightActive(uint32_t index, bool active);

	void SetPointLightPos(uint32_t index, const Vector3& lightPos);

	void SetPointLightColor(uint32_t index, const Vector3& lightcolor);
	
	void SetPointLightAtten(uint32_t index, const Vector3& lightAtten);

	void SetSpotLightActive(uint32_t index, bool active);

	void SetSpotLightDir(uint32_t index, const Vector3& lightdir);

	void SetSpotLightPos(uint32_t index, const Vector3& lightPos);

	void SetSpotLightColor(uint32_t index, const Vector3& lightColor);

	void SetSpotLightAtten(uint32_t index, const Vector3& lightAtten);

	void SetSpotLightFactorAngle(uint32_t index, const Vector2& lightFactorAngle);

	void DefaultLightSetting();

private:

	//定数バッファ
	void constantBuffGeneration();

	//定数バッファ用のリソース設定関数
	D3D12_RESOURCE_DESC constBuffResourceGeneration(uint32_t size);

public:

	static const uint16_t SDirLightNum_ = 3;

	static const uint16_t SPointLightNum_ = 3;

	static const uint16_t SSpotLightNum_ = 3;

public:

	struct ConstBufferData
	{
		//環境光の色
		XMFLOAT3 ambientColor;
		float pad1;
		//平行光源用
		DirectionalLight::ConstBufferData dirLights[SDirLightNum_];
		//点光源用
		PointLight::ConstBufferData pointLights[SPointLightNum_];

		SpotLight::ConstBufferData spotLights[SSpotLightNum_];

	}; 

private:

	HRESULT result_;

	//static Microsoft::WRL::ComPtr<ID3D12Device> dev;


	

	//ComPtr<ID3D12Resource> constBuff;

	XMFLOAT3 ambientColor_ = { 1,1,1 };

	DirectionalLight dirLights_[SDirLightNum_];

	PointLight pointLights_[SPointLightNum_];

	SpotLight SpotLights_[SSpotLightNum_];

	bool dirty_ = false;

	//定数バッファ
	ComPtr<ID3D12Resource>constBuff_ = nullptr;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp_{};
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc_{};

};

