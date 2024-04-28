#include "ParticleManager.h"
#include "Util.h"
#include <fstream>

ParticleManager::~ParticleManager()
{
}

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;

	return &instance;
}

bool ParticleManager::LoadParicle()
{
	//読み込んでるやつを全部消して特定フォルダの中身を入れる
	particleDatas_.clear();

	particleNames_.clear();

	fileNameBuff_ = DirectXpre::Util::FindFileNames(directoryPath, exceptExt, false);

	//ファイルストリーム
	for (int32_t i = 0; i < fileNameBuff_.size(); i++)
	{
		std::string fileName = directoryPath + fileNameBuff_[i] + exceptExt;
		std::ifstream file(fileName);

		if (!file)
		{
			return false;
		}

		//JSON文字列から解凍したデータ
		nlohmann::json deserialized;

		//解凍
		file >> deserialized;

		//正しいイベントファイルかチェック
		if (!deserialized.is_object() || !deserialized.contains("name") || !deserialized["name"].is_string())
		{
			return false;
		}

		//"name"を文字列として取得
		std::string name = deserialized["name"].get<std::string>();

		//正しいかどうかチェック
		if (name.compare("Particle") != 0)
		{
			return false;
		}

		//"Particle"を走査
		ParicleDataScanning(deserialized["ParticleParameters"],i);
	}

	return true;
}

void ParticleManager::ParicleDataScanning(nlohmann::json& paricle, int32_t count)
{
	//ParticleParametersがなければ止める
	//assert(paricle.contains("ParticleParameters"));
	
	ParticleData particleData;

	//設定のパラメータ読み込み

	//エミッターの活動時間
	particleData.ActiveTime = (float)paricle["ActiveTime"];
	//名前
	particleData.Name = paricle["Name"];
	//パーティクルの行動時間
	particleData.actionTime = (float)paricle["actionTime"];
	//エミッターのパーティクルを出す間隔
	particleData.ct = (float)paricle["ct"];

	//パーティクルの生存時間
	particleData.liveTime = (float)paricle["liveTime"];

	//出てくる幅のランダム範囲X
	particleData.randRengeX.x = (float)paricle["randRengeX"][0];
	particleData.randRengeX.y = (float)paricle["randRengeX"][1];

	//出てくる幅のランダム範囲Y
	particleData.randRengeY.x = (float)paricle["randRengeY"][0];
	particleData.randRengeY.y = (float)paricle["randRengeY"][1];

	//出てくる幅のランダム範囲Z
	particleData.randRengeZ.x = (float)paricle["randRengeZ"][0];
	particleData.randRengeZ.y = (float)paricle["randRengeZ"][1];

	//開始スケール
	particleData.startScale.x = (float)paricle["startScale"][0];
	particleData.startScale.y = (float)paricle["startScale"][1];
	particleData.startScale.z = (float)paricle["startScale"][2];

	//終了時のスケール
	particleData.endScale.x = (float)paricle["endScale"][0];
	particleData.endScale.y = (float)paricle["endScale"][1];
	particleData.endScale.z = (float)paricle["endScale"][2];
	
	std::string handle = particleData.Name;

	if (handle == "")
	{
		handle = "particleData_" + std::to_string(count);
	}

	//名前で登録
	particleDatas_[handle] = particleData;
	particleNames_.push_back(handle);

}

ParticleData ParticleManager::GetParticleData(std::string handle)
{
	if (particleDatas_.find(handle) == particleDatas_.end()) {
		return ParticleData();
	} return particleDatas_[handle];
};

bool ParticleManager::ParticleSearch(std::string handle)
{
	if (particleDatas_.find(handle) == particleDatas_.end()) {
		return false;
	}
	return true;
}

std::string ParticleManager::GetParticleName(int32_t index)
{
	if (particleNames_.empty())
	{
		return "BASIC";
	}

	if (particleNames_.size() - 1 < index)
	{
		
		return particleNames_[particleNames_.size() - 1];
	}
	else if (0 > index)
	{
		return particleNames_[0];
	}

	return particleNames_[index];
}

int32_t ParticleManager::ParticleNumSearch(std::string handle)
{
	for (int32_t i = 0; i < particleNames_.size(); i++)
	{
		if (particleNames_[i] == handle)
		{
			return i;
		}
	}

	return -1;
}
