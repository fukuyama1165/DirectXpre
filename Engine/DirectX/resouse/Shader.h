#pragma once
//D3Dコンパイラ
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

//ComPtr用インクルード
#include <wrl.h>

#include <vector>
#include <memory>
#include <map>
#include <string>



class Shader
{
public:
	

	Microsoft::WRL::ComPtr<ID3DBlob> blob_ = nullptr;

	//エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_ = nullptr;

	bool Roadsuccess = false;

	Shader() {};
	/// <summary>
	/// 指定して読み込み
	/// </summary>
	/// <param name="ShaderName">読み込みたいシェーダ名(パス)</param>
	/// <param name="EntryPoint">エントリーポイント</param>
	/// <param name="ShaderModelName">シェーダモデル名</param>
	Shader(std::string ShaderName,std::string EntryPoint,std::string ShaderModelName);

private:

	

	class ShaderDate
	{
	public:

		static ShaderDate* GetInstance() {
			static ShaderDate instance;
			return &instance;
		};

	private:

		//サウンドデータの連想配列
		std::map<std::string, Shader> Shaders_;


	private:
		ShaderDate() = default;
		~ShaderDate() {};

		ShaderDate(const ShaderDate&) = delete;
		ShaderDate& operator=(const ShaderDate&) = delete;
		

	};

	


};


