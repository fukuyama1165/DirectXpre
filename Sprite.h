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
using namespace DirectX;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "matrix4x4.h"
#include "Vector3.h"
#include "Vector4.h"

//ComPtr用インクルード
#include <wrl.h>

#include "SpriteCommon.h"
#include "Texture.h"

class Sprite
{
public:
	Sprite();

	void initialize(ID3D12Device* dev, SpriteCommon* spriteCommon);

	void Draw(ID3D12GraphicsCommandList* cmdList, int ChangeTexure = 1, int PipeLineRuleFlag=0);

	void setColor(Vector4 color);

private:

	void vertexBuffGeneration(ID3D12Device* dev);

	//頂点レイアウトの設定
	void vertexLayout();

	//グラフィックスパイプラインの設定
	void graphicPipelineGeneration();

	//ルートパラメータの設定(定数バッファとシェーダについて)
	void rootParamGeneration();

	//ルートシグネチャ
	void rootsignatureGeneration(ID3D12Device* dev);

	//定数バッファ
	void constantBuffGeneration(ID3D12Device* dev);

	void descriptorRangeGeneration();

	//テクスチャサンプラーの設定
	void textureSamplerGeneration();

	/// <summary>
	/// 二次元のアフィン変換をする関数
	/// </summary>
	/// <param name="box">頂点データ(X,Y)</param>
	/// <param name="box">図形の中の原点(X,Y)</param>
	/// <param name="moveX">x方向の移動量</param>
	/// <param name="moveY">y方向の移動量</param>
	/// <param name="rotate">回転角度(度数法)</param>
	/// <param name="scaleX">x方向の拡大率</param>
	/// <param name="scaleY">y方向の拡大率</param>
	XMFLOAT3 Afin(XMFLOAT3 box, float moveX, float moveY, float rotate, float scaleX, float scaleY);

	void ObjAfin(float moveX, float moveY, float rotate, float scaleX, float scaleY);

	void vertexMap();

public:

	float posX = 0;
	float posY = 0;
	float rotate = 0;
	float scaleX = 1;
	float scaleY = 1;

	

private:

	SpriteCommon* common;

	//頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos;//xyz座標
		XMFLOAT2 uv;
	};

	Vertex vertices[6] = {};

	UINT sizeVB;

	HRESULT result;

	//頂点レイアウト(要素を増やすなら配列数を増やす)
	D3D12_INPUT_ELEMENT_DESC inputLayout[2];

	//エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	//頂点バッファ用変数
	D3D12_HEAP_PROPERTIES heapprop{};//ヒープ設定
	D3D12_RESOURCE_DESC resDesc{};//リソース設定
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
	Vertex* vertMap;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};


	//デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	//テクスチャサンプラー
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};


	//グラフィックスパイプラインの各ステージの設定をする構造体を用意
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline2{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline3{};//加算
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline4{};//減算
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline5{};//色反転

	//ルートパラメータ(定数バッファの数が増えたら配列の要素数を増やして設定をしている関数の中身にも追加すること)
	D3D12_ROOT_PARAMETER rootParam[2] = {};

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;

	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate2 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate3 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate4 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate5 = nullptr;

	struct  ConstBufferMaterial
	{

		XMFLOAT4 color;

	};

	//定数バッファそのもの

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	//マッピングするときのポインタ
	ConstBufferMaterial* constMapMaterial = nullptr;

	struct  ConstBufferPos
	{

		XMFLOAT4 posM;

	};

	//定数バッファそのもの

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffPos = nullptr;
	//マッピングするときのポインタ
	ConstBufferPos* constMapPos = nullptr;


	struct  ConstBufferWidth
	{

		float width;
		float height;

	};

	//定数バッファそのもの

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffWidth = nullptr;
	//マッピングするときのポインタ
	ConstBufferWidth* constMapWidth = nullptr;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};

	//定数バッファ用のリソース設定関数
	D3D12_RESOURCE_DESC constBuffResourceGeneration(int size);

	static Texture* texture;


	float oldPosX;
	float oldPosY;

};