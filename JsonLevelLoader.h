#pragma once

#include <string>
#include <vector>
#include "Vector3.h"
#include "Vector4.h"
#include "json.hpp"

struct LevelData
{
	//������Ă���R���C�_�[�̃f�[�^���i�[����Ƃ���
	struct ObjectCollider
	{

		std::string ColliderType="";
		Vector3 center;
		Vector3 size;

	};

	//�z�u�̃f�[�^
	struct ObjectTransformData
	{
		
		std::string fileName;

		Vector4 trans;

		Vector4 rot;

		Vector4 scale;

		ObjectCollider collider;

	};

	

	//�����ɔz�u�̃f�[�^���܂Ƃ߂Ĉ���ɂ��Ă�
	std::vector<ObjectTransformData> objects;
	std::vector<ObjectCollider> colliders;

};

//json�ǂݍ���(���x���G�f�B�^(blender)����o�Ă����f�[�^��ǂݍ���)
class JsonLevelLoader
{
public:
	
	//�ǂݍ��ލۂ̃t�@�C���p�X(�t�@�C���������Ŏw�肷�邽��)
	static const std::string defaultDataPath;

	//�t�@�C���g���q
	static const std::string extension;

private:


public:

	static LevelData* LoadJsonFile(const std::string& fileName);



private:

	//�I�u�W�F�N�g��{������
	static void objectScanning(LevelData* levelData, const nlohmann::json deserialized,nlohmann::json& object);

};

