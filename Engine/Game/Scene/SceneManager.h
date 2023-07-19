#pragma once
#include "IScene.h"
#include "ISceneFactory.h"
#include <memory>

class SceneManager
{
public:

	static SceneManager* GetInstance();

	
	void SetNextScene(std::unique_ptr<IScene> nextScene) { nextScene_ = std::move(nextScene); };

	void Update();

	void Draw();

	//終了処理
	void Finalize();

	//シーンファクトリーセット
	void SetSceneFactory(std::unique_ptr<ISceneFactory> sceneFactory) { sceneFactory_ = std::move(sceneFactory); };

	void ChangeScene(const std::string& sceneName);

private:

	SceneManager() = default;
	~SceneManager();

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

private:

	std::unique_ptr<IScene> nowScene_;

	std::unique_ptr<IScene> nextScene_ = nullptr;

	std::unique_ptr<ISceneFactory> sceneFactory_ = nullptr;

};