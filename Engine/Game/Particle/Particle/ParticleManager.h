#pragma once
#include "ParticleData.h"
#include <vector>
#include <string>
#include <map>
#include <json.hpp>

class ParticleManager
{
public:

	static ParticleManager* GetInstance();

	bool LoadParicle();

	std::vector<std::string> GetSavePatileDataName() { return particleNames_; };

	ParticleData GetParticleData(std::string handle);

	float GetParticleDataNum() { return (float)particleDatas_.size(); };

	bool ParticleSearch(std::string handle);

	int32_t ParticleNumSearch(std::string handle);

	std::string GetParticleName(int32_t index);

private:

	//シングルトン用
	ParticleManager() = default;
	~ParticleManager();

	ParticleManager(const ParticleManager&) = delete;
	ParticleManager& operator=(const ParticleManager&) = delete;

	//パーティクルデータの中身読み込むよう
	void ParicleDataScanning(nlohmann::json& paricle,int32_t count);

	//パーティクルのデータを保持用
	std::map<std::string, ParticleData> particleDatas_;

	//パーティクルの名前保持
	std::vector<std::string> particleNames_;

	//パーティクルのフォルダ内のパスを保持するためのバッファ
	std::vector<std::string> fileNameBuff_;

	//パスの位置
	std::string directoryPath = "Resources\\ParticleData\\";

	//拡張子
	std::string exceptExt = ".Pcle";

};

