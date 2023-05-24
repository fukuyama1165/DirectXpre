#pragma once

#include <string>
#include <vector>
#include "Vector3.h"
#include "Vector4.h"
#include "json.hpp"

struct LevelData
{
	//もらってくるコライダーのデータを格納するところ
	struct ObjectCollider
	{

		std::string ColliderType="";
		Vector3 center;
		Vector3 size;

	};

	//配置のデータ
	struct ObjectTransformData
	{
		
		std::string fileName;

		Vector4 trans;

		Vector4 rot;

		Vector4 scale;

		ObjectCollider collider;

	};

	

	//ここに配置のデータをまとめて一個分にしてる
	std::vector<ObjectTransformData> objects;
	std::vector<ObjectCollider> colliders;

};

//json読み込み(レベルエディタ(blender)から出てきたデータを読み込む)
class JsonLevelLoader
{
public:
	
	//読み込む際のファイルパス(ファイル名だけで指定するため)
	static const std::string defaultDataPath;

	//ファイル拡張子
	static const std::string extension;

private:


public:

	static LevelData* LoadJsonFile(const std::string& fileName);



private:

	//オブジェクトを捜査する
	static void objectScanning(LevelData* levelData, const nlohmann::json deserialized,nlohmann::json& object);

};

