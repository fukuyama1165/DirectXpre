#include "FBXLoader.h"

AnimationModel::~AnimationModel()
{
}

bool AnimationModel::Load(std::string filename, std::string fileType)
{
	//�C���X�^���X���擾
	Assimp::Importer importer;

	//�t�@�C���p�X�������`
	std::string baseDirectory = "Resources\\";
	std::string extend = "." + fileType;
	filename = baseDirectory + filename + "\\" + filename + extend;

	//�w�肵���t�@�C���̓ǂݍ��݂炵��
	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_CalcTangentSpace |//�C���|�[�g�������b�V���̃^���W�F���g�Ƃ����v�Z���Ă����炵��
		aiProcess_Triangulate |//�O�p�ʉ����Ă����
		aiProcess_JoinIdenticalVertices |//�C���|�[�g�������b�V���̓���̒��_�f�[�^�Z�b�g�����ʁA��������
		aiProcess_SortByPType |//2�ȏ�̃v���~�e�B�u�^�C�v�������b�V�����ώ��ȃT�u���b�V���ɕ�������
		aiProcess_MakeLeftHanded |//������W�n��
		aiProcess_FlipUVs |//UV���W����������_�ɂ���
		aiProcess_FlipWindingOrder//CCW�J�����O�ɓK�������v���̖ʂ̏����ɂ���
	);

	//�ǂݍ��݂Ɏ��s������G���[���͂��Ď~�܂�?
	if (nullptr != scene)
	{
		//DoTheImportThing(importer.GetErrorString());
		return false;
	}

	//���g�ɐG���炵��

	//�{�[���̏��̕����炵��
	//aiNode* a = scene->mRootNode;

	//�f�[�^�̂�݂���


	return false;
}