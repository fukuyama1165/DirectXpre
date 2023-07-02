#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Vector3.h"
#include "Vector4.h"
#include "json.hpp"

struct LevelData
{
	//������Ă���R���C�_�[�̃f�[�^���i�[����Ƃ���
	struct ObjectCollider
	{

		std::string ColliderType_ ="";
		Vector3 center_;
		Vector3 size_;

	};

	//�z�u�̃f�[�^
	struct ObjectTransformData
	{
		
		std::string fileName_;
		std::string name_;

		Vector4 trans_;

		Vector4 rot_;

		Vector4 scale_;

		ObjectCollider collider_;

	};

	

	//�����ɔz�u�̃f�[�^���܂Ƃ߂Ĉ���ɂ��Ă�
	std::vector<ObjectTransformData> objects_;
	std::vector<ObjectCollider> colliders_;

};

//json�ǂݍ���(���x���G�f�B�^(blender)����o�Ă����f�[�^��ǂݍ���)
class JsonLevelLoader
{
public:
	
	//�ǂݍ��ލۂ̃t�@�C���p�X(�t�@�C���������Ŏw�肷�邽��)
	static const std::string SDefaultDataPath_;

	//�t�@�C���g���q
	static const std::string SExtension_;

private:


public:

	static std::unique_ptr<LevelData> LoadJsonFile(const std::string& fileName);



private:

	//�I�u�W�F�N�g��{������
	static void objectScanning(LevelData* levelData, const nlohmann::json deserialized,nlohmann::json& object);

};

