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

//キーボードやコントローラーなどの入力するヘッダとライブラリのリンク
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")

#pragma comment(lib,"dxguid.lib")

const float PI = 3.141592653589;

#include <DirectXTex.h>

class DrawingInit
{
public:
	DrawingInit();
	~DrawingInit();

	//頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos;//xyz座標
		XMFLOAT2 uv;//uv座標
	};

	//頂点データ(増やしたいならここも増やしておく)
	Vertex vertices[4];

	//頂点データサイズ
	UINT sizeVB;

	HRESULT result;

	//頂点バッファ生成
	void vertexBuffGeneration(ID3D12Device* dev);
	//頂点バッファ用変数
	D3D12_HEAP_PROPERTIES heapprop{};//ヒープ設定
	D3D12_RESOURCE_DESC resDesc{};//リソース設定
	ID3D12Resource* vertBuff = nullptr;
	Vertex* vertMap = nullptr;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//頂点シェーダの読み込みとコンパイル
	void vertexShaderGeneration();
	//ピクセルシェーダの読み込みとコンパイル
	void pixelShaderGeneration();
	ID3DBlob* vsBlob = nullptr;//頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr;//ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr;//エラーオブジェクト

	//頂点レイアウトの設定
	void vertexLayout();
	//頂点レイアウト(要素を増やすなら配列数を増やす)
	D3D12_INPUT_ELEMENT_DESC inputLayout[2];

	//グラフィックスパイプラインの設定
	void graphicPipelineGeneration();
	//グラフィックスパイプラインの各ステージの設定をする構造体を用意
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline2{};

	//デスクリプタレンジの設定
	void descriptorRangeGeneration();
	//デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	//ルートパラメータの設定(定数バッファとシェーダについて)
	void rootParamGeneration();
	//ルートパラメータ(定数バッファの数が増えたら配列の要素数を増やして設定をしている関数の中身にも追加すること)
	D3D12_ROOT_PARAMETER rootParam[3] = {};
	
	void textureSamplerGeneration();
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};

	void rootsignatureGeneration(ID3D12Device* dev);

	

private:

};

