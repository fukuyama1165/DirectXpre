#pragma once
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include<d3d12.h>
#include<dxgi1_6.h>

//���w���C�u����(�œK�����ꂷ���Ďg���ɂ����炵���̂ł��ƂŎ��삵�����)
#include<DirectXMath.h>
using namespace DirectX;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "matrix4x4.h"
#include "Float3.h"

//ComPtr�p�C���N���[�h
#include <wrl.h>

class SpriteCommon
{
public:
	void initialize(ID3D12Device* dev);

	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void vertexShaderGeneration();

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void pixelShaderGeneration();

	//���_���C�A�E�g�̐ݒ�
	void vertexLayout();

	//�O���t�B�b�N�X�p�C�v���C���̐ݒ�
	void graphicPipelineGeneration();

	//���[�g�V�O�l�`��
	void rootsignatureGeneration(ID3D12Device* dev);

	
	Microsoft::WRL::ComPtr<ID3D12RootSignature> getRootsignature() { return rootsignature; };

	//�p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> getPipelinestate() { return pipelinestate; };
	Microsoft::WRL::ComPtr<ID3D12PipelineState> getPipelinestate2() { return pipelinestate2; };
	Microsoft::WRL::ComPtr<ID3D12PipelineState> getPipelinestate3() { return pipelinestate3; };
	Microsoft::WRL::ComPtr<ID3D12PipelineState> getPipelinestate4() { return pipelinestate4; };
	Microsoft::WRL::ComPtr<ID3D12PipelineState> getPipelinestate5() { return pipelinestate5; };
	

private:



	HRESULT result;

	


	//���_�V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;

	//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;

	//�G���[�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	//���_���C�A�E�g(�v�f�𑝂₷�Ȃ�z�񐔂𑝂₷)
	D3D12_INPUT_ELEMENT_DESC inputLayout[1];

	//�O���t�B�b�N�X�p�C�v���C���̊e�X�e�[�W�̐ݒ������\���̂�p��
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline2{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline3{};//���Z
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline4{};//���Z
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline5{};//�F���]

	//���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;

	//�p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate2 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate3 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate4 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate5 = nullptr;

	struct  ConstBuffermaterial
	{
		
		XMFLOAT4 color;

	};

};

