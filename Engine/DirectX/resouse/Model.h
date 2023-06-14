#pragma once
//D3D�R���p�C��
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<d3d12.h>
#pragma comment(lib,"d3d12.lib")

#include <vector>

#include "DirectXInit.h"

#include "Vector4.h"
#include "Vector3.h"
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>


bool DoTheImportThing(const std::string& pFile);

class Model
{
public:
	Model();
	~Model();

private:

	//���_�o�b�t�@
	void vertexBuffObjGeneration();

	//�C���f�b�N�X�f�[�^�֘A(�C���f�b�N�X�r���[������)
	void indicesBuffGeneration();

private:

	//���_�f�[�^�\����
	struct Vertex
	{
		Vector3 pos_;//xyz���W
		Vector3 normal_;//�@���x�N�g��
		Vector3 uv_;//uv���W
	};

	struct Material
	{
		std::string name_;//�}�e���A����
		XMFLOAT3 ambient_;//�A���r�G���g�e���x
		XMFLOAT3 diffuse_;//�f�B�t���[�Y�e���x
		XMFLOAT3 specular_;//�X�y�L�����[�e���x
		float alpha_;//�A���t�@
		std::string textureFilename_;//�e�N�X�`���t�@�C����

		//�R���X�g���N�^
		Material()
		{
			ambient_ = { 0.3f, 0.3f, 0.3f };
			diffuse_ = { 0.0f, 0.0f, 0.0f };
			specular_ = { 0.0f, 0.0f, 0.0f };
			alpha_ = 1.0f;
		}

	};

	//���_�f�[�^(���₵�����Ȃ炱�������₵�Ă���)
	std::vector <Vertex> vertices_;

	//���_�f�[�^�T�C�Y
	uint32_t sizeVB_;

	HRESULT result_;

	//���_�o�b�t�@�p�ϐ�
	D3D12_HEAP_PROPERTIES heapprop_{};//�q�[�v�ݒ�
	D3D12_RESOURCE_DESC resDesc_{};//���\�[�X�ݒ�
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	Vertex* vertMap_ = nullptr;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//�C���f�b�N�X�f�[�^
	std::vector< unsigned short> indices_;
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	uint32_t sizeIB_;
	//�C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	//�C���f�b�N�X�r���[
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	//���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData_;

	//�e�N�X�`��
	static Texture* STexture_;

	//�}�e���A��
	Material material_;

};

Model::Model()
{
}

Model::~Model()
{
}
