#pragma once

/**
 * @file SpriteCommon.h
 * @brief 2Dスプライトの共通設定クラス
 * @author フクヤマ
 * @date 2023_12/29
 */

#include<d3dcompiler.h>

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include<d3d12.h>
#include<dxgi1_6.h>

//数学ライブラリ(最適化されすぎて使いにくいらしいのであとで自作しろって)
#include<DirectXMath.h>

#include "matrix4x4.h"
#include "Vector3.h"

//ComPtr用インクルード
#include <wrl.h>

class SpriteCommon
{
public:
	static SpriteCommon* GetInstance();

	void initialize();

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

	SpriteCommon() = default;
	~SpriteCommon();

	SpriteCommon(const SpriteCommon&) = delete;
	SpriteCommon& operator=(const SpriteCommon&) = delete;

private:



	HRESULT result = S_OK;




	//頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;

	//ピクセルシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> gsBlob = nullptr;

	//エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;




};