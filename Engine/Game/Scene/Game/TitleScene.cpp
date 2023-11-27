#include "TitleScene.h"
#include <imgui.h>
#include "Input.h"
#include "SceneManager.h"
#include "LightManager.h"
#include "Texture.h"
#include "EmitterManager.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{

	//ライトの生成
	lightManager_ = LightManager::GetInstance();

	lightManager_->lightGroups_[0].SetDirLightActive(0, true);
	lightManager_->lightGroups_[0].SetDirLightActive(1, false);
	lightManager_->lightGroups_[0].SetDirLightActive(2, false);

	lightManager_->lightGroups_[0].SetPointLightActive(0, false);
	lightManager_->lightGroups_[0].SetPointLightActive(1, false);
	lightManager_->lightGroups_[0].SetPointLightActive(2, false);

	lightManager_->lightGroups_[0].SetSpotLightActive(0, false);
	lightManager_->lightGroups_[0].SetSpotLightActive(1, false);

	lightManager_->lightGroups_[0].SetAmbientColor({ 0.25f,0.25f,0.25f });
	lightManager_->lightGroups_[0].SetDirLightColor(0, { 0.05f,0.05f,0.05f });

	Object3D::SetLight(&lightManager_->lightGroups_[0]);

	objobj3_.objDrawInit("Resources/obj/skydome/", "skydome.obj");
	objobj3_.SetScale({ 1000,1000,1000 });

	cameobj_ = cameraObj((float)WinApp::GetInstance()->getWindowSizeWidth(), (float)WinApp::GetInstance()->getWindowSizeHeight());

	cameobj_.pos_ = { 0,0,-50 };

	textureNum_ = Texture::GetInstance()->loadTexture("Resources/titleText.png","titleText");

	title_.initialize(textureNum_);

	title_.pos_ = { (float)WinApp::SWindowWidth_ / 2,(float)WinApp::SWindowHeight_ / 2 };
	title_.scale_ = { 1,1 };


	//playerのinit
	play_.Init();

	play_.isTitle_ = true;
	play_.attackFlag_ = true;

	play_.playerCamera_.pos_ = { 0,0,0 };

	cameobj_ = play_.playerCamera_;
	Camera::nowCamera = play_.playerCamera_.GetCameraP();

	//レベル読み込み
	std::unique_ptr<LevelData> levelData = JsonLevelLoader::LoadJsonFile("MapTest");

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

	ModelManager::GetInstance()->Load("testFBX", "gltf", "basketballmanBox", "basketballman2");
	ModelManager::GetInstance()->Load("testFBX", "gltf", "whiteBox", "white1x1");
	ModelManager::GetInstance()->Load("testGLTFBall", "gltf", "whiteBall", "white1x1");
	ModelManager::GetInstance()->Load("testFBX", "gltf", "Ground", "Dirt", "jpg");
	ModelManager::GetInstance()->Load("testFBX", "gltf", "Building", "Biru2");

	testModel_ = ModelManager::GetInstance()->SearchModelData("basketballmanBox");
	levelModel_ = ModelManager::GetInstance()->SearchModelData("whiteBall");
	levelBallModel_ = ModelManager::GetInstance()->SearchModelData("whiteBall");
	levelGroundModel_ = ModelManager::GetInstance()->SearchModelData("Ground");
	levelBuildingModel_ = ModelManager::GetInstance()->SearchModelData("Building");

	enemys_ = EnemyManager::GetInstance();


	eventManager_ = EventPointManager::GetInstance();

	//イベントデータの読み込み
	eventManager_->LoadEventData("titleEvent");

}

void TitleScene::Finalize()
{
	CollisionManager::GetInstance()->AllRemoveCollider(); 
	eventManager_->reset();
	enemys_->Reset();
	EmitterManager::GetInstance()->reset();
}

void TitleScene::Update()
{


	if (chengCamera_)
	{
		Camera::nowCamera = play_.playerCamera_.GetCameraP();
	}
	else
	{
		cameobj_.pos_ = cameraPos_;

		cameobj_.upDate();
		Camera::nowCamera = cameobj_.GetCameraP();
	}
	
	lightManager_->lightGroups_[0].Update();

	if ((Input::GetInstance()->GetMouseButtonDown(0)and Input::GetInstance()->GetMouseInWindow()) or Input::GetInstance()->GetGamePadButton(XINPUT_GAMEPAD_A))
	{
		startFlag_ = true;
		play_.attackFlag_ = false;
	}

	if (startFlag_)
	{

		if (eventManager_->GetPEventPoint()->GetEventSeting().enemyNum > EventEnemyCount_ and EventWaitTimer_>=EventWait_)
		{
			EventWaitTimer_ = 0;
			play_.reticle3DObj_.Trans_ = eventManager_->GetPEventPoint()->GetEventSeting().enemySpawnPos[EventEnemyCount_];
			play_.Attack();
			EventEnemyCount_++;
		}

		if (EventWaitTimer_ <= EventWait_)
		{
			EventWaitTimer_++;
		}

		if (eventManager_->GetEventAllEnd())
		{
			SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		}
	}

	objobj3_.Update();

	title_.Update();

	if (Input::GetInstance()->TriggerKey(DIK_F3))
	{
		debugMenu_ = !debugMenu_;
	}

	play_.Update();

	for (LevelObj a : levelObj)
	{
		a.obj.Update();
	}

	for (uint16_t b = 0; b < wallObj_.size(); b++)
	{
		wallObj_[b]->obj.Update();
	}

	enemys_->UpDate(play_.playerCamera_.GetCamera().eye_);

	CollisionManager::GetInstance()->CheckAllCollisions();

	eventManager_->Update();

	EmitterManager::GetInstance()->Update();

	ImguiUpdate();
}

void TitleScene::Draw()
{
	objobj3_.Draw();

	for (LevelObj a : levelObj)
	{
		if (a.name.find("box1") != std::string::npos)
		{
			a.obj.FBXDraw(*levelModel_);
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
		wallObj_[b]->obj.Draw(levelBuildingModel_);
	}



	enemys_->Draw();

	play_.Draw();

	eventManager_->Draw();

	EmitterManager::GetInstance()->Draw();

	title_.Draw();
}

void TitleScene::ImguiUpdate()
{
#ifdef _DEBUG


	if (debugMenu_)
	{

#pragma region check

		ImGui::Begin("check");

		ImGui::Text("%0.0fFPS", ImGui::GetIO().Framerate);

		ImGui::Text("eventEnd:%d", eventManager_->GetInstance()->GetPEventPoint()->GetIsFinished());
		ImGui::Text("eventAllEnd:%d", eventManager_->GetInstance()->GetEventAllEnd());
		ImGui::Text("eventNum:%d", eventManager_->GetInstance()->GetEventNum());
		ImGui::Text("eventcount:%d", eventManager_->GetInstance()->GetEventCount());


#pragma endregion

		ImGui::End();
	}

#pragma region camera

	ImGui::Begin("camera");

	ImGui::Checkbox("chengCamera", &chengCamera_);

	ImGui::DragFloat("cameraX", &cameraPos_.x, 1.0f, -1000.0f, 1000.0f);
	ImGui::DragFloat("cameraY", &cameraPos_.y, 1.0f, -1000.0f, 1000.0f);
	ImGui::DragFloat("cameraZ", &cameraPos_.z, 1.0f, -1000.0f, 1000.0f);


	ImGui::Text("reset");

	if (ImGui::Button("posX"))
	{
		cameraPos_.x = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("posY"))
	{
		cameraPos_.y = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("posZ"))
	{
		cameraPos_.z = -200;
	}

	if (Input::GetInstance()->PushKey(DIK_UP))
	{
		cameraPos_.x += Vector3::normalize(Camera::nowCamera->forward_).x;
		cameraPos_.z += Vector3::normalize(Camera::nowCamera->forward_).z;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN))
	{
		cameraPos_.x += -Vector3::normalize(Camera::nowCamera->forward_).x;
		cameraPos_.z += -Vector3::normalize(Camera::nowCamera->forward_).z;
	}
	if (Input::GetInstance()->PushKey(DIK_RIGHT))
	{
		cameraPos_ += Camera::nowCamera->rightDirection;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT))
	{
		cameraPos_ += -Camera::nowCamera->rightDirection;
	}

	ImGui::End();

#pragma endregion

#endif
}