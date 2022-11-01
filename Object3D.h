#pragma once
//D3D�R���p�C��
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

#include <DirectXTex.h>

#include "Texture.h"
#include "Float4.h"


class Object3D
{
public:
	Object3D();
	~Object3D();

	void Init(ID3D12Device* dev);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList, bool PipeLineRuleFlag, bool ChangeSquareFlag, bool ChangeTexure);

	void constTransformMatUpdata(Matrix4x4 matView, Matrix4x4 matProjection);

	void SetScale(Float3 scale);
	void SetRotate(Float3 rotate);
	void SetPos(Float3 pos);
	void SetParent(Object3D* parent);


	Matrix4x4 matScaleGeneration(Float3 scale);

	Matrix4x4 matRotateXGeneration(float rotateX);
	Matrix4x4 matRotateYGeneration(float rotateY);
	Matrix4x4 matRotateZGeneration(float rotateZ);

	Matrix4x4 matRotateGeneration(Float3 rotate);

	Matrix4x4 matMoveGeneration(Float3 translation);

	Float3 GetScale();
	Float3 GetRotate();
	Float3 GetPos();

	Matrix4x4 GetWorldMat();

	Object3D* GetParent();

	//�`�揉���������֐�
	void basicInit(ID3D12Device* dev, float WinWidth, float WinHeight);//basicPS�ǂݍ���
	void colorChangeInit(ID3D12Device* dev, float WinWidth, float WinHeight);//colorChangePS�ǂݍ���
	void objDrawInit(ID3D12Device* dev, const char filename[]);//.obj��ǂݍ���ŕ`��

	//�����l�p��O�p�`�ׂ̈̒��_�f�[�^�ݒ�֐�

	void basicVertexInit();
	void colorChangeVertexInit();

	//���_�f�[�^�\����
	struct Vertex
	{
		XMFLOAT3 pos;//xyz���W
		XMFLOAT3 normal;//�@���x�N�g��
		XMFLOAT2 uv;//uv���W
	};

	//���_�o�b�t�@����
	void vertexBuffGeneration(ID3D12Device* dev);
	void objVertexBuffGeneration(ID3D12Device* dev, const char filename[]);

	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void vertexShaderGeneration();//basicVS�ǂݍ���
	void vertexShaderGeneration2();//vertexMoveVS�ǂݍ���

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void pixelShaderGeneration();//basicPS�ǂݍ���
	void pixelShaderGeneration2();//colorChangePS�ǂݍ���

	//���_���C�A�E�g�̐ݒ�
	void vertexLayout();

	//�O���t�B�b�N�X�p�C�v���C���̐ݒ�
	void graphicPipelineGeneration();

	//�f�X�N���v�^�����W�̐ݒ�
	void descriptorRangeGeneration();

	//���[�g�p�����[�^�̐ݒ�(�萔�o�b�t�@�ƃV�F�[�_�ɂ���)
	void rootParamGeneration();

	//�e�N�X�`���T���v���[�̐ݒ�
	void textureSamplerGeneration();

	//���[�g�V�O�l�`��
	void rootsignatureGeneration(ID3D12Device* dev);

	//�萔�o�b�t�@
	void constantBuffGeneration(ID3D12Device* dev);

	//�C���f�b�N�X�f�[�^�֘A(�C���f�b�N�X�r���[������)
	void indicesBuffGeneration(ID3D12Device* devconst);

	Matrix4x4 matViewGeneration(Float3 eye, Float3 target, Float3 up);

	Matrix4x4 perspectiveProjectionGeneration(float FovAngleY, float NearZ, float FarZ);

	/// <summary>
	/// sin��cos��Ԃ��֐�
	/// </summary>
	/// <param name="Sin">sin�̒l���������</param>
	/// <param name="Cos">cos�̒l���������</param>
	/// <param name="angle">���̒l</param>
	void sinCos(float& Sin, float& Cos, float angle);

	Float3 float3Dat(Float3 A, Float3 B);

	void deleteTexture();

	void matViewUpdata(Float3 eye, Float3 target, Float3 up);

private:

	//��ʃT�C�Y
	float Win_width;
	float Win_height;

	//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
	struct ConstBufferDataTransform
	{
		Matrix4x4 mat;//3D�ϊ��s��
	};

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;

	ConstBufferDataTransform* constMapTransform = nullptr;

	Float3 Scale_;
	Float3 Rotate_;
	Float3 Trans_;

	Matrix4x4 matScale;
	Matrix4x4 matRotate;
	Matrix4x4 matTrans;

	Matrix4x4 matWorld;

	Object3D* parent_ = nullptr;

	//���_�f�[�^(���₵�����Ȃ炱�������₵�Ă���)
	std::vector <Vertex> vertices;

	//���_�f�[�^�T�C�Y
	UINT sizeVB;

	HRESULT result;

	//���_�o�b�t�@�p�ϐ�
	D3D12_HEAP_PROPERTIES heapprop{};//�q�[�v�ݒ�
	D3D12_RESOURCE_DESC resDesc{};//���\�[�X�ݒ�
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
	Vertex* vertMap = nullptr;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//���_�V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;

	//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;

	//�G���[�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	//���_���C�A�E�g(�v�f�𑝂₷�Ȃ�z�񐔂𑝂₷)
	D3D12_INPUT_ELEMENT_DESC inputLayout[3];

	//�O���t�B�b�N�X�p�C�v���C���̊e�X�e�[�W�̐ݒ������\���̂�p��
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline2{};

	//�f�X�N���v�^�����W
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	//���[�g�p�����[�^(�萔�o�b�t�@�̐�����������z��̗v�f���𑝂₵�Đݒ�����Ă���֐��̒��g�ɂ��ǉ����邱��)
	D3D12_ROOT_PARAMETER rootParam[4] = {};

	//�e�N�X�`���T���v���[
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};

	//���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;

	//�p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate2 = nullptr;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};

	//�萔�o�b�t�@�p�̃��\�[�X�ݒ�֐�
	D3D12_RESOURCE_DESC constBuffResourceGeneration(int size);

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
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial2 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform0 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform1 = nullptr;

	//�}�b�s���O����Ƃ��̃|�C���^
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	ConstBufferDataMaterial2* constMapMaterial2 = nullptr;
	ConstBufferDataTransform* constMapTransform0 = nullptr;
	ConstBufferDataTransform* constMapTransform1 = nullptr;

	//�C���f�b�N�X�f�[�^
	std::vector< unsigned short> indices;
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB;
	//�C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;
	//�C���f�b�N�X�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};

	//�摜�f�[�^��
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;

	//ID3D12DescriptorHeap* srvHeap = nullptr;

	//�������e�s��
	Matrix4x4 matProjection;

	//�r���[�ϊ��s��
	Matrix4x4 matView;
	Float3 eye_;//���_���W
	Float3 target_;//�����_���W
	Float3 up_;//������x�N�g��

	static Texture* texture;

};


