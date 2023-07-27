#pragma once

#include "ISceneFactory.h"

//継承している理由
//作っているものが変わった時にシーンが増えた時に新たに処理が書けるし
//変えるのが簡単になるため

class SceneFactory:public ISceneFactory
{
public:
	SceneFactory();
	~SceneFactory();

	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	/// <returns>生成したシーン</returns>
	std::unique_ptr<IScene> CreateScene(const std::string& sceneName)override;

private:

};

