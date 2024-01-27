#pragma once
/**
 * @file SceneManager.h
 * @brief シーン管理クラス
 * @author フクヤマ
 * @date 2023_12/29
 */

#include "IScene.h"
#include "ISceneChange.h"
#include "ISceneFactory.h"
#include "ISceneChangeFactory.h"
#include <memory>

class SceneManager
{
public:

	static SceneManager* GetInstance();

	void Update();

	void Draw();

	//終了処理
	void Finalize();

	//シーンファクトリーセット
	void SetSceneFactory(std::unique_ptr<ISceneFactory> sceneFactory) { sceneFactory_ = std::move(sceneFactory); };

	void ChangeScene(const std::string& sceneName, const std::string& sceneChangeName = "BASIC");

private:

	SceneManager() = default;
	~SceneManager();

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

private:

	std::unique_ptr<IScene> nowScene_;

	std::unique_ptr<IScene> nextScene_ = nullptr;

	std::unique_ptr<ISceneChange> SceneChange_ = nullptr;

	std::unique_ptr<ISceneFactory> sceneFactory_ = nullptr;

	std::unique_ptr<ISceneChangeFactory> sceneChangeFactory_ = nullptr;

};