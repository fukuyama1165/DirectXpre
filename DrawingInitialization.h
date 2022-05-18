#pragma once
//D3D�R���p�C��
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<vector>
#include<string>

#include<d3d12.h>
#include<dxgi1_6.h>

//���w���C�u����(�œK�����ꂷ���Ďg���ɂ����炵���̂ł��ƂŎ��삵�����)
#include<DirectXMath.h>
using namespace DirectX;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

//�L�[�{�[�h��R���g���[���[�Ȃǂ̓��͂���w�b�_�ƃ��C�u�����̃����N
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")

#pragma comment(lib,"dxguid.lib")

const float PI = 3.141592653589;

#include <DirectXTex.h>

class DrawingInit
{
public:
	DrawingInit();
	~DrawingInit();

	//���_�f�[�^�\����
	struct Vertex
	{
		XMFLOAT3 pos;//xyz���W
		XMFLOAT2 uv;//uv���W
	};

	//���_�f�[�^(���₵�����Ȃ炱�������₵�Ă���)
	Vertex vertices[4];

	//���_�f�[�^�T�C�Y
	UINT sizeVB;

	HRESULT result;

	//���_�o�b�t�@����
	void vertexBuffGeneration(ID3D12Device* dev);
	//���_�o�b�t�@�p�ϐ�
	D3D12_HEAP_PROPERTIES heapprop{};//�q�[�v�ݒ�
	D3D12_RESOURCE_DESC resDesc{};//���\�[�X�ݒ�
	ID3D12Resource* vertBuff = nullptr;
	Vertex* vertMap = nullptr;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void vertexShaderGeneration();
	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void pixelShaderGeneration();
	ID3DBlob* vsBlob = nullptr;//���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr;//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr;//�G���[�I�u�W�F�N�g

	//���_���C�A�E�g�̐ݒ�
	void vertexLayout();
	//���_���C�A�E�g(�v�f�𑝂₷�Ȃ�z�񐔂𑝂₷)
	D3D12_INPUT_ELEMENT_DESC inputLayout[2];

	//�O���t�B�b�N�X�p�C�v���C���̐ݒ�
	void graphicPipelineGeneration();
	//�O���t�B�b�N�X�p�C�v���C���̊e�X�e�[�W�̐ݒ������\���̂�p��
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline2{};

	//�f�X�N���v�^�����W�̐ݒ�
	void descriptorRangeGeneration();
	//�f�X�N���v�^�����W
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	//���[�g�p�����[�^�̐ݒ�(�萔�o�b�t�@�ƃV�F�[�_�ɂ���)
	void rootParamGeneration();
	//���[�g�p�����[�^(�萔�o�b�t�@�̐�����������z��̗v�f���𑝂₵�Đݒ�����Ă���֐��̒��g�ɂ��ǉ����邱��)
	D3D12_ROOT_PARAMETER rootParam[3] = {};
	
	void textureSamplerGeneration();
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};

	void rootsignatureGeneration(ID3D12Device* dev);

	

private:

};

