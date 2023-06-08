#include "JsonLevelLoader.h"
#include <fstream>
#include <cassert>

const std::string JsonLevelLoader::SDefaultDataPath_ = "Resources/json/";
const std::string JsonLevelLoader::SExtension_ = ".json";

LevelData* JsonLevelLoader::LoadJsonFile(const std::string& fileName)
{
	//������p�ӂ��Ă����p�X���������Ċ��S�ɒʂ�p�X�ɂ���
	const std::string fullPath =SDefaultDataPath_ + fileName + SExtension_;

	//�t�@�C���X�g���[��
	std::ifstream file;

	// �t�@�C�����J��
	file.open(fullPath);

	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	//��
	file >> deserialized;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();

	//���������ǂ����`�F�b�N
	assert(name.compare("scene") == 0);

	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	LevelData* levelData = new LevelData();
	//"object"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"])
	{
		
		JsonLevelLoader::objectScanning(levelData, deserialized, object);
	}

	return levelData;

}

void JsonLevelLoader::objectScanning(LevelData* levelData, nlohmann::json deserialized,nlohmann::json& object)
{

	
	//type���Ȃ���Ύ~�߂�
	assert(object.contains("type"));
	//�^�C�v���擾
	std::string type = object["type"].get<std::string>();

	

	//MESH�Ȃ�
	if (type.compare("MESH") == 0)
	{

		levelData->objects_.emplace_back(LevelData::ObjectTransformData{});

		//���ǉ������v�f�̎Q�Ƃ𓾂�
		LevelData::ObjectTransformData& objectData = levelData->objects_.back();

		//�t�@�C����������Ȃ�����
		if (object.contains("file_name"))
		{
			objectData.fileName_ = object["file_name"];
		}

		//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
		nlohmann::json& transform = object["transform"];

		//���s�ړ�
		objectData.trans_.x = (float)transform["translation"][1];
		objectData.trans_.y = (float)transform["translation"][2];
		objectData.trans_.z = -(float)transform["translation"][0];
		objectData.trans_.w = 1.0f;


		//��]�p
		objectData.rot_.x = -(float)transform["rotation"][1];
		objectData.rot_.y = -(float)transform["rotation"][2];
		objectData.rot_.z = (float)transform["rotation"][0];
		objectData.rot_.w = 0.0f;


		//�X�P�[�����O
		objectData.scale_.x = (float)transform["scaling"][1];
		objectData.scale_.y = (float)transform["scaling"][2];
		objectData.scale_.z = (float)transform["scaling"][0];
		objectData.scale_.w = 0.0f;

		//�t�@�C����������Ȃ�����
		if (object.contains("collider"))
		{
			//�R���C�_�[
			levelData->colliders_.emplace_back(LevelData::ObjectCollider{});
			//���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::ObjectCollider& objectCollider = levelData->colliders_.back();

			nlohmann::json& collider = object["collider"];

			//���Ԃ�^�C�v
			objectCollider.ColliderType_ = collider["collider"];

			//����
			objectCollider.center_.x = (float)collider["collider_center"][1];
			objectCollider.center_.y = (float)collider["collider_center"][2];
			objectCollider.center_.z = -(float)collider["collider_center"][0];

			objectCollider.size_.x = (float)collider["collider_size"][1];
			objectCollider.size_.y = (float)collider["collider_size"][2];
			objectCollider.size_.z = (float)collider["collider_size"][0];
		}
	}

	if (object.contains("children"))
	{
		nlohmann::json& children = object["children"];
		for (uint16_t i = 0; i < children.size();i++)
		{
			objectScanning(levelData, deserialized, children[i]);
		}
		
	}

	

}