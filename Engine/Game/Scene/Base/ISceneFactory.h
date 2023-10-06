#pragma once

#include "IScene.h"
#include <string>
#include <memory>

class ISceneFactory
{
public:
	virtual ~ISceneFactory() = default;

	//シーン生成
	virtual std::unique_ptr<IScene> CreateScene(const std::string& sceneName) = 0;

private:

};