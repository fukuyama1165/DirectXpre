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
	std::string name;
	std::vector<Vector3> position;
	std::vector<double> positionTime;
	std::vector<Vector3> rotation;
	std::vector<double> totationTime;
	std::vector<Vector3> scale;
	std::vector<double> scaleTime;

};

//�A�j���[�V�����̓����ׂ̈̍\����(�i���Ƃ����O�Ƃ�)
struct Animation
{

	std::string name;
	double duration;//�Ԋu
	double ticksPerSecond;//�o��?����
	std::vector< NodeUnit> channels;//���Ԃ񕔈ʒP�ʂ̈ʒu�������Ă�

};

//��P�ʂ��Ǝv��(���_�݂����́H)
struct Node
{
	//���O(���Ԃ񓮂����ۂɂ��̕����̂���������߂ɕK�v�ɂȂ�H)
	std::string name;

	//���f�����ď����Ă��邯�ǒP�ʓI�Ƀ��b�V���ɂȂ�
	std::vector<std::unique_ptr<Model>> meshes;

	//��������P�̂̍s��⃍�[�J�����W

	//���Ԃ�XMVECTOR�ł����󂯎��Ȃ��H
	//�傫��
	DirectX::XMVECTOR scale = { 1,1,1,0 };
	//�p�x
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//�ʒu
	DirectX::XMVECTOR translation = { 0,0,0,1 };

	//���[�J�����W�s��
	DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();

	//�O���[�o��(���[���h)���W�s��?
	DirectX::XMMATRIX globalTransform = DirectX::XMMatrixIdentity();

	//�O���[�o��(���[���h)���W�̋t�s��
	DirectX::XMMATRIX globalInverseTransform = DirectX::XMMatrixIdentity();

	//�e�̃A�j���[�V�����s��?
	DirectX::XMMATRIX animaetionParentMat = DirectX::XMMatrixIdentity();

	//�e�I�u�W�F�N�g
	Node* parent = nullptr;

};

//�ꂩ���Ɏ��Ă�ő�{�[����?
static const unsigned short int NUM_BONES_PER_VERTEX = 4;

//�ő�{�[����
static const unsigned short int MAX_BONE = 128;

//���_�f�[�^
struct AnimationVertex
{

	Vector3 pos;
	Vector3 normal;
	Vector2 uv;
	unsigned int ids[NUM_BONES_PER_VERTEX] = {};
	float weights[NUM_BONES_PER_VERTEX] = {};

};

//�萔�o�b�t�@�p
struct ConstBuffSkin
{
	Matrix4x4 boneMats[MAX_BONE] = {};
};

//�E�G�C�g�̏��
struct SetWeight
{
	unsigned int id;
	float weight;
};

//�{�[���̏��
struct Bone
{
	//�ǂ��̂��
	std::string name;

	//�����ʒu
	Matrix4x4 offsetMatrix;

	//�ŏI�ʒu?
	Matrix4x4 finalMatrix;

};

//�����object3D�Ɏ������邱�Ƃňʒu���ړ��������肷��
class AnimationModel
{

public:
	//texture�̃C���X�^���X��������Ă���
	DirectXInit* directXinit = DirectXInit::GetInstance();
	Texture* texture = Texture::GetInstance(directXinit->Getdev().Get());

	//���_�ʒu�Q�H
	std::vector<std::unique_ptr<Node>> nodes;
	std::vector<std::unique_ptr<Animation>> animations;
	std::vector<Bone> bones;

	//�f�X�g���N�^
	~AnimationModel();

	//�t�@�C����
	std::string filename;

	//aiscene�������Ă���
	//�ǂݍ���
	bool Load(std::string filename, std::string fileType);

	/// <summary>
	/// aiscene�ɓ����Ă�m�[�h�̏����R�s�[����
	/// </summary>
	/// <param name="node">�m�[�h(�ǉ����������)</param>
	/// <param name="scene">�S��(�ǂ̈ʒu�ɂ��邩���f���邽��?)</param>
	/// <param name="targetParent">�e�q�֌W������Ȃ�����</param>
	void CopyNodeMesh(aiNode* node, aiScene* scene, Node* targetParent = nullptr);

	void MeshAssignment(aiMesh* mesh,const aiScene* scene,)

};