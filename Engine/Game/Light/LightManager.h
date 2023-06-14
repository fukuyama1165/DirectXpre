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

	//�X�V
	void ALLLightUpdate();

	//�ŏI�I�ɂ͖��O�ŊǗ�������
	//LightGroup��ǉ�����
	void CreateLightGroup(/*const std::string& name*/);

	//DirectionalLight��ǉ�����
	void CreateDirectionalLight(/*const std::string& name*/);

	//LightGroup GetLightGroups(const std::string& name);

	//�����ɒǉ�����̂ł͂Ȃ�Create�֐��Œǉ����邱��
	std::vector<LightGroup> lightGroups_;
	//�����ɒǉ�����̂ł͂Ȃ�Create�֐��Œǉ����邱��
	std::vector<DirectionalLight> directionalLights_;

private:

	LightManager() = default;
	~LightManager();

	LightManager(const LightManager&) = delete;
	LightManager& operator=(const LightManager&) = delete;

};


