#pragma once
//D3D�R���p�C��
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include <DirectXMath.h>
#include "DirectionalLight.h"

class LightGroup
{
private:

	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	static void Staticlnitialize();

	//�C���X�^���X����
	static LightGroup* Create();

	void Init();

	void Update();

	void Draw(UINT rootParameterIndex);

	//�萔�o�b�t�@�]��
	void TransferConstBuffer();

	void SetAmbientColor(const XMFLOAT3& color);

	void SetDirLightActive(uint32_t index, bool active);

	void SetDirLightDir(uint32_t index, const XMVECTOR& lightdir);

	void SetDirLightColor(uint32_t index, const XMFLOAT3& lightcolor);

	void DefaultLightSetting();

private:

	//�萔�o�b�t�@
	void constantBuffGeneration();

	//�萔�o�b�t�@�p�̃��\�[�X�ݒ�֐�
	D3D12_RESOURCE_DESC constBuffResourceGeneration(uint32_t size);

public:

	static const uint16_t SDirLightNum_ = 3;

public:

	struct ConstBufferData
	{
		//�����̐F
		XMFLOAT3 ambientColor;
		float pad1;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[SDirLightNum_];

	}; 

private:

	HRESULT result_;

	static Microsoft::WRL::ComPtr<ID3D12Device> dev;


	

	//ComPtr<ID3D12Resource> constBuff;

	XMFLOAT3 ambientColor_ = { 1,1,1 };

	DirectionalLight dirLights_[SDirLightNum_];

	bool dirty_ = false;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuff_ = nullptr;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp_{};
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc_{};

};

