#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Vector3.h"
#include "Vector4.h"
#include "json.hpp"

struct LevelData
{
	//もらってくるコライダーのデータを格納するところ
	struct ObjectCollider
	{

		std::string ColliderType_ ="";
		Vector3 center_;
		Vector3 size_;

	};

	//配置のデータ
	struct ObjectTransformData
	{
		
		std::string fileName_;
		std::string name_;

		Vector4 trans_;

		Vector4 rot_;

		Vector4 scale_;

		ObjectCollider collider_;

	};

	

	//ここに配置のデータをまとめて一個分にしてる
	std::vector<ObjectTransformData> objects_;
	std::vector<ObjectCollider> colliders_;

};

//json読み込み(レベルエディタ(blender)から出てきたデータを読み込む)
class JsonLevelLoader
{
public:
	
	//読み込む際のファイルパス(ファイル名だけで指定するため)
	static const std::string SDefaultDataPath_;

	//ファイル拡張子
	static const std::string SExtension_;

private:


public:

	static std::unique_ptr<LevelData> LoadJsonFile(const std::string& fileName);



private:

	//オブジェクトを捜査する
	static void objectScanning(LevelData* levelData, const nlohmann::json deserialized,nlohmann::json& object);

};

