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

//const float PI = 3.141592653589;

#include <DirectXTex.h>

class DrawingObj
{
public:
	//�������������œK���ɃG���[���͂��Ȃ��Ǝv���l�����Ă����{�I�ɏ��������Ă����Ȃ��͂�(�ݒ肪�j�]���ĂȂ����)
	DrawingObj();

	//���W���w��ł��邪�����吙
	DrawingObj(XMFLOAT3 vertexPos1, XMFLOAT2 vertexUv1, XMFLOAT3 vertexPos2, XMFLOAT2 vertexUv2, XMFLOAT3 vertexPos3, XMFLOAT2 vertexUv3, XMFLOAT3 vertexPos4, XMFLOAT2 vertexUv4 );

	~DrawingObj();

	//�`�揉���������֐�
	void basicInit(ID3D12Device* dev);//basicPS�ǂݍ���
	void colorChangeInit(ID3D12Device* dev);//colorChangePS�ǂݍ���

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
	void pixelShaderGeneration();//basicPS�ǂݍ���
	void pixelShaderGeneration2();//colorChangePS�ǂݍ���

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
	
	//�e�N�X�`���T���v���[�̐ݒ�
	void textureSamplerGeneration();
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};

	//���[�g�V�O�l�`��
	void rootsignatureGeneration(ID3D12Device* dev);

	//���[�g�V�O�l�`��
	ID3D12RootSignature* rootsignature;

	//�p�C�v���C���X�e�[�g
	ID3D12PipelineState* pipelinestate = nullptr;
	ID3D12PipelineState* pipelinestate2 = nullptr;

	//�萔�o�b�t�@
	void constantBuffGeneration(ID3D12Device* dev);

	//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;//�F(RGBA)
	};

	struct ConstBufferDataMaterial2
	{
		XMFLOAT4 posM;//�ʒu�ړ��Ɏg��(XYZ);
	};

	//�萔�o�b�t�@���̂���
	ID3D12Resource* constBuffMaterial = nullptr;
	ID3D12Resource* constBuffMaterial2 = nullptr;

	//�}�b�s���O����Ƃ��̃|�C���^
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	ConstBufferDataMaterial2* constMapMaterial2 = nullptr;

	//�C���f�b�N�X�f�[�^�֘A(�C���f�b�N�X�r���[������)
	void indicesBuffGeneration(ID3D12Device* dev);
	//�C���f�b�N�X�f�[�^
	unsigned short indices[6];
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB;
	//�C���f�b�N�X�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};

	//�摜�C���[�W�f�[�^
	void imageDataGeneration();
	
	//�摜�f�[�^��
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	//�e�N�X�`���o�b�t�@
	void textureBuffGeneraion(ID3D12Device* dev);

	ID3D12Resource* texBuff = nullptr;

	//�V�F�[�_���\�[�X�r���[
	void SRVGeneraion(ID3D12Device* dev);

	//GPU�ɒ��_�f�[�^��]������֐�
	ID3D12DescriptorHeap* srvHeap = nullptr;

	void vertexMap();

private:

};

