#pragma once
#include "Model.h"
#include "FBXLoader.h"

class ModelManager
{
public:

	static ModelManager* GetInstance() {
		static ModelManager instance;
		if (instance.models_.empty())
		{
			instance.Load("testFBX", "gltf", "whiteBox", "white1x1");
		}

		return &instance;
	};
	
	/// <summary>
	/// ���f���̓o�^
	/// </summary>
	/// <param name="filename">�ǂݍ��ރt�@�C��</param>
	/// <param name="fileType">�ǂݍ��ރt�@�C���^�C�v</param>
	/// <param name="handle">�Ăяo���Ƃ��̃n���h��</param>
	/// <param name="materialName">�}�e���A�����ǂݍ��߂Ȃ��������p�̉摜�̖��O(gltf��ǂݍ��ނƂ��͕K�{�A�K�����f���Ɠ����ʒu�ɒu������!!!)</param>
	/// <param name="materialType">�摜�̃t�@�C���^�C�v</param>
	/// <returns>�n���h��</returns>
	std::string Load(std::string filename, std::string fileType, std::string handle = "", std::string materialName = "", std::string materialType = "png");
	
	/// <summary>
	/// �o�^���ꂽ���f����T��(�Ȃ���Δ����{�b�N�X�Ԃ�)
	/// </summary>
	/// <param name="handle">�o�^�����Ƃ��̃n���h��</param>
	/// <returns>���f���̃f�[�^</returns>
	AnimationModel* SearchModelData(std::string handle);

	/// <summary>
	/// �������łɓǂݍ���ł��܂������f���̓o�^
	/// </summary>
	/// <param name="handle">�Ăяo���p�̃n���h��</param>
	/// <param name="model">�������łɂ��郂�f��</param>
	void RegisterModel(std::string handle, std::shared_ptr<AnimationModel> model);

	/// <summary>
	/// �w�肵���o�^�ς݃��f���̓o�^����
	/// </summary>
	/// <param name="handle">�n���h��</param>
	void eraseModel(std::string handle);

private:
	ModelManager() = default;
	~ModelManager() {};

	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;


	//�o�^�p�̍\����
	struct ModelData
	{

		std::string fileName = "";
		std::string materialName = "";
		std::shared_ptr<AnimationModel> model;

	};


private:


	//���f���f�[�^�̘A�z�z��
	std::map<std::string, ModelData> models_;

};