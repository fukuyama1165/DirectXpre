#pragma once

//D3D�R���p�C��
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<d3d12.h>

#pragma comment(lib,"d3d12.lib")

#include<DirectXMath.h>
#include <wrl.h>

class DirectionalLight
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
		uint32_t active;

	};

private://�ÓI�����o�ϐ�

	//�f�o�C�X
	//static Microsoft::WRL::ComPtr<ID3D12Device> dev;

public://�ÓI�����o�֐�

	static void StaticInitialize();

public:

	//�C���X�^���X����
	//static DirectionalLight* Create();

	//������
	void Init();

	//�X�V
	void Update();

	//�`��
	void Draw(uint32_t rootParameterIndex);

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

	//�L���t���O���Z�b�g
	inline void SetIsActive(bool flag) { isActive_ = flag; };

	//�L���t���O���擾
	inline bool GetIsActive() { return isActive_; };

	XMVECTOR GetLightDir() { return lightDir_; };

	XMFLOAT3 GetLightColor() { return lightColor_; };

private://�����o�֐�(privete)

	//�萔�o�b�t�@
	void constantBuffGeneration();

	//�萔�o�b�t�@�p�̃��\�[�X�ݒ�֐�
	D3D12_RESOURCE_DESC constBuffResourceGeneration(uint32_t size);


private://�����o�ϐ�

	HRESULT result_;

	//�L���t���O
	bool isActive_ = false;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuff_ = nullptr;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp_{};
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc_{};

	//���C�g��������(�P�ʃx�N�g��)
	XMVECTOR lightDir_ = { 1,0,0,0 };

	//���C�g�F
	XMFLOAT3 lightColor_ = { 1,1,1 };

	//�_�[�e�B�t���O
	bool dirty_ = false;

};

