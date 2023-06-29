#pragma once
//D3D�R���p�C��
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

//ComPtr�p�C���N���[�h
#include <wrl.h>

#include <vector>
#include <memory>
#include <map>
#include <string>



class Shader
{
public:
	

	Microsoft::WRL::ComPtr<ID3DBlob> blob_ = nullptr;

	//�G���[�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_ = nullptr;

	bool Roadsuccess = false;

	Shader() {};
	/// <summary>
	/// �w�肵�ēǂݍ���
	/// </summary>
	/// <param name="ShaderName">�ǂݍ��݂����V�F�[�_��(�p�X)</param>
	/// <param name="EntryPoint">�G���g���[�|�C���g</param>
	/// <param name="ShaderModelName">�V�F�[�_���f����</param>
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

		//�T�E���h�f�[�^�̘A�z�z��
		std::map<std::string, Shader> Shaders_;


	private:
		ShaderData() = default;
		~ShaderData() {};

		ShaderData(const ShaderData&) = delete;
		ShaderData& operator=(const ShaderData&) = delete;
		

	};

	


};


