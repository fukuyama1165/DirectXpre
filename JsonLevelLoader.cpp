#include "JsonLevelLoader.h"
#include <fstream>
#include <cassert>

const std::string JsonLevelLoader::defaultDataPath = "Resources/json/";
const std::string JsonLevelLoader::extension = ".json";

LevelData* JsonLevelLoader::LoadJsonFile(const std::string& fileName)
{
	//������p�ӂ��Ă����p�X���������Ċ��S�ɒʂ�p�X�ɂ���
	const std::string fullPath = defaultDataPath + fileName + extension;

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

		levelData->objects.emplace_back(LevelData::ObjectTransformData{});

		//���ǉ������v�f�̎Q�Ƃ𓾂�
		LevelData::ObjectTransformData& objectData = levelData->objects.back();

		//�t�@�C����������Ȃ�����
		if (object.contains("file_name"))
		{
			objectData.fileName = object["file_name"];
		}

		//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
		nlohmann::json& transform = object["transform"];

		//���s�ړ�
		objectData.trans.x = (float)transform["translation"][1];
		objectData.trans.y = (float)transform["translation"][2];
		objectData.trans.z = -(float)transform["translation"][0];
		objectData.trans.w = 1.0f;


		//��]�p
		objectData.rot.x = -(float)transform["rotation"][1];
		objectData.rot.y = -(float)transform["rotation"][2];
		objectData.rot.z = (float)transform["rotation"][0];
		objectData.rot.w = 0.0f;


		//�X�P�[�����O
		objectData.scale.x = (float)transform["scaling"][1];
		objectData.scale.y = (float)transform["scaling"][2];
		objectData.scale.z = (float)transform["scaling"][0];
		objectData.scale.w = 0.0f;

		//�t�@�C����������Ȃ�����
		if (object.contains("collider"))
		{
			//�R���C�_�[
			levelData->colliders.emplace_back(LevelData::ObjectCollider{});
			//���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::ObjectCollider& objectCollider = levelData->colliders.back();

			nlohmann::json& collider = object["collider"];

			//���Ԃ�^�C�v
			objectCollider.ColliderType = collider["collider"];

			//����
			objectCollider.center.x = (float)collider["collider_center"][1];
			objectCollider.center.y = (float)collider["collider_center"][2];
			objectCollider.center.z = -(float)collider["collider_center"][0];

			objectCollider.size.x = (float)collider["collider_size"][1];
			objectCollider.size.y = (float)collider["collider_size"][2];
			objectCollider.size.z = (float)collider["collider_size"][0];
		}
	}

	if (object.contains("children"))
	{
		nlohmann::json& children = object["children"];
		for (int i = 0; i < children.size();i++)
		{
			objectScanning(levelData, deserialized, children[i]);
		}
		
	}

	

}