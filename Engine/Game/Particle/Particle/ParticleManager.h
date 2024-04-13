#pragma once
#include "ParticleData.h"
#include <vector>
#include <string>
#include <map>

class ParicleManager
{
public:
	ParicleManager();
	~ParicleManager();

	bool LoadParicle();

private:

	std::map<std::string, ParticleData> particleDatas;

	std::vector<std::string> fileNameBuff;

};

