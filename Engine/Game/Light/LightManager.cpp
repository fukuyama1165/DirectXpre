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

	for (DirectionalLight d : directionalLights_)
	{
		d.Update();
	}

}

void LightManager::CreateLightGroup()
{

	LightGroup newLight;

	newLight.Init();

	lightGroups_.emplace_back(newLight);

}

void LightManager::CreateDirectionalLight()
{

	DirectionalLight newLight;

	newLight.Init();

	directionalLights_.emplace_back(newLight);

}