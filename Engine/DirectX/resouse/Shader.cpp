#include "Shader.h"

Shader::Shader(std::string ShaderName, std::string EntryPoint, std::string ShaderModelName)
{
	
#pragma region 

	HRESULT result;

	result = D3DCompileFromFile(
		std::wstring(ShaderName.begin(), ShaderName.end()).c_str(),//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		EntryPoint.c_str(),//�G���g���[�|�C���g��
		ShaderModelName.c_str(),//�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&blob_,
		&errorBlob_
	);

#pragma endregion 

	//�ǂݍ��ݎ��̃G���[��\������ꏊ
#pragma region 

	//�ǂݍ��ݎ��̃G���[��\������ꏊ
	if (FAILED(result))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	Roadsuccess = result;

#pragma endregion
}

