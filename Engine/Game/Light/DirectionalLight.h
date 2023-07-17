#pragma once

//D3D�R���p�C��
#include<d3dcompiler.h>

#include<d3d12.h>

#include<DirectXMath.h>
#include <wrl.h>

#include "Vector3.h"
#include "Vector4.h"

class DirectionalLight
{
private://�G�C���A�X

	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	

public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
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
	void SetLightDir(const Vector4& lightdir);

	/// <summary>
	/// ���C�g�F���Z�b�g
	/// </summary>
	/// <param name="lightcolor">���C�g�F</param>
	void SetLightColor(const Vector3& lightcolor);

	//�L���t���O���Z�b�g
	void SetIsActive(bool flag) { isActive_ = flag; };

	//�L���t���O���擾
	bool GetIsActive() { return isActive_; };

	Vector4 GetLightDir() { return lightDir_; };

	Vector3 GetLightColor() { return lightColor_; };

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
	Vector4 lightDir_ = { 1,0,0,0 };

	//���C�g�F
	Vector3 lightColor_ = { 1,1,1 };

	//�_�[�e�B�t���O
	bool dirty_ = false;

};

