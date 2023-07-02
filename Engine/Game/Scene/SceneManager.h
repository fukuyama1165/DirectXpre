#pragma once
#include "IScene.h"
#include <memory>

class SceneManager
{
public:

	static SceneManager* GetInstance();

	
	void SetNextScene(std::unique_ptr<IScene> nextScene) { nextScene_ = std::move(nextScene); };

	void Update();

	void Draw();

	//I—¹ˆ—
	void Finalize();

private:

	SceneManager() = default;
	~SceneManager();

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

private:

	std::unique_ptr<IScene> nowScene_;

	std::unique_ptr<IScene> nextScene_ = nullptr;

};