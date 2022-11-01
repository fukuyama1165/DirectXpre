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

	

	//�X�P�[���ύX�s��
	void SetScale(Float3 scale);
	//�p�x�ύX�s��
	void SetRotate(Float3 rotate);
	//���s�ړ��s��
	void SetTrans(Float3 TransForm);

	void obj3DUpdate();

	

	void colorMap(float R,float G,float B);

	

private:

	

	

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
	//Object3D object3Ds[kObjectConst];

	

	

};
