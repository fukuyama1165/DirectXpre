#pragma once
#include "ISceneChange.h"
#include <string>
#include <memory>

class ISceneChangeFactory
{
public:
	virtual ~ISceneChangeFactory() = default;

	//シーン生成
	virtual std::unique_ptr<ISceneChange> CreateSceneChange(const std::string& sceneChangeName) = 0;

private:

};