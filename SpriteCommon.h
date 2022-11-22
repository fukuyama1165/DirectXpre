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
#include "Float3.h"

//ComPtr用インクルード
#include <wrl.h>

class SpriteCommon
{
public:
	void initialize(ID3D12Device* dev);

	//頂点シェーダの読み込みとコンパイル
	void vertexShaderGeneration();

	//ピクセルシェーダの読み込みとコンパイル
	void pixelShaderGeneration();

	//頂点レイアウトの設定
	void vertexLayout();

	//グラフィックスパイプラインの設定
	void graphicPipelineGeneration();

	//ルートシグネチャ
	void rootsignatureGeneration(ID3D12Device* dev);

	
	Microsoft::WRL::ComPtr<ID3D12RootSignature> getRootsignature() { return rootsignature; };

	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> getPipelinestate() { return pipelinestate; };
	Microsoft::WRL::ComPtr<ID3D12PipelineState> getPipelinestate2() { return pipelinestate2; };
	Microsoft::WRL::ComPtr<ID3D12PipelineState> getPipelinestate3() { return pipelinestate3; };
	Microsoft::WRL::ComPtr<ID3D12PipelineState> getPipelinestate4() { return pipelinestate4; };
	Microsoft::WRL::ComPtr<ID3D12PipelineState> getPipelinestate5() { return pipelinestate5; };
	

private:



	HRESULT result;

	


	//頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;

	//ピクセルシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;

	//エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	//頂点レイアウト(要素を増やすなら配列数を増やす)
	D3D12_INPUT_ELEMENT_DESC inputLayout[1];

	//グラフィックスパイプラインの各ステージの設定をする構造体を用意
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline2{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline3{};//加算
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline4{};//減算
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline5{};//色反転

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;

	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate2 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate3 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate4 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate5 = nullptr;

	struct  ConstBuffermaterial
	{
		
		XMFLOAT4 color;

	};

};

