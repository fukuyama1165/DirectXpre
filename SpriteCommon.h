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

	//ジオメトリシェーダの読み込みとコンパイル
	void geometryShaderGeneration();


	//頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> getVsBlob() { return vsBlob; };

	//ピクセルシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> getPsBlob() { return psBlob; };

	//ピクセルシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> getGsBlob() { return gsBlob; };


private:



	HRESULT result;




	//頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;

	//ピクセルシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> gsBlob = nullptr;

	//エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;




};