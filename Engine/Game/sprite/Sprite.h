#pragma once
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include<d3d12.h>
#include<dxgi1_6.h>

//数学ライブラリ(最適化されすぎて使いにくいらしいのであとで自作しろって)
#include<DirectXMath.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "matrix4x4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vector2.h"

//ComPtr用インクルード
#include <wrl.h>

#include "SpriteCommon.h"
#include "Texture.h"
#include "WinApp.h"

class Sprite
{
public:
	Sprite();

	void initialize(SpriteCommon* spriteCommon, uint32_t textureIndex = UINT32_MAX);

	void Update();

	void Draw(uint32_t ChangeTexure = 0, uint16_t PipeLineRuleFlag=0);

	void setColor(const Vector4& color);

	void SetTextureLeftTop(const DirectX::XMFLOAT2& lefttop) { textureLeftTop_ = lefttop; };

	void SetTextureSize(const DirectX::XMFLOAT2& size) { textureSize_ = size; isReSize_ = true; };

	DirectX::XMFLOAT2 GetTextureLeftTop() { return textureLeftTop_; };

	DirectX::XMFLOAT2 GetTextureSize() { return textureSize_; };

protected:

	void vertexBuffGeneration();

	//頂点レイアウトの設定
	void vertexLayout();

	//グラフィックスパイプラインの設定
	void graphicPipelineGeneration();

	//ルートパラメータの設定(定数バッファとシェーダについて)
	void rootParamGeneration();

	//ルートシグネチャ
	void rootsignatureGeneration();

	//定数バッファ
	void constantBuffGeneration();

	void descriptorRangeGeneration();

	//テクスチャサンプラーの設定
	void textureSamplerGeneration();
	
	
	void vertexMap();

	//サイズをテクスチャに合わせる
	void AdjustTexSize(uint32_t textureIndex);

	Matrix4x4 matScaleGeneration(const Vector3& scale);

	Matrix4x4 matRotateXGeneration(float rotateX);
	Matrix4x4 matRotateYGeneration(float rotateY);
	Matrix4x4 matRotateZGeneration(float rotateZ);

	Matrix4x4 matRotateGeneration(const Vector3& rotate);

	Matrix4x4 matMoveGeneration(const Vector3& translation);

	void matWorldGeneration();

public:

	Vector2 pos_ = {};
	float rotate_ = 0;
	Vector2 scale_ = {1,1};

	

protected:

	SpriteCommon* common_;

	//頂点データ構造体
	struct Vertex
	{
		Vector3 pos;//xyz座標
		Vector2 uv;
	};


	Vertex vertices_[6] = {};

	uint32_t sizeVB_;

	HRESULT result_;

	//頂点レイアウト(要素を増やすなら配列数を増やす)
	D3D12_INPUT_ELEMENT_DESC inputLayout_[2];

	//エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_ = nullptr;

	//頂点バッファ用変数
	D3D12_HEAP_PROPERTIES heapprop_{};//ヒープ設定
	D3D12_RESOURCE_DESC resDesc_{};//リソース設定
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	Vertex* vertMap_;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};


	//デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange_{};

	//テクスチャサンプラー
	D3D12_STATIC_SAMPLER_DESC sampleDesc_{};


	//グラフィックスパイプラインの各ステージの設定をする構造体を用意
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline_{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline2_{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline3_{};//加算
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline4_{};//減算
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline5_{};//色反転

	//ルートパラメータ(定数バッファの数が増えたら配列の要素数を増やして設定をしている関数の中身にも追加すること)
	D3D12_ROOT_PARAMETER rootParam_[3] = {};

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature_;

	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate2_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate3_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate4_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate5_ = nullptr;

	struct  ConstBufferMaterial
	{

		Vector4 color;

	};

	//定数バッファそのもの

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	//マッピングするときのポインタ
	ConstBufferMaterial* constMapMaterial_ = nullptr;

	struct ConstBuffDataTransform
	{
		Matrix4x4 mat;
	};

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;
	//マッピングするときのポインタ
	ConstBuffDataTransform* constMapTransform_ = nullptr;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp_{};
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc_{};

	//定数バッファ用のリソース設定関数
	D3D12_RESOURCE_DESC constBuffResourceGeneration(uint32_t size);

	static Texture* STexture_;


	Vector2 oldPos_;

	float setScaleX_ = 1;
	float setScaleY_ = 1;

	DirectX::XMFLOAT2 textureLeftTop_ = { 0.0f,0.0f };

	DirectX::XMFLOAT2 textureSize_ = { 100.0f,100.0f };

	bool isReSize_ = false;

	uint32_t texIndex = 1;

	//平行投影
	Matrix4x4 matProjection_;

	Matrix4x4 matScale_;
	Matrix4x4 matRotate_;
	Matrix4x4 matTrans_;

	Matrix4x4 matWorld_;

};