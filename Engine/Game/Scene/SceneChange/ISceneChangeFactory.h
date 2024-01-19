#pragma once

/**
 * @file ISceneChange.h
 * @brief シーンチェンジ工場基底クラス
 * @author フクヤマ
 * @date 2023_12/29
 */

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