#include "SceneChangeFactory.h"
#include "BasicSceneChange.h"

SceneChangeFactory::SceneChangeFactory()
{
}

SceneChangeFactory::~SceneChangeFactory()
{
}

std::unique_ptr<ISceneChange> SceneChangeFactory::CreateSceneChange(const std::string& sceneChangeName)
{

	std::unique_ptr<ISceneChange> newSceneChange = nullptr;

	if (sceneChangeName == "BASIC")
	{
		newSceneChange = std::make_unique<BasicSceneChange>();
		newSceneChange->Initialize();
	}

	if (sceneChangeName == "NONE")
	{
		newSceneChange = std::make_unique<BasicSceneChange>();
		newSceneChange->Initialize();
		newSceneChange->SetIsClose(true);
		newSceneChange->SetIsEnd(true);
	}

	return std::move(newSceneChange);

}