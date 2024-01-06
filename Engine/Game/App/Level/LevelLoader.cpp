#include "LevelLoader.h"
#include "Input.h"
#include "ModelManager.h"
#include <imgui.h>
#include "LightManager.h"

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

	levelObj.clear();
	wallObj_.clear();
	lightCount_ = 1;
	//使うのが0番のライトのまとまりなので
	LightManager::GetInstance()->lightGroups_[0].AllPointLightOff();
	std::unique_ptr<LevelData> levelData = JsonLevelLoader::LoadJsonFile(filename);

	for (auto& objData : levelData->objects_)
	{
		if (objData.name_.find("Wall") != std::string::npos)
		{
			std::unique_ptr<LevelWallObj> newWall = std::make_unique<LevelWallObj>();

			newWall->obj.obj_.pos_ = Vector3{ objData.pos_.x,objData.pos_.y ,objData.pos_.z };
			newWall->obj.obj_.Rotate_ = Vector3{ Util::AngleToRadian(objData.rot_.x),Util::AngleToRadian(objData.rot_.y) ,Util::AngleToRadian(objData.rot_.z) };
			newWall->obj.obj_.Scale_ = Vector3{ objData.scale_.x,objData.scale_.y ,objData.scale_.z };
			newWall->obj.obj_.matWorldGeneration();
			newWall->obj.Init();
			newWall->name = objData.name_;

			wallObj_.emplace_back(std::move(newWall));
		}
		else if (objData.fileName_.find("Light") != std::string::npos || objData.name_.find("Light") != std::string::npos)
		{
			LightManager::GetInstance()->lightGroups_[0].SetPointLightActive(lightCount_, true);
			LightManager::GetInstance()->lightGroups_[0].SetPointLightPos(lightCount_, Vector3{ objData.pos_.x,objData.pos_.y ,objData.pos_.z });
			LightManager::GetInstance()->lightGroups_[0].SetPointLightAtten(lightCount_, { 0.1f,0.1f,0.1f });
			lightCount_++;

			LevelObj newObject;
			newObject.obj.FBXInit();
			newObject.obj.pos_ = Vector3{ objData.pos_.x,objData.pos_.y ,objData.pos_.z };
			newObject.obj.Rotate_ = Vector3{ Util::AngleToRadian(objData.rot_.x),Util::AngleToRadian(objData.rot_.y) ,Util::AngleToRadian(objData.rot_.z) };
			newObject.obj.Scale_ = Vector3{ objData.scale_.x,objData.scale_.y ,objData.scale_.z };
			newObject.obj.matWorldGeneration();
			newObject.name = objData.name_;

			levelObj.emplace_back(newObject);
		}
		else
		{
			
			LevelObj newObject;
			newObject.obj.FBXInit();
			newObject.obj.pos_ = Vector3{ objData.pos_.x,objData.pos_.y ,objData.pos_.z };
			newObject.obj.Rotate_ = Vector3{ Util::AngleToRadian(objData.rot_.x),Util::AngleToRadian(objData.rot_.y) ,Util::AngleToRadian(objData.rot_.z) };
			newObject.obj.Scale_ = Vector3{ objData.scale_.x,objData.scale_.y ,objData.scale_.z };
			newObject.obj.matWorldGeneration();
			newObject.name = objData.name_;

			levelObj.emplace_back(newObject);
		}

	}
}

void LevelLoader::Initialize()
{

	ModelManager::GetInstance()->Load("testFBX", "gltf", "basketballmanBox", "basketballman2");
	ModelManager::GetInstance()->Load("testGLTFBall", "gltf", "whiteBall", "white1x1");
	ModelManager::GetInstance()->Load("testFBX", "gltf", "Ground", "Dirt", "jpg");
	ModelManager::GetInstance()->Load("testFBX", "gltf", "Building", "Biru2");
	ModelManager::GetInstance()->Load("testFBX", "gltf", "Wall", "wallstone", "jpg");
	ModelManager::GetInstance()->Load("light", "gltf", "lightObj", "gray2x2");

	levelModel_ = ModelManager::GetInstance()->SearchModelData("whiteBall");
	levelBallModel_ = ModelManager::GetInstance()->SearchModelData("whiteBall");
	levelGroundModel_ = ModelManager::GetInstance()->SearchModelData("Ground");
	levelBuildingModel_ = ModelManager::GetInstance()->SearchModelData("Building");
	levelWallModel_ = ModelManager::GetInstance()->SearchModelData("Wall");
	levelLightModel_ = ModelManager::GetInstance()->SearchModelData("lightObj");

	testObj_.FBXInit();

	testObj_.pos_={ 0,20,-200 };
}

void LevelLoader::Update()
{
	
	for (LevelObj a : levelObj)
	{
		a.obj.Update();

		
	}
	for (uint16_t a = 0; a < levelObj.size(); a++)
	{
		if (levelObj[a].name.find("Ground") != std::string::npos)
		{
			levelObj[a].obj.material_.material_.tile_ = { levelObj[a].obj.Scale_.x,levelObj[a].obj.Scale_.z };
		}
		else if (levelObj[a].name.find("Box") != std::string::npos)
		{
			levelObj[a].obj.material_.material_.tile_ = { 5,5 };
		}
	}
	

	for (uint16_t b = 0; b < wallObj_.size(); b++)
	{
		wallObj_[b]->obj.Update();
	}

#ifdef _DEBUG

	//ImGui::Begin("Test");

	//ImGui::Text("日本語テスト");

	///*ImGui::DragFloat2("tile2", test_, 0.1f, 1.0f, 1000.0f);
	//ImGui::DragFloat2("tile3", test2_, 0.1f, 1.0f, 10000.0f);
	//ImGui::DragFloat2("tile4", test3_, 0.1f, 1.0f, 10000.0f);
	//
	//ImGui::Text("tile:%0.2ff,%0.2ff", levelObj[0].obj.material_.material_.tile_.x, levelObj[0].obj.material_.material_.tile_.y);*/

	//ImGui::End();
	

	/*levelObj[0].obj.SetMaterialTiring({ test_[0],test_[1] });
	levelObj[1].obj.SetMaterialTiring({ test2_[0],test2_[1] });
	

	testObj_.SetMaterialTiring({ test3_[0],test3_[1] });*/
	//testObj_.SetMaterialTiring({ test3_[0],test3_[1] });


	testObj_.Update();
	//levelObj[0].obj.SetMaterialTiring({test_[0],test_[1]});

#endif


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
		else if (a.name.find("Light") != std::string::npos)
		{
			a.obj.FBXDraw(*levelLightModel_);
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

	testObj_.FBXDraw(*levelGroundModel_);
}

void LevelLoader::reloadLevel(const BYTE& CheckKey, std::string filename)
{
	if (Input::GetInstance()->TriggerKey(CheckKey))
	{
		LoadLevel(filename);
	}
}