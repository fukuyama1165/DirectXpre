#include "ParticleManager.h"
//#include <filesystem>
//#include <fstream>
//#include <fileapi.h>
#include "Util.h"

ParicleManager::ParicleManager()
{
}

ParicleManager::~ParicleManager()
{
}

bool ParicleManager::LoadParicle()
{
	//読み込んでるやつを全部消して特定フォルダの中身を入れる
	particleDatas.clear();

	fileNameBuff = DirectXpre::Util::FindFileNames("Resources\\ParticleData\\", ".Pcle", false);

	return true;

	////ファイルストリーム
	//std::ifstream file(fileName);

	//if (!file)
	//{
	//	loadErrorText_ = "can not find file";
	//	return false;
	//}

	////JSON文字列から解凍したデータ
	//nlohmann::json deserialized;

	////解凍
	//file >> deserialized;

	////正しいイベントファイルかチェック
	//if (!deserialized.is_object() || !deserialized.contains("name") || !deserialized["name"].is_string())
	//{
	//	loadErrorText_ = "Not the correct event file";
	//	return false;
	//}

	////"name"を文字列として取得
	//std::string name = deserialized["name"].get<std::string>();

	////正しいかどうかチェック
	//if (name.compare("event") != 0)
	//{
	//	loadErrorText_ = "Not event file";
	//	return false;
	//}

	////"events"の全オブジェクトを走査
	//for (nlohmann::json& events : deserialized["events"])
	//{

	//	bool result = true;

	//	result = EventScanning(events);

	//	if (!result)
	//	{
	//		seting_.clear();
	//		return false;
	//	}
	//}
}

