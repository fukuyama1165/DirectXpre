#include "LightManager.h"


LightManager::~LightManager()
{
}

LightManager* LightManager::GetInstance()
{
	static LightManager instance;
	return &instance;
}

void LightManager::ALLLightUpdate()
{
	for (LightGroup g : lightGroups_)
	{
		g.Update();
	}

}

void LightManager::CreateLightGroup()
{

	LightGroup newLight;

	newLight.Init();

	lightGroups_.emplace_back(newLight);

}

