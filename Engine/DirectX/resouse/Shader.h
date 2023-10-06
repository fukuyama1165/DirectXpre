#pragma once
//D3Dコンパイラ
#include<d3dcompiler.h>
//ComPtr用インクルード
#include <wrl.h>

#include <vector>
#include <memory>
#include <map>
#include <string>



class Shader
{
public:
	
	std::string name_ = "";

	Microsoft::WRL::ComPtr<ID3DBlob> blob_ = nullptr;

	//エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_ = nullptr;

	bool Roadsuccess = false;

	Shader() {};
	/// <summary>
	/// 指定して読み込み
	/// </summary>
	/// <param name="id">呼び出すとき使用するid</param>
	/// <param name="ShaderName">読み込みたいシェーダ名(パス)</param>
	/// <param name="EntryPoint">エントリーポイント</param>
	/// <param name="ShaderModelName">シェーダモデル名</param>
	Shader(std::string id, std::string ShaderName,std::string EntryPoint,std::string ShaderModelName);

	/// <summary>
	/// 指定した物を生成してデータに登録(コンストラクタとの違いは配列に入れるかどうか基本的にこっちを呼び出す)
	/// </summary>
	/// <param name="id">呼び出すとき使用するid</param>
	/// <param name="ShaderName">読み込みたいシェーダ名(パス)</param>
	/// <param name="EntryPoint">エントリーポイント</param>
	/// <param name="ShaderModelName">シェーダモデル名</param>
	/// <returns>登録したシェーダ</returns>
	static Shader ShaderLoad(std::string id, std::string ShaderName, std::string EntryPoint, std::string ShaderModelName);

	/// <summary>
	///シェーダを探して返す(なければ何も入っていないシェーダを返す)
	/// </summary>
	/// <param name="id">探したいid</param>
	/// <returns>結果</returns>
	static Shader SearchShaderData(std::string id);

	/// <summary>
	/// 配列に登録
	/// </summary>
	/// <param name="id">呼び出すとき使用するid</param>
	/// <param name="shader">登録したいシェーダ</param>
	static void RegisterShader(std::string id, Shader shader);

private:

	

	class ShaderData
	{
	public:

		static ShaderData* GetInstance() {
			static ShaderData instance;
			return &instance;
		};

		//シェーダデータの連想配列
		std::map<std::string, Shader> Shaders_;


	private:
		ShaderData() = default;
		~ShaderData() {};

		ShaderData(const ShaderData&) = delete;
		ShaderData& operator=(const ShaderData&) = delete;
		

	};

	


};


