#pragma once
#include "DirectXInit.h"
#include "WinApp.h"
#include "Texture.h"
#include "matrix4x4.h"

class PostEffect
{

public:
	PostEffect();

	void Initialize();

	void Update();

	void Draw(uint16_t PipeLineRuleFlag = 0);

	void PreDrawScene();

	void PostDrawScene();

private:

	void vertexBuffGeneration();

	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void vertexShaderGeneration();

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void pixelShaderGeneration();

	//���_���C�A�E�g�̐ݒ�
	void vertexLayout();

	//�O���t�B�b�N�X�p�C�v���C���̐ݒ�
	void graphicPipelineGeneration();

	//���[�g�p�����[�^�̐ݒ�(�萔�o�b�t�@�ƃV�F�[�_�ɂ���)
	void rootParamGeneration();

	//���[�g�V�O�l�`��
	void rootsignatureGeneration();

	//�萔�o�b�t�@
	void constantBuffGeneration();

	void descriptorRangeGeneration();

	//�e�N�X�`���T���v���[�̐ݒ�
	void textureSamplerGeneration();

	//�e�N�X�`���o�b�t�@����
	void textureBuffGeneraion();

	//SRV�p�f�X�N���v�^�q�[�v����
	void SRVDescHeapGeneraion();

	//RTV�p�f�X�N���v�^�q�[�v����
	void RTVDescHeapGeneraion();

	//DSV�p�f�X�N���v�^�q�[�v����
	void DSVDescHeapGeneraion();

	//�萔�o�b�t�@�p�̃��\�[�X�ݒ�֐�
	D3D12_RESOURCE_DESC constBuffResourceGeneration(uint32_t size);

private:

	//���_�f�[�^�\����
	struct Vertex
	{
		XMFLOAT3 pos;//xyz���W
		XMFLOAT2 uv;
	};


	Vertex vertices_[6] = {};

	uint32_t sizeVB_;

	HRESULT result_;

	//���_���C�A�E�g(�v�f�𑝂₷�Ȃ�z�񐔂𑝂₷)
	D3D12_INPUT_ELEMENT_DESC inputLayout_[2];

	//�G���[�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_ = nullptr;

	//���_�o�b�t�@�p�ϐ�
	D3D12_HEAP_PROPERTIES heapprop_{};//�q�[�v�ݒ�
	D3D12_RESOURCE_DESC resDesc_{};//���\�[�X�ݒ�
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	Vertex* vertMap_;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//�e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource>texBuff_;

	//�V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV_;

	//�[�x�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff_;

	//RTV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV_;

	//DSV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV_;

	static const float clearColor[4];

	//���\�[�X�o���A
	D3D12_RESOURCE_BARRIER barrierDesc_{};

	static Texture* STexture_;

	//���_�V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob_ = nullptr;

	//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob_ = nullptr;

	//�e�N�X�`���T���v���[
	D3D12_STATIC_SAMPLER_DESC sampleDesc_{};

	//�f�X�N���v�^�����W
	D3D12_DESCRIPTOR_RANGE descriptorRange_{};

	//�O���t�B�b�N�X�p�C�v���C���̊e�X�e�[�W�̐ݒ������\���̂�p��
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline_{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline2_{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline3_{};//���Z
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline4_{};//���Z
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline5_{};//�F���]

	//���[�g�p�����[�^(�萔�o�b�t�@�̐�����������z��̗v�f���𑝂₵�Đݒ�����Ă���֐��̒��g�ɂ��ǉ����邱��)
	D3D12_ROOT_PARAMETER rootParam_[3] = {};

	//���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature_;

	//�p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate2_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate3_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate4_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate5_ = nullptr;

	struct  ConstBufferMaterial
	{

		XMFLOAT4 color;

	};

	//�萔�o�b�t�@���̂���

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	//�}�b�s���O����Ƃ��̃|�C���^
	ConstBufferMaterial* constMapMaterial_ = nullptr;

	struct ConstBuffDataTransform
	{
		Matrix4x4 mat;
	};

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;
	//�}�b�s���O����Ƃ��̃|�C���^
	ConstBuffDataTransform* constMapTransform_ = nullptr;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp_{};
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc_{};

	

	//���s���e
	Matrix4x4 matProjection_;

};

