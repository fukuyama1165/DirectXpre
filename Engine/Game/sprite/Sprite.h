#pragma once

#include <fstream>
#include <sstream>

#include "matrix4x4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vector2.h"

#include "SpriteCommon.h"
#include "Texture.h"
#include "WinApp.h"

class Sprite
{
public:
	Sprite();

	void initialize(std::string textureIndex);

	void Update();

	void Draw(std::string ChangeTexure = "", uint16_t PipeLineRuleFlag = 0);

	void setColor(const Vector4& color);

	void SetAnchorPoint(const Vector2& AnchorPoint) { anchorPoint_ = AnchorPoint; changeFlag = true; };

	void SetTextureRect(float srcX, float srcY, float width, float height);

	//現在のテクスチャのサイズを返す
	Vector2 GetNowTextureSize() { return textureSize_; };

	//今使用しているテクスチャのサイズを返す
	Vector2 GetTextureSize() {
		Vector2 textureDescSize = {
	   static_cast<float>(STexture_->GetTexBuff(texIndex)->GetDesc().Width),
	   static_cast<float>(STexture_->GetTexBuff(texIndex)->GetDesc().Height)
		};
		return textureDescSize;
	 };


	void ChangeTexture(std::string textureIndex);



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

	void indicesBuffGeneration();

	//サイズをテクスチャに合わせる
	void AdjustTexSize(std::string textureIndex);

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

	

private:

	SpriteCommon* common_ = nullptr;

	//頂点データ構造体
	struct Vertex
	{
		Vector3 pos;//xyz座標
		Vector2 uv;
	};


	std::vector <Vertex> vertices_;

	uint32_t sizeVB_ = 0;

	HRESULT result_ = S_OK;

	//頂点レイアウト(要素を増やすなら配列数を増やす)
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout_;

	//エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_ = nullptr;

	//頂点バッファ用変数
	D3D12_HEAP_PROPERTIES heapprop_{};//ヒープ設定
	D3D12_RESOURCE_DESC resDesc_{};//リソース設定
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	Vertex* vertMap_ = nullptr;
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
	std::vector<D3D12_ROOT_PARAMETER> rootParam_ = {};

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

	//インデックスデータ
	std::vector< unsigned short> indices_;
	//インデックスデータ全体のサイズ
	uint32_t sizeIB_ = 0;
	//インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	//インデックスビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	static Texture* STexture_;


	Vector2 textureSize_ = { 100.0f,100.0f };


	std::string texIndex = "";

	//平行投影
	Matrix4x4 matProjection_;

	Matrix4x4 matScale_;
	Matrix4x4 matRotate_;
	Matrix4x4 matTrans_;

	Matrix4x4 matWorld_;

	Vector2 srcTexturePos_ = { 0,0 };

	Vector2 anchorPoint_ = { 0.5f,0.5f };

	bool changeFlag = false;

};