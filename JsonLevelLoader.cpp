#include "JsonLevelLoader.h"
#include <fstream>
#include <cassert>

const std::string JsonLevelLoader::defaultDataPath = "Resources/json/";
const std::string JsonLevelLoader::extension = ".json";

LevelData* JsonLevelLoader::LoadJsonFile(const std::string& fileName)
{
	//元から用意していたパスをくっつけて完全に通るパスにする
	const std::string fullPath = defaultDataPath + fileName + extension;

	//ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullPath);

	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();

	//正しいかどうかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();
	//"object"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		
		JsonLevelLoader::objectScanning(levelData, deserialized, object);
	}

	return levelData;

}

void JsonLevelLoader::objectScanning(LevelData* levelData, nlohmann::json deserialized,nlohmann::json& object)
{

	
	//typeがなければ止める
	assert(object.contains("type"));
	//タイプを取得
	std::string type = object["type"].get<std::string>();

	

	//MESHなら
	if (type.compare("MESH") == 0)
	{

		levelData->objects.emplace_back(LevelData::ObjectTransformData{});

		//今追加した要素の参照を得る
		LevelData::ObjectTransformData& objectData = levelData->objects.back();

		//ファイル名があるなら入れる
		if (object.contains("file_name"))
		{
			objectData.fileName = object["file_name"];
		}

		//トランスフォームのパラメータ読み込み
		nlohmann::json& transform = object["transform"];

		//平行移動
		objectData.trans.x = (float)transform["translation"][1];
		objectData.trans.y = (float)transform["translation"][2];
		objectData.trans.z = -(float)transform["translation"][0];
		objectData.trans.w = 1.0f;


		//回転角
		objectData.rot.x = -(float)transform["rotation"][1];
		objectData.rot.y = -(float)transform["rotation"][2];
		objectData.rot.z = (float)transform["rotation"][0];
		objectData.rot.w = 0.0f;


		//スケーリング
		objectData.scale.x = (float)transform["scaling"][1];
		objectData.scale.y = (float)transform["scaling"][2];
		objectData.scale.z = (float)transform["scaling"][0];
		objectData.scale.w = 0.0f;

		//ファイル名があるなら入れる
		if (object.contains("collider"))
		{
			//コライダー
			levelData->colliders.emplace_back(LevelData::ObjectCollider{});
			//今追加した要素の参照を得る
			LevelData::ObjectCollider& objectCollider = levelData->colliders.back();

			nlohmann::json& collider = object["collider"];

			//たぶんタイプ
			objectCollider.ColliderType = collider["collider"];

			//中央
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