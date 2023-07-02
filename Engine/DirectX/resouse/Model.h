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
#include "Vector2.h"
#include "matrix4x4.h"
#include "Texture.h"
#include "Material.h"

#include <unordered_map>


//�ꂩ���Ɏ��Ă�ő�{�[����?
static const uint32_t SNUM_BONES_PER_VERTEX = 4;

//�ő�{�[����
static const uint32_t SMAX_BONE = 128;

//���_�f�[�^
struct AnimationVertex
{

	Vector3 pos_;
	Vector3 normal_;
	Vector2 uv_;
	uint32_t ids_[SNUM_BONES_PER_VERTEX] = {};
	float weights_[SNUM_BONES_PER_VERTEX] = {};

};

//�萔�o�b�t�@�p
struct ConstBuffSkin
{
	Matrix4x4 boneMats_[SMAX_BONE] = {};
};

//�E�G�C�g�̏��
struct SetWeight
{
	uint32_t id_;
	float weight_;
};

//�{�[���̏��
struct Bone
{
	//�ǂ��̂��
	std::string name_;

	//�����ʒu
	Matrix4x4 offsetMatrix_;

	//�ŏI�ʒu?
	Matrix4x4 finalMatrix_;

};

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

public:

	//���_�f�[�^(���₵�����Ȃ炱�������₵�Ă���)
	std::vector <Vertex> vertices_;


	//�C���f�b�N�X�f�[�^
	std::vector< unsigned short> indices_;

};

//�A�j���[�V�����p�̃��b�V��
class AnimationMesh
{
public:
	AnimationMesh();
	~AnimationMesh();

	void Init();

	

private:

	void SetSizeVB();

	void SetSizeIB();

	//���_�o�b�t�@
	void VertexBuffObjGeneration(const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state);
	void VertexBuffMap();
	void VertexBuffViewGeneration();

	//�C���f�b�N�X�f�[�^�֘A(�C���f�b�N�X�r���[������)
	
	void IndicesBuffGeneration(const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state);
	void IndicesBuffMap();
	void IndicesBuffViewGeneration(DXGI_FORMAT format);

private:
	
	


	//���_�f�[�^�T�C�Y
	uint32_t sizeVB_;

	HRESULT result_;

	
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	AnimationVertex* vertMap_ = nullptr;
	
	

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	uint32_t sizeIB_;
	//�C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	

	//���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData_;

	

public:

	

	//���_�f�[�^(���₵�����Ȃ炱�������₵�Ă���)
	std::vector <AnimationVertex> vertices_;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView_{};



	//�C���f�b�N�X�f�[�^
	std::vector< unsigned short> indices_;

	//�C���f�b�N�X�r���[
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	//�e�N�X�`��
	static Texture* STexture_;

	//�ǂ̃e�N�X�`�������f���邽�߂̒l(��ł�����ς���)
	std::vector <uint32_t> textureHandle = {};

	//�}�e���A��(�����ɑΉ����邽��)
	std::vector<Material> material_;

};