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

	static Shader ShaderLoad(std::string id, std::string ShaderName, std::string EntryPoint, std::string ShaderModelName);

	static Shader SearchShaderData(std::string id);

	static void RegisterShader(std::string id, Shader shader);

private:

	

	class ShaderData
	{
	public:

		static ShaderData* GetInstance() {
			static ShaderData instance;
			return &instance;
		};

		//サウンドデータの連想配列
		std::map<std::string, Shader> Shaders_;


	private:
		ShaderData() = default;
		~ShaderData() {};

		ShaderData(const ShaderData&) = delete;
		ShaderData& operator=(const ShaderData&) = delete;
		

	};

	


};


