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
#include "Material.h"

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
#pragma warning(push)
#pragma warning(disable: 4324)
typedef struct Node
{
	//���O(���Ԃ񓮂����ۂɂ��̕����̂���������߂ɕK�v�ɂȂ�H)
	std::string name_;

	//���b�V��
	std::vector<std::unique_ptr<AnimationMesh>> meshes_;

	//��������P�̂̍s��⃍�[�J�����W

	//���Ԃ�XMVECTOR�ł����󂯎��Ȃ��H
	//�傫��
	DirectX::XMVECTOR scale_ = { 1,1,1,0 };
	//�p�x
	DirectX::XMVECTOR rotation_ = { 0,0,0,0 };
	//�ʒu
	DirectX::XMVECTOR translation_ = { 0,0,0,1 };

	//���[�J�����W�s��
	Matrix4x4 transform_ = Matrix4x4::Identity();

	//�O���[�o��(���[���h)���W�s��?
	Matrix4x4 globalTransform_ = Matrix4x4::Identity();

	//�O���[�o��(���[���h)���W�̋t�s��
	Matrix4x4 globalInverseTransform_ = Matrix4x4::Identity();

	//�e�̃A�j���[�V�����s��?
	Matrix4x4 animaetionParentMat_ = Matrix4x4::Identity();

	//�e�I�u�W�F�N�g
	Node* parent_ = nullptr;

}Node;
#pragma warning(pop)




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
	std::string materialName_;

	//aiscene�������Ă���
	
	bool Load(std::string filename, std::string fileType, std::string materialName = "", std::string materialType = "png");

	void Draw()const;

	/// <summary>
	/// aiscene�ɓ����Ă�m�[�h�̏����R�s�[����
	/// </summary>
	/// <param name="node">�m�[�h(�ǉ����������)</param>
	/// <param name="scene">�S��(�ǂ̈ʒu�ɂ��邩���f���邽��?)</param>
	/// <param name="targetParent">�e�q�֌W������Ȃ�����</param>
	void CopyNodeMesh(const aiNode* node,const aiScene* scene, Node* targetParent = nullptr);

	/// <summary>
	/// ���b�V���̏�������
	/// </summary>
	/// <param name="mesh">���ꂽ�����b�V��</param>
	/// <param name="scene">�ǂݍ���aiScene</param>
	/// <param name="model">�ۑ�����ꏊ</param>
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, AnimationMesh& model);

	/// <summary>
	/// �}�e���A���ɂ���e�N�X�`����ǂݍ��ނ��
	/// </summary>
	/// <param name="material">�Y������}�e���A��</param>
	/// <param name="type">�󂯎�肽����̏���?</param>
	/// <param name="model">�ۑ�����ꏊ</param>
	void LoadMaterialTextures(aiMaterial* material, aiTextureType type, AnimationMesh& model);

	//void MeshAssignment(const aiMesh* mesh, const aiScene* scene);



};