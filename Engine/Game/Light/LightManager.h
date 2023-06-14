#pragma once
#include "DirectionalLight.h"
#include "LightGroup.h"
#include <vector>
#include <string>

class LightManager
{
public:

	static LightManager* GetInstance();

	/*struct LightG
	{
		std::string nameId;

		LightGroup lightGroup;
	};

	struct DirectionalL
	{
		std::string nameId;

		DirectionalLight directionalLight;
	};*/

	//更新
	void ALLLightUpdate();

	//最終的には名前で管理したい
	//LightGroupを追加する
	void CreateLightGroup(/*const std::string& name*/);

	//DirectionalLightを追加する
	void CreateDirectionalLight(/*const std::string& name*/);

	//LightGroup GetLightGroups(const std::string& name);

	//ここに追加するのではなくCreate関数で追加すること
	std::vector<LightGroup> lightGroups_;
	//ここに追加するのではなくCreate関数で追加すること
	std::vector<DirectionalLight> directionalLights_;

private:

	LightManager() = default;
	~LightManager();

	LightManager(const LightManager&) = delete;
	LightManager& operator=(const LightManager&) = delete;

};


