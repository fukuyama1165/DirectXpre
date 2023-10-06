#pragma once
#include "DirectXInit.h"
#include "WinApp.h"
#include "Texture.h"
#include "matrix4x4.h"
#include "Shader.h"
#include "PipeLine.h"

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

	//デスクリプタレンジ
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
		Vector3 pos;//xyz座標
		Vector2 uv;
	};


	Vertex vertices_[6] = {};

	uint32_t sizeVB_;

	HRESULT result_;

	//頂点レイアウト(要素を増やすなら配列数を増やす)
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayouts_;

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
	Microsoft::WRL::ComPtr<ID3D12Resource>texBuff_[2];

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
	Shader vs;
	//ピクセルシェーダオブジェクト
	Shader ps;

	//頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob_ = nullptr;

	//ピクセルシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob_ = nullptr;

	//テクスチャサンプラー
	D3D12_STATIC_SAMPLER_DESC sampleDesc_{};

	//デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange_{};
	D3D12_DESCRIPTOR_RANGE descriptorRange2_{};

	//グラフィックスパイプラインの各ステージの設定をする構造体を用意
	PipeLine pipeline_;
	PipeLine pipeline2_;
	PipeLine pipeline3_;
	PipeLine pipeline4_;


	//ルートパラメータ(定数バッファの数が増えたら配列の要素数を増やして設定をしている関数の中身にも追加すること)
	D3D12_ROOT_PARAMETER rootParam_[4] = {};

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature_;

	//パディングの対応の為全てvector4を使っている
	struct  ConstBufferMaterial
	{

		Vector4 color;
		Vector4 sigma;//ブルームを掛けるときのブラー用の値(xのみ)
		Vector4 grayScaleStep;//ブルーム用のグレースケールのsmoothstep用の大きさ(x,yのみ)

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

	//定数バッファに送るとき用の変数
	float sigma_ = 0.0025f;
	float stepWidth_ = 0.001f;
	float grayScaleStep_[2] = {0.6f,0.9f};

	//平行投影
	Matrix4x4 matProjection_;

	bool cheng_ = true;

};

