#include "Shader.h"

Shader::Shader(std::string ShaderName, std::string EntryPoint, std::string ShaderModelName)
{
	
#pragma region 

	HRESULT result;

	result = D3DCompileFromFile(
		std::wstring(ShaderName.begin(), ShaderName.end()).c_str(),//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		EntryPoint.c_str(),//エントリーポイント名
		ShaderModelName.c_str(),//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&blob_,
		&errorBlob_
	);

#pragma endregion 

	//読み込み時のエラーを表示する場所
#pragma region 

	//読み込み時のエラーを表示する場所
	if (FAILED(result))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	Roadsuccess = result;

#pragma endregion
}

