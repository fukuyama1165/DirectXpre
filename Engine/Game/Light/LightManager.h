#pragma once

/**
 * @file LightManager.h
 * @brief 全てのライトを管理
 * @author フクヤマ
 * @date 2023_12/29
 */
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

	
	std::vector<LightGroup> lightGroups_;
	

private:

	LightManager() = default;
	~LightManager();

	LightManager(const LightManager&) = delete;
	LightManager& operator=(const LightManager&) = delete;

};


