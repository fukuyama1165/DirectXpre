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

#include "matrix4x4.h"
#include "Float3.h"
#include "Object3D.h"
#include "Texture.h"
#include "Float4.h"

//ComPtr�p�C���N���[�h
#include <wrl.h>

class DrawingObj
{
public:
	//�������������œK���ɃG���[���͂��Ȃ��Ǝv���l�����Ă����{�I�ɏ��������Ă����Ȃ��͂�(�ݒ肪�j�]���ĂȂ����)
	DrawingObj(const float windowWidth, const float windowHeight);

	//���W���w��ł��邪�����吙
	DrawingObj(const float windowWidth, const float windowHeight,XMFLOAT3 vertexPos1, XMFLOAT3 vertexPos2, XMFLOAT3 vertexPos3, XMFLOAT3 vertexPos4, XMFLOAT2 vertexUv1 = { 0.0f,1.0f }, XMFLOAT2 vertexUv2 = { 0.0f,0.0f }, XMFLOAT2 vertexUv3 = { 1.0f,1.0f }, XMFLOAT2 vertexUv4 = { 1.0f,0.0f });

	~DrawingObj();

	//�`�揉���������֐�
	void basicInit(ID3D12Device* dev);//basicPS�ǂݍ���
	void colorChangeInit(ID3D12Device* dev);//colorChangePS�ǂݍ���

	//���_�f�[�^�\����
	struct Vertex
	{
		XMFLOAT3 pos;//xyz���W
		XMFLOAT3 normal;//�@���x�N�g��
		XMFLOAT2 uv;//uv���W
	};

	//���_�o�b�t�@����
	void vertexBuffGeneration(ID3D12Device* dev);

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
	void constantBuffGeneration1(ID3D12Device* dev);

	//�C���f�b�N�X�f�[�^�֘A(�C���f�b�N�X�r���[������)
	void indicesBuffGeneration(ID3D12Device* devconst);

	//�摜�C���[�W�f�[�^
	void imageDataGeneration();

	//�e�N�X�`���o�b�t�@
	void textureBuffGeneraion(ID3D12Device* dev);

	//�V�F�[�_���\�[�X�r���[
	void SRVGeneraion(ID3D12Device* dev);

	//GPU�ɒ��_�f�[�^��]������֐�
	void vertexMap();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	/// <param name="PipeLineRuleFlag">�`����@��ύX���邩�ǂ���(���݂̓��C���[�t���[�����h��Ԃ�)true�͓h��Ԃ�</param>
	/// <param name="ChangeSquareFlag">�O�p�`�ŕ`�悷�邩�l�p�`�ɕ`�悷��(true�͎l�p�`)</param>
	void Draw(ID3D12GraphicsCommandList* cmdList,bool PipeLineRuleFlag, bool ChangeSquareFlag, bool ChangeTexure);


	//�A�t�B���ϊ����̂��̂̊֐�(2D)
	XMFLOAT3 Afin(XMFLOAT3 box, float moveX, float moveY, float rotate, float scaleX, float scaleY);

	//Obj�ɑ΂��ăA�t�B���ϊ���������֐�
	void ObjAfin(float moveX, float moveY, float rotate, float scaleX, float scaleY);

	//�萔�o�b�t�@��ύX����֐�
	void constBuffColorUpdata(float Red,float Green,float Blue);
	void constBuffPosMUpdata(float X,float Y,float Z);

	//�r���[�ϊ��s��X�V
	//void matViewUpdata(Float3 eye, Float3 target, Float3 up);
	void matViewUpdata(Float3 eye, Float3 target, Float3 up);

	//�萔�o�b�t�@�̍s����X�V����֐�
	void constTransformMatUpdata();
	void constTransformMatUpdata1();

	//���[���h���W�X�V
	void matWorldUpdata();
	void matWorldUpdata1();

	Matrix4x4 matScaleGeneration(Float3 scale);

	Matrix4x4 matRotateXGeneration(float rotateX);
	Matrix4x4 matRotateYGeneration(float rotateY);
	Matrix4x4 matRotateZGeneration(float rotateZ);

	Matrix4x4 matRotateGeneration(Float3 rotate);

	Matrix4x4 matMoveGeneration(Float3 translation);

	Matrix4x4 matViewGeneration(Float3 eye, Float3 target, Float3 up);

	Matrix4x4 perspectiveProjectionGeneration(float FovAngleY, float NearZ, float FarZ);

	//�X�P�[���ύX�s��
	void SetScale(Float3 scale);
	//�p�x�ύX�s��
	void SetRotate(Float3 rotate);
	//���s�ړ��s��
	void SetTrans(Float3 TransForm);

	void obj3DUpdate();

	/// <summary>
	/// sin��cos��Ԃ��֐�
	/// </summary>
	/// <param name="Sin">sin�̒l���������</param>
	/// <param name="Cos">cos�̒l���������</param>
	/// <param name="angle">���̒l</param>
	void sinCos(float& Sin, float& Cos, float angle);

	Float3 float3Dat(Float3 A, Float3 B);

	void colorMap(float R,float G,float B);

	void deleteTexture();

private:

	//��ʃT�C�Y
	float Win_width;
	float Win_height;

	//���_�f�[�^(���₵�����Ȃ炱�������₵�Ă���)
	Vertex vertices[24];

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

	//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
	struct ConstBufferDataTransform
	{
		XMMATRIX mat;//3D�ϊ��s��
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
	unsigned short indices[36];
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

	Matrix4x4 matWorld;

	Matrix4x4 matScale;
	Matrix4x4 matRotate;
	Matrix4x4 matTrans;

	Float3 Scale_;
	Float3 Rotate_;
	Float3 Trans_;

	////�������e�s��
	//XMMATRIX matProjection;

	////�r���[�ϊ��s��
	//XMMATRIX matView;
	//XMFLOAT3 eye_;//���_���W
	//XMFLOAT3 target_;//�����_���W
	//XMFLOAT3 up_;//������x�N�g��

	//XMMATRIX matWorld;
	//XMMATRIX matWorld1;

	//XMMATRIX matScale;
	//XMMATRIX matRotate;
	//XMMATRIX matTrans;

	//XMFLOAT3 Scale_;
	//XMFLOAT3 Rotate_;
	//XMFLOAT3 Trans_;

	//3D�I�u�W�F�N�g�̐�
	const static size_t kObjectConst = 50;
	
	

	//3D�I�u�W�F�N�g�̔z��
	Object3D object3Ds[kObjectConst];

	static Texture* texture;

	

};
