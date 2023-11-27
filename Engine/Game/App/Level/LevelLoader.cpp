#include "LevelLoader.h"
#include "Input.h"
#include "ModelManager.h"

LevelLoader::~LevelLoader()
{

}

LevelLoader* LevelLoader::GetInstance()
{
	static LevelLoader instance;

	if (instance.levelModel_ == nullptr)
	{
		instance.Initialize();
	}

	return &instance;
}

void LevelLoader::LoadLevel(std::string filename)
{
	std::unique_ptr<LevelData> levelData = JsonLevelLoader::LoadJsonFile(filename);

	for (auto& objData : levelData->objects_)
	{
		if (objData.name_.find("Wall") == std::string::npos)
		{
			LevelObj newObject;
			newObject.obj.FBXInit();
			newObject.obj.Trans_ = Vector3{ objData.trans_.x,objData.trans_.y ,objData.trans_.z };
			newObject.obj.Rotate_ = Vector3{ Util::AngleToRadian(objData.rot_.x),Util::AngleToRadian(objData.rot_.y) ,Util::AngleToRadian(objData.rot_.z) };
			newObject.obj.Scale_ = Vector3{ objData.scale_.x,objData.scale_.y ,objData.scale_.z };
			newObject.obj.matWorldGeneration();
			newObject.name = objData.name_;

			levelObj.emplace_back(newObject);
		}
		else
		{
			std::unique_ptr<LevelWallObj> newWall = std::make_unique<LevelWallObj>();

			newWall->obj.obj_.Trans_ = Vector3{ objData.trans_.x,objData.trans_.y ,objData.trans_.z };
			newWall->obj.obj_.Rotate_ = Vector3{ Util::AngleToRadian(objData.rot_.x),Util::AngleToRadian(objData.rot_.y) ,Util::AngleToRadian(objData.rot_.z) };
			newWall->obj.obj_.Scale_ = Vector3{ objData.scale_.x,objData.scale_.y ,objData.scale_.z };
			newWall->obj.obj_.matWorldGeneration();
			newWall->obj.Init();
			newWall->name = objData.name_;

			wallObj_.emplace_back(std::move(newWall));
		}

	}
}

void LevelLoader::Initialize()
{

	ModelManager::GetInstance()->Load("testFBX", "gltf", "basketballmanBox", "basketballman2");
	ModelManager::GetInstance()->Load("testGLTFBall", "gltf", "whiteBall", "white1x1");
	ModelManager::GetInstance()->Load("testFBX", "gltf", "Ground", "Dirt", "jpg");
	ModelManager::GetInstance()->Load("testFBX", "gltf", "Building", "Biru2");
	ModelManager::GetInstance()->Load("testFBX", "gltf", "Wall", "gray2x2");

	levelModel_ = ModelManager::GetInstance()->SearchModelData("whiteBall");
	levelBallModel_ = ModelManager::GetInstance()->SearchModelData("whiteBall");
	levelGroundModel_ = ModelManager::GetInstance()->SearchModelData("Ground");
	levelBuildingModel_ = ModelManager::GetInstance()->SearchModelData("Building");
	levelWallModel_ = ModelManager::GetInstance()->SearchModelData("Wall");
}

void LevelLoader::Update()
{
	for (LevelObj a : levelObj)
	{
		a.obj.Update();
	}

	for (uint16_t b = 0; b < wallObj_.size(); b++)
	{
		wallObj_[b]->obj.Update();
	}
}

void LevelLoader::Draw()
{
	for (LevelObj a : levelObj)
	{
		if (a.name.find("test") != std::string::npos)
		{
			a.obj.FBXDraw(*levelModel_);
		}
		else if (a.name.find("Box") != std::string::npos)
		{
			a.obj.FBXDraw(*levelWallModel_);
		}
		else if (a.name.find("ball") != std::string::npos)
		{
			a.obj.FBXDraw(*levelBallModel_);
		}
		else if (a.name.find("Ground") != std::string::npos)
		{
			a.obj.FBXDraw(*levelGroundModel_);
		}
		else if (a.name.find("building") != std::string::npos)
		{
			a.obj.FBXDraw(*levelBuildingModel_);
		}
		else
		{
			a.obj.FBXDraw(*levelModel_);
		}
	}

	for (uint16_t b = 0; b < wallObj_.size(); b++)
	{
		wallObj_[b]->obj.Draw(levelWallModel_);
	}
}

void LevelLoader::reloadLevel(const BYTE& CheckKey, std::string filename)
{
	if (Input::GetInstance()->TriggerKey(CheckKey))
	{
		levelObj.clear();
		wallObj_.clear();
		std::unique_ptr<LevelData> levelData = JsonLevelLoader::LoadJsonFile(filename);

		for (auto& objData : levelData->objects_)
		{
			if (objData.name_.find("Wall") == std::string::npos)
			{
				LevelObj newObject;
				newObject.obj.FBXInit();
				newObject.obj.Trans_ = Vector3{ objData.trans_.x,objData.trans_.y ,objData.trans_.z };
				newObject.obj.Rotate_ = Vector3{ Util::AngleToRadian(objData.rot_.x),Util::AngleToRadian(objData.rot_.y) ,Util::AngleToRadian(objData.rot_.z) };
				newObject.obj.Scale_ = Vector3{ objData.scale_.x,objData.scale_.y ,objData.scale_.z };
				newObject.obj.matWorldGeneration();
				newObject.name = objData.name_;

				levelObj.emplace_back(newObject);
			}
			else
			{
				std::unique_ptr<LevelWallObj> newWall = std::make_unique<LevelWallObj>();

				newWall->obj.obj_.Trans_ = Vector3{ objData.trans_.x,objData.trans_.y ,objData.trans_.z };
				newWall->obj.obj_.Rotate_ = Vector3{ Util::AngleToRadian(objData.rot_.x),Util::AngleToRadian(objData.rot_.y) ,Util::AngleToRadian(objData.rot_.z) };
				newWall->obj.obj_.Scale_ = Vector3{ objData.scale_.x,objData.scale_.y ,objData.scale_.z };
				newWall->obj.obj_.matWorldGeneration();
				newWall->name = objData.name_;
				newWall->obj.Init();

				wallObj_.emplace_back(std::move(newWall));
			}

		}
	}
}