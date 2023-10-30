#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "EventEditorScene.h"

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
	else if (sceneName == "EventEditor")
	{
		newScene = std::make_unique<EventEditorScene>();
	}

	return std::move(newScene);

}