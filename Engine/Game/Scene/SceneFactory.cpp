#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"

SceneFactory::SceneFactory()
{
}

SceneFactory::~SceneFactory()
{
}

std::unique_ptr<IScene> SceneFactory::CreateScene(const std::string& sceneName)
{

	std::unique_ptr<IScene> newScene = nullptr;

	if (sceneName == "TITLE")
	{
		newScene = std::make_unique<TitleScene>();
	}
	else if (sceneName == "GAMEPLAY")
	{
		newScene = std::make_unique<GameScene>();
	}

	return std::move(newScene);

}