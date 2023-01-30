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

	static void Staticlnitialize(ID3D12Device* device);

	//�C���X�^���X����
	static LightGroup* Create();

	void Init();

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList,UINT rootParameterIndex);

	//�萔�o�b�t�@�]��
	void TransferConstBuffer();

	void SetAmbientColor(const XMFLOAT3& color);

	void SetDirLightActive(int index, bool active);

	void SetDirLightDir(int index, const XMVECTOR& lightdir);

	void SetDirLightColor(int index, const XMFLOAT3& lightcolor);

	void DefaultLightSetting();

private:

	//�萔�o�b�t�@
	void constantBuffGeneration(ID3D12Device* dev);

	//�萔�o�b�t�@�p�̃��\�[�X�ݒ�֐�
	D3D12_RESOURCE_DESC constBuffResourceGeneration(int size);

public:

	static const int DirLightNum = 3;

public:

	struct ConstBufferData
	{
		//�����̐F
		XMFLOAT3 ambientColor;
		float pad1;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[DirLightNum];

	}; 

private:

	HRESULT result;

	static ID3D12Device* dev;


	

	//ComPtr<ID3D12Resource> constBuff;

	XMFLOAT3 ambientColor = { 1,1,1 };

	DirectionalLight dirLights[DirLightNum];

	bool dirty = false;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuff = nullptr;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};

};

