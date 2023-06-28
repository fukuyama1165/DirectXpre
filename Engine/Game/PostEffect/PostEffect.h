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

	//頂点シェーダの読み込みとコンパイル
	void vertexShaderGeneration();

	//ピクセルシェーダの読み込みとコンパイル
	void pixelShaderGeneration();

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

	//テクスチャバッファ生成
	void textureBuffGeneraion();

	//SRV用デスクリプタヒープ生成
	void SRVDescHeapGeneraion();

	//RTV用デスクリプタヒープ生成
	void RTVDescHeapGeneraion();

	//DSV用デスクリプタヒープ生成
	void DSVDescHeapGeneraion();

	//定数バッファ用のリソース設定関数
	D3D12_RESOURCE_DESC constBuffResourceGeneration(uint32_t size);

private:

	//頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos;//xyz座標
		XMFLOAT2 uv;
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

	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>texBuff_;

	//シェーダーリソースビュー
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV_;

	//深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff_;

	//RTV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV_;

	//DSV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV_;

	static const float clearColor[4];

	//リソースバリア
	D3D12_RESOURCE_BARRIER barrierDesc_{};

	static Texture* STexture_;

	//頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob_ = nullptr;

	//ピクセルシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob_ = nullptr;

	//テクスチャサンプラー
	D3D12_STATIC_SAMPLER_DESC sampleDesc_{};

	//デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange_{};

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

		XMFLOAT4 color;

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

	

	//平行投影
	Matrix4x4 matProjection_;

};

