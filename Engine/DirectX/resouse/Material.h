#pragma once
//D3D�R���p�C��
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<d3d12.h>
#pragma comment(lib,"d3d12.lib")

#include <vector>

#include "DirectXInit.h"

#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "matrix4x4.h"
#include "Texture.h"

#include <unordered_map>


class Material
{
public:
	Material();
	~Material();

	void MaterialConstBuffInit();

	void Update();

	void VertexBuffObjSetting(D3D12_HEAP_TYPE heaptype, D3D12_RESOURCE_DIMENSION dimension, uint32_t sizeVB, uint16_t height, uint16_t DepthOrArraySize, int16_t MipLevels, uint16_t SampleDescCount, D3D12_TEXTURE_LAYOUT layput);

	void IndicesBuffSetting(D3D12_RESOURCE_DIMENSION dimension, uint32_t sizeIB, uint16_t height, uint16_t DepthOrArraySize, int16_t MipLevels, uint16_t SampleDescCount, D3D12_TEXTURE_LAYOUT layput);

	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient_;//�A���r�G���g�e���x
		float pad1_;//�p�f�B���O
		XMFLOAT3 diffuse_;//�f�B�t���[�Y�e���x
		float pad2_;//�p�f�B���O
		XMFLOAT3 specular_;//�X�y�L�����[�e���x
		float alpha_;//�A���t�@
	};

	struct MaterialObj
	{
		std::string name_;//�}�e���A����
		XMFLOAT3 ambient_ = {};//�A���r�G���g�e���x
		XMFLOAT3 diffuse_ = {};//�f�B�t���[�Y�e���x
		XMFLOAT3 specular_ = {};//�X�y�L�����[�e���x
		float alpha_ = 1.0f;//�A���t�@
		std::string textureFilename_;//�e�N�X�`���t�@�C����

		//�R���X�g���N�^
		MaterialObj()
		{
			ambient_ = { 0.3f, 0.3f, 0.3f };
			diffuse_ = { 0.0f, 0.0f, 0.0f };
			specular_ = { 0.0f, 0.0f, 0.0f };
			alpha_ = 1.0f;
		}

	};

	//�萔�o�b�t�@�p
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1_ = nullptr;

	ConstBufferDataB1* constMapB1_ = nullptr;

	D3D12_HEAP_PROPERTIES cbHeapProp_{};//�q�[�v�ݒ�

	MaterialObj material_;

	//���_�o�b�t�@�p�ϐ�
	D3D12_HEAP_PROPERTIES heapprop_{};//�q�[�v�ݒ�
	D3D12_RESOURCE_DESC resDesc_{};//���\�[�X�ݒ�

private:

	HRESULT result_;

};

