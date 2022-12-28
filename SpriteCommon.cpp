#include "SpriteCommon.h"

void SpriteCommon::initialize(ID3D12Device* dev)
{

	vertexShaderGeneration();

	pixelShaderGeneration();

	//geometryShaderGeneration();

}



void SpriteCommon::vertexShaderGeneration()
{
	//頂点シェーダファイルの読み込みとコンパイル
#pragma region 

	//頂点シェーダファイルの読み込み辺
	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/SpriteVS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"vs_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&vsBlob,
		&errorBlob
	);

#pragma endregion 

	//頂点シェーダの読み込み時のエラーを表示する場所
#pragma region 

	//頂点シェーダの読み込み時のエラーを表示する場所
	if (FAILED(result))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void SpriteCommon::pixelShaderGeneration()
{
	//ピクセルシェーダの読み込みとコンパイル
#pragma region 

	//ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/SpritePS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"ps_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&psBlob,
		&errorBlob
	);

#pragma endregion


	//ピクセルシェーダの読み込み時のエラーを表示する場所
#pragma region 

	//ピクセルシェーダの読み込み時のエラーを表示する場所
	if (FAILED(result))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void SpriteCommon::geometryShaderGeneration()
{
	//ジオメトリシェーダの読み込みとコンパイル
#pragma region 

//	//ジオメトリシェーダの読み込みとコンパイル
//	result = D3DCompileFromFile(
//		L"Resources/shaders/SpriteGS.hlsl",//シェーダファイル名
//		nullptr,
//		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
//		"main",//エントリーポイント名
//		"ps_5_0",//シェーダモデル指定
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
//		0,
//		&gsBlob,
//		&errorBlob
//	);
//
//#pragma endregion
//
//
//	//ジオメトリシェーダの読み込み時のエラーを表示する場所
//#pragma region 
//
//	//ジオメトリシェーダの読み込み時のエラーを表示する場所
//	if (FAILED(result))
//	{
//		//errorBlobからエラー内容をstring型にコピー
//		std::string errstr;
//		errstr.resize(errorBlob->GetBufferSize());
//
//		std::copy_n((char*)errorBlob->GetBufferPointer(),
//			errorBlob->GetBufferSize(),
//			errstr.begin());
//		errstr += "\n";
//		//エラー内容をウィンドウに表示
//		OutputDebugStringA(errstr.c_str());
//		exit(1);
//	}

#pragma endregion

}

