#pragma once
//D3Dコンパイラ
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<vector>
#include<string>

#include<d3d12.h>
#include<dxgi1_6.h>

//数学ライブラリ(最適化されすぎて使いにくいらしいのであとで自作しろって)
#include<DirectXMath.h>
using namespace DirectX;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "matrix4x4.h"
#include "Float3.h"

//ComPtr用インクルード
#include <wrl.h>

class Object3D
{
public:
	Object3D();
	~Object3D();

	void Init(ID3D12Device* dev);

	void Update(Matrix4x4 matView, Matrix4x4 matprojection, XMMATRIX a , XMMATRIX b);

	void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_VERTEX_BUFFER_VIEW& vbView, D3D12_INDEX_BUFFER_VIEW& idView, UINT numIndices, bool ChangeSquareFlag);

	void constTransformMatUpdata(Matrix4x4 matView, Matrix4x4 matProjection, XMMATRIX a, XMMATRIX b);

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

private:

	//定数バッファ用データ構造体(3D変換行列)
	struct ConstBufferDataTransform
	{
		Matrix4x4 mat;//3D変換行列
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

};


