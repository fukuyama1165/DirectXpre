#pragma once

/**
 * @file SceneChangeFactory.h
 * @brief シーンチェンジ工場クラス
 * @author フクヤマ
 * @date 2023_12/29
 */

#include "ISceneChangeFactory.h"

//継承している理由
//作っているものが変わった時にシーンが増えた時に新たに処理が書けるし
//変えるのが簡単になるため

class SceneChangeFactory :public ISceneChangeFactory
{
public:
	SceneChangeFactory();
	~SceneChangeFactory();

	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	/// <returns>生成したシーン</returns>
	std::unique_ptr<ISceneChange> CreateSceneChange(const std::string& sceneChangeName)override;

private:

};