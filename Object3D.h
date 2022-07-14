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

#include "matrix4x4.h"
#include "Float3.h"

//ComPtr�p�C���N���[�h
#include <wrl.h>

class Object3D
{
public:
	Object3D();
	~Object3D();

	void Init(ID3D12Device* dev);

	void Update(XMMATRIX& matView, XMMATRIX& matprojection);

	void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_VERTEX_BUFFER_VIEW& vbView, D3D12_INDEX_BUFFER_VIEW& idView, UINT numIndices, bool ChangeSquareFlag);

	void constTransformMatUpdata(XMMATRIX& matView, XMMATRIX& matProjection);

	void SetScale(XMFLOAT3 scale);
	void SetRotate(XMFLOAT3 rotate);
	void SetPos(XMFLOAT3 pos);
	void SetParent(Object3D* parent);


	XMFLOAT3 GetScale();
	XMFLOAT3 GetRotate();
	XMFLOAT3 GetPos();

	XMMATRIX GetWorldMat();

	Object3D* GetParent();

private:

	//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
	struct ConstBufferDataTransform
	{
		XMMATRIX mat;//3D�ϊ��s��
	};

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;

	ConstBufferDataTransform* constMapTransform = nullptr;

	XMFLOAT3 Scale_;
	XMFLOAT3 Rotate_;
	XMFLOAT3 Trans_;

	XMMATRIX matScale;
	XMMATRIX matRotate;
	XMMATRIX matTrans;

	XMMATRIX matWorld;

	Object3D* parent_ = nullptr;

};


