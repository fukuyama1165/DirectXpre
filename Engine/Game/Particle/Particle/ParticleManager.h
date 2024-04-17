#pragma once
#include "ParticleData.h"
#include <vector>
#include <string>
#include <map>
#include <json.hpp>

class ParicleManager
{
public:

	static ParicleManager* GetInstance();

	bool LoadParicle();

private:

	//シングルトン用
	ParicleManager() = default;
	~ParicleManager();

	ParicleManager(const ParicleManager&) = delete;
	ParicleManager& operator=(const ParicleManager&) = delete;

	//パーティクルデータの中身読み込むよう
	void ParicleDataScanning(nlohmann::json& paricle);

	//パーティクルのデータを保持用
	std::map<std::string, ParticleData> particleDatas_;

	//パーティクルの名前保持
	std::vector<std::string> particleNames_;

	//パーティクルのフォルダ内のパスを保持するためのバッファ
	std::vector<std::string> fileNameBuff_;

};

