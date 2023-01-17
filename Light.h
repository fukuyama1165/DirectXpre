#pragma once

//D3D�R���p�C��
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<d3d12.h>

#pragma comment(lib,"d3d12.lib")

#include<DirectXMath.h>
#include <wrl.h>

class Light
{
private://�G�C���A�X

	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{

		XMVECTOR lightv;
		XMFLOAT3 lightColor;

	};

private://�ÓI�����o�ϐ�

	//�f�o�C�X
	static ID3D12Device* dev;

public://�ÓI�����o�֐�

	static void StaticInitialize(ID3D12Device* device);

public:

	//�C���X�^���X����
	static Light* Create();

	//������
	void Init();

	//�X�V
	void Update();

	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList,UINT rootParameterIndex);

	//�萔�o�b�t�@�]��
	void TransferConstBuffer();

	/// <summary>
	/// ���C�g�������Z�b�g
	/// </summary>
	/// <param name="lightdir">���C�g����</param>
	void SetLightDir(const XMVECTOR& lightdir);

	/// <summary>
	/// ���C�g�F���Z�b�g
	/// </summary>
	/// <param name="lightcolor">���C�g�F</param>
	void SetLightColor(const XMFLOAT3& lightcolor);

private://�����o�֐�(privete)

	//�萔�o�b�t�@
	void constantBuffGeneration(ID3D12Device* dev);

	//�萔�o�b�t�@�p�̃��\�[�X�ݒ�֐�
	D3D12_RESOURCE_DESC constBuffResourceGeneration(int size);


private://�����o�ϐ�

	HRESULT result;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuff = nullptr;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};

	//���C�g��������(�P�ʃx�N�g��)
	XMVECTOR lightDir = { 1,0,0,0 };

	//���C�g�F
	XMFLOAT3 lightColor = { 1,1,1 };

	//�_�[�e�B�t���O
	bool dirty = false;

};

