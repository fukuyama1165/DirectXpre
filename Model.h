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
	void vertexBuffObjGeneration(ID3D12Device* dev);

	//�C���f�b�N�X�f�[�^�֘A(�C���f�b�N�X�r���[������)
	void indicesBuffGeneration(ID3D12Device* dev);

private:

	//���_�f�[�^�\����
	struct Vertex
	{
		Vector3 pos;//xyz���W
		Vector3 normal;//�@���x�N�g��
		Vector3 uv;//uv���W
	};

	struct Material
	{
		std::string name;//�}�e���A����
		XMFLOAT3 ambient;//�A���r�G���g�e���x
		XMFLOAT3 diffuse;//�f�B�t���[�Y�e���x
		XMFLOAT3 specular;//�X�y�L�����[�e���x
		float alpha;//�A���t�@
		std::string textureFilename;//�e�N�X�`���t�@�C����

		//�R���X�g���N�^
		Material()
		{
			ambient = { 0.3f, 0.3f, 0.3f };
			diffuse = { 0.0f, 0.0f, 0.0f };
			specular = { 0.0f, 0.0f, 0.0f };
			alpha = 1.0f;
		}

	};

	//���_�f�[�^(���₵�����Ȃ炱�������₵�Ă���)
	std::vector <Vertex> vertices;

	//���_�f�[�^�T�C�Y
	UINT sizeVB;

	HRESULT result;

	//���_�o�b�t�@�p�ϐ�
	D3D12_HEAP_PROPERTIES heapprop{};//�q�[�v�ݒ�
	D3D12_RESOURCE_DESC resDesc{};//���\�[�X�ݒ�
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
	Vertex* vertMap = nullptr;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//�C���f�b�N�X�f�[�^
	std::vector< unsigned short> indices;
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB;
	//�C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;
	//�C���f�b�N�X�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};

	//���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;

	//�e�N�X�`��
	static Texture* texture;

	//�}�e���A��
	Material material;

};

Model::Model()
{
}

Model::~Model()
{
}
