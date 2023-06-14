#pragma once

//D3D�R���p�C��
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<d3d12.h>
#pragma comment(lib,"d3d12.lib")

#include <vector>
#include <memory>

#include "DirectXInit.h"

#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "matrix4x4.h"
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>

#include "Model.h"

//�l�̃R�[�h���č����

//���ʂ�transform���Ǝv��
struct NodeUnit
{
	std::string name_;
	std::vector<Vector3> position_;
	std::vector<double> positionTime_;
	std::vector<Vector3> rotation_;
	std::vector<double> totationTime_;
	std::vector<Vector3> scale_;
	std::vector<double> scaleTime_;

};

//�A�j���[�V�����̓����ׂ̈̍\����(�i���Ƃ����O�Ƃ�)
struct Animation
{

	std::string name_;
	double duration_;//�Ԋu
	double ticksPerSecond_;//�o��?����
	std::vector< NodeUnit> channels_;//���Ԃ񕔈ʒP�ʂ̈ʒu�������Ă�

};

//��P�ʂ��Ǝv��(���_�݂����́H)
struct Node
{
	//���O(���Ԃ񓮂����ۂɂ��̕����̂���������߂ɕK�v�ɂȂ�H)
	std::string name_;

	//���f�����ď����Ă��邯�ǒP�ʓI�Ƀ��b�V���ɂȂ�
	std::vector<std::unique_ptr<Model>> meshes_;

	//��������P�̂̍s��⃍�[�J�����W

	//���Ԃ�XMVECTOR�ł����󂯎��Ȃ��H
	//�傫��
	DirectX::XMVECTOR scale_ = { 1,1,1,0 };
	//�p�x
	DirectX::XMVECTOR rotation_ = { 0,0,0,0 };
	//�ʒu
	DirectX::XMVECTOR translation_ = { 0,0,0,1 };

	//���[�J�����W�s��
	DirectX::XMMATRIX transform_ = DirectX::XMMatrixIdentity();

	//�O���[�o��(���[���h)���W�s��?
	DirectX::XMMATRIX globalTransform_ = DirectX::XMMatrixIdentity();

	//�O���[�o��(���[���h)���W�̋t�s��
	DirectX::XMMATRIX globalInverseTransform_ = DirectX::XMMatrixIdentity();

	//�e�̃A�j���[�V�����s��?
	DirectX::XMMATRIX animaetionParentMat_ = DirectX::XMMatrixIdentity();

	//�e�I�u�W�F�N�g
	Node* parent_ = nullptr;

};

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

//�����object3D�Ɏ������邱�Ƃňʒu���ړ��������肷��
class AnimationModel
{

public:
	//texture�̃C���X�^���X��������Ă���
	DirectXInit* directXinit_ = DirectXInit::GetInstance();
	Texture* texture_ = Texture::GetInstance();

	//���_�ʒu�Q�H
	std::vector<std::unique_ptr<Node>> nodes_;
	std::vector<std::unique_ptr<Animation>> animations_;
	std::vector<Bone> bones;

	//�f�X�g���N�^
	~AnimationModel();

	//�t�@�C����
	std::string filename_;

	//aiscene�������Ă���
	//�ǂݍ���
	bool Load(std::string filename, std::string fileType);

	/// <summary>
	/// aiscene�ɓ����Ă�m�[�h�̏����R�s�[����
	/// </summary>
	/// <param name="node">�m�[�h(�ǉ����������)</param>
	/// <param name="scene">�S��(�ǂ̈ʒu�ɂ��邩���f���邽��?)</param>
	/// <param name="targetParent">�e�q�֌W������Ȃ�����</param>
	void CopyNodeMesh(const aiNode* node,const aiScene* scene,const Node* targetParent = nullptr);

	void MeshAssignment(const aiMesh* mesh,const aiScene* scene)

};