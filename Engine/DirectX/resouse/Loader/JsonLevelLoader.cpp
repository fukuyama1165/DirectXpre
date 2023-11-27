#include "JsonLevelLoader.h"
#include <fstream>
#include <cassert>

const std::string JsonLevelLoader::SDefaultDataPath_ = "Resources/json/";
const std::string JsonLevelLoader::SExtension_ = ".json";

std::unique_ptr<LevelData> JsonLevelLoader::LoadJsonFile(const std::string& fileName)
{
	//元から用意していたパスをくっつけて完全に通るパスにする
	const std::string fullPath = SDefaultDataPath_ + fileName + SExtension_;

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
	std::unique_ptr<LevelData> levelData = std::make_unique<LevelData>();
	//"object"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		JsonLevelLoader::objectScanning(levelData.get(), deserialized, object);
	}

	return std::move(levelData);

}

void JsonLevelLoader::objectScanning(LevelData* levelData, nlohmann::json deserialized, nlohmann::json& object)
{


	//typeがなければ止める
	assert(object.contains("type"));
	//タイプを取得
	std::string type = object["type"].get<std::string>();



	//MESHなら
	if (type.compare("MESH") == 0)
	{

		levelData->objects_.emplace_back(LevelData::ObjectTransformData{});

		//今追加した要素の参照を得る
		LevelData::ObjectTransformData& objectData = levelData->objects_.back();

		//ファイル名があるなら入れる
		if (object.contains("file_name"))
		{
			objectData.fileName_ = object["file_name"];
		}

		objectData.name_ = object["name"].get<std::string>();

		//トランスフォームのパラメータ読み込み
		nlohmann::json& transform = object["transform"];

		//平行移動
		objectData.trans_.x = (float)transform["translation"][1];
		objectData.trans_.y = (float)transform["translation"][2];
		objectData.trans_.z = -(float)transform["translation"][0];
		objectData.trans_.w = 1.0f;


		//回転角
		objectData.rot_.x = -(float)transform["rotation"][1];
		objectData.rot_.y = -(float)transform["rotation"][2];
		objectData.rot_.z = (float)transform["rotation"][0];
		objectData.rot_.w = 0.0f;


		//スケーリング
		objectData.scale_.x = (float)transform["scaling"][1];
		objectData.scale_.y = (float)transform["scaling"][2];
		objectData.scale_.z = (float)transform["scaling"][0];
		objectData.scale_.w = 0.0f;

		//ファイル名があるなら入れる
		if (object.contains("collider"))
		{
			//コライダー
			levelData->colliders_.emplace_back(LevelData::ObjectCollider{});
			//今追加した要素の参照を得る
			LevelData::ObjectCollider& objectCollider = levelData->colliders_.back();

			nlohmann::json& collider = object["collider"];

			//たぶんタイプ
			objectCollider.ColliderType_ = collider["collider"];

			//中央
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
		for (uint16_t i = 0; i < children.size(); i++)
		{
			objectScanning(levelData, deserialized, children[i]);
		}

	}



}