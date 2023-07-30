#include "GameScene.h"
#include "EmitterManager.h"

void GameScene::Initialize()
{

	xAudio_ = XAudio::GetInstance();

	test_ = xAudio_->SoundLoadWave("Resources/sound/music_InGame.wav");

	LightGroup::Staticlnitialize();

	//ライトの生成
	lightManager_ = LightManager::GetInstance();

	lightManager_->CreateLightGroup();

	lightManager_->lightGroups_[0].SetDirLightActive(0, false);
	lightManager_->lightGroups_[0].SetDirLightActive(1, false);
	lightManager_->lightGroups_[0].SetDirLightActive(2, false);

	lightManager_->lightGroups_[0].SetPointLightActive(0, true);
	lightManager_->lightGroups_[0].SetPointLightActive(1, false);
	lightManager_->lightGroups_[0].SetPointLightActive(2, false);

	lightManager_->lightGroups_[0].SetSpotLightActive(0, true);
	lightManager_->lightGroups_[0].SetSpotLightActive(1, false);

	//lightGroup->SetLightColor({ 1,1,1 });

	Object3D::SetLight(&lightManager_->lightGroups_[0]);

	camera_ = Camera((float)WinApp::GetInstance()->getWindowSizeWidth(), (float)WinApp::GetInstance()->getWindowSizeHeight());

	camera_.eye_ = { 0.0f,0.0f,-1000.0f };

	cameobj_ = cameraObj((float)WinApp::GetInstance()->getWindowSizeWidth(), (float)WinApp::GetInstance()->getWindowSizeHeight());

	playerCameobj_.pos_.z = -200;

	//cameobj.cameobj.SetParent(&objobj);


	//.objのオブジェクト
	//DrawOBJ test(winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());



	play_.Init("Resources/obj/karaage/", "karaage.obj");

	


	sphere_.center_ = { 0,0,0 };
	sphere_.radius_ = 1.0f;

	plane_.normal_ = { 0,1,0 };
	plane_.distance_ = 0.0f;

	triangle_.p0_ = { -50.0f,-50.0f,50.0f };
	triangle_.p1_ = { 50.0f,-50.0f,50.0f };
	triangle_.p2_ = { -50.0f, 50.0f,50.0f };
	triangle_.ComputeNormal();
	//triangle.normal = { 0.0f,0.0f,-1.0f,0 };

	ray_.start_ = { 0.0f,0.0f,0.0f };
	ray_.dir_ = { 0.0f,0.0f,-1.0f };

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
	/*LevelWallObj newWall;
	obj.Init();
	newWall.obj = obj;
	wallObj_.push_back(newWall);*/

	objobj3_.objDrawInit("Resources/obj/skydome/", "skydome.obj");

	testFBX_.FBXInit();

	testModel_ = std::make_unique<AnimationModel>();
	levelModel_ = std::make_unique<AnimationModel>();
	levelBallModel_ = std::make_unique<AnimationModel>();
	levelGroundModel_ = std::make_unique<AnimationModel>();
	levelBuildingModel_ = std::make_unique<AnimationModel>();

	testModel_->Load("testFBX", "gltf", "basketballman2");
	levelModel_->Load("testFBX", "gltf", "white1x1");
	levelBallModel_->Load("testGLTFBall", "gltf", "white1x1");
	levelGroundModel_->Load("testFBX", "gltf", "Dirt", "jpg");
	levelBuildingModel_->Load("testFBX", "gltf", "Biru2");


	//spritecommon_->initialize();
	
	
	objobj3_.SetScale({ 1000,1000,1000 });


	XAudio::PlaySoundData(test_, 0.3f, true);

	XAudio::StapSoundData(test_);

	enemys_ = EnemyManager::GetInstance();

	enemys_->PopEnemy(Vector3(0, 0, -100));

	eventManager = EventPointManager::GetInstance();

	eventManager->SetDebugMoveEvent();
	
	eventManager->SetDebugBattleEvent();
}

void GameScene::Finalize()
{

	//XAudio::StapSoundData(test_);

	

}

void GameScene::Update()
{


#pragma region DirectX毎フレーム処理

	//DirectX毎フレーム処理　ここから

#pragma region キーボード情報の取得



#pragma endregion

#pragma region 更新処理



	if (Input::GetInstance()->PushKey(DIK_UP))
	{
		cameraPos_.x += Vector3::normalize(debugCamera_.forward_).x;
		cameraPos_.z += Vector3::normalize(debugCamera_.forward_).z;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN))
	{
		cameraPos_.x += -Vector3::normalize(debugCamera_.forward_).x;
		cameraPos_.z += -Vector3::normalize(debugCamera_.forward_).z;
	}
	if (Input::GetInstance()->PushKey(DIK_RIGHT))
	{
		cameraPos_.x += 1;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT))
	{
		cameraPos_.x += -1;
	}

	Vector4 moveY(0, 0.01f, 0, 0);
	if (Input::GetInstance()->PushKey(DIK_W))
	{
		movecoll_.y += moveY.y;
	}
	if (Input::GetInstance()->PushKey(DIK_S))
	{
		movecoll_.y -= moveY.y;
	}
	Vector4 moveX(0.01f, 0, 0, 0);
	if (Input::GetInstance()->PushKey(DIK_D))
	{
		movecoll_.x += moveX.x;
	}
	if (Input::GetInstance()->PushKey(DIK_A))
	{
		movecoll_.x -= moveX.x;
	}

	if (Input::GetInstance()->PushKey(DIK_Z))
	{
		movecoll_.z += 0.01f;
	}

	if (Input::GetInstance()->PushKey(DIK_X))
	{
		movecoll_.z -= 0.01f;
	}

	sphere_.center_ = { movecoll_.x ,movecoll_.y,movecoll_.z };

	//ray.start = { movecoll.x ,movecoll.y,movecoll.z};

	bool hit = Collision::CheckRay2Sphere(ray_, sphere_);

	lightManager_->lightGroups_[0].SetAmbientColor(Vector3(ambientColor0_[0], ambientColor0_[1], ambientColor0_[2]));

	lightManager_->lightGroups_[0].SetDirLightDir(0, { lightDir0_[0],lightDir0_[1] ,lightDir0_[2],0 });
	lightManager_->lightGroups_[0].SetDirLightColor(0, { lightColor0_[0],lightColor0_[1] ,lightColor0_[2] });

	lightManager_->lightGroups_[0].SetDirLightDir(1, { lightDir1_[0],lightDir1_[1] ,lightDir1_[2],0 });
	lightManager_->lightGroups_[0].SetDirLightColor(1, { lightColor1_[0],lightColor1_[1] ,lightColor1_[2] });

	lightManager_->lightGroups_[0].SetDirLightDir(2, { lightDir2_[0],lightDir2_[1] ,lightDir2_[2],0 });
	lightManager_->lightGroups_[0].SetDirLightColor(2, { lightColor2_[0],lightColor2_[1] ,lightColor2_[2] });
	//lightGroup->SetDirLightDir(2,{ lightDir2.x,lightDir2.y ,lightDir2.z });

	lightManager_->lightGroups_[0].SetPointLightPos(0, pointLightPos_);
	lightManager_->lightGroups_[0].SetPointLightColor(0, { pointLightColor_[0],pointLightColor_[1] ,pointLightColor_[2] });
	lightManager_->lightGroups_[0].SetPointLightAtten(0, { pointLightAtten_[0],pointLightAtten_[1] ,pointLightAtten_[2] });

	lightManager_->lightGroups_[0].SetSpotLightPos(0, spotLightPos_);
	lightManager_->lightGroups_[0].SetSpotLightDir(0, { spotDir_[0],spotDir_[1] ,spotDir_[2] });
	lightManager_->lightGroups_[0].SetSpotLightColor(0, { spotLightColor_[0],spotLightColor_[1] ,spotLightColor_[2] });
	lightManager_->lightGroups_[0].SetSpotLightAtten(0, { spotLightAtten_[0],spotLightAtten_[1] ,spotLightAtten_[2] });
	lightManager_->lightGroups_[0].SetSpotLightFactorAngle(0, { spotLightFactorAngle_[0],spotLightFactorAngle_[1]});

	lightManager_->lightGroups_[0].SetSpotLightPos(1, spotLightPos_);
	lightManager_->lightGroups_[0].SetSpotLightDir(1, { spotDir_[0],spotDir_[1] ,spotDir_[2] });
	lightManager_->lightGroups_[0].SetSpotLightColor(1, { spotLightColor_[0],spotLightColor_[1] ,spotLightColor_[2] });
	lightManager_->lightGroups_[0].SetSpotLightAtten(1, { spotLightAtten_[0],spotLightAtten_[1] ,spotLightAtten_[2] });
	lightManager_->lightGroups_[0].SetSpotLightFactorAngle(1, { spotLightFactorAngle_[0],spotLightFactorAngle_[1] });

	if (Input::GetInstance()->TriggerKey(DIK_B))
	{
		IsUseCameraMouse_ = !IsUseCameraMouse_;
	}

#pragma region imgui_Light

	ImGui::Begin("Light");
	//ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 200));

	ImGui::ColorEdit3("ambientColor", ambientColor0_, ImGuiColorEditFlags_Float);
	if (ImGui::CollapsingHeader("Directional"))
	{

		ImGui::InputFloat3("lightDir0", lightDir0_);
		ImGui::ColorEdit3("lightColor0", lightColor0_, ImGuiColorEditFlags_Float);
		ImGui::InputFloat3("lightDir1", lightDir1_);
		ImGui::ColorEdit3("lightColor1", lightColor1_, ImGuiColorEditFlags_Float);
		ImGui::InputFloat3("lightDir2", lightDir2_);
		ImGui::ColorEdit3("lightColor2", lightColor2_, ImGuiColorEditFlags_Float);
	}

	if (ImGui::CollapsingHeader("point"))
	{
		ImGui::ColorEdit3("pointLightColor", pointLightColor_, ImGuiColorEditFlags_Float);
		ImGui::SliderFloat3("pointLightPos", pointPos, -50.0f, 50.0f, "%.3f");
		ImGui::InputFloat3("pointLightAtten", pointLightAtten_);
	}

	if (ImGui::CollapsingHeader("Spot"))
	{
		ImGui::ColorEdit3("spotLightColor", spotLightColor_, ImGuiColorEditFlags_Float);
		ImGui::SliderFloat3("spotLightPos", spotPos_, -50.0f, 50.0f, "%.3f");
		ImGui::InputFloat3("spotLightDir", spotDir_);
		ImGui::InputFloat3("spotLightAtten", spotLightAtten_);
		ImGui::InputFloat2("spotLightFactorAngle", spotLightFactorAngle_);
	}

	pointLightPos_ = Vector3{ pointPos[0],pointPos[1] ,pointPos[2] };
	spotLightPos_ = Vector3{ spotPos_[0],spotPos_[1] ,spotPos_[2] };

	ImGui::End();

#pragma endregion

#pragma region Collision

	ImGui::Begin("Collision");

	//ImGui::SetWindowSize(ImVec2(300, 300));

	ImGui::Text("move:WASDZX");

	ImGui::SliderFloat("movecollx", &movecoll_.x, -200.0f, 200.0f, "%.3f");
	ImGui::SliderFloat("movecolly", &movecoll_.y, -200.0f, 200.0f, "%.3f");
	ImGui::SliderFloat("movecollz", &movecoll_.z, -200.0f, 200.0f, "%.3f");

	ImGui::Text("x:%f y:%f z:%f", ray_.start_.x, ray_.start_.y, ray_.start_.z);

	if (hit)
	{
		ImGui::Text("hit");
	}
	

	ImGui::End();

#pragma endregion

#pragma region camera

	ImGui::Begin("camera");

	
	ImGui::DragFloat("cameraX", &cameraPos_.x, 1.0f, -1000.0f, 1000.0f);
	ImGui::DragFloat("cameraY", &cameraPos_.y, 1.0f, -1000.0f, 1000.0f);
	ImGui::DragFloat("cameraZ", &cameraPos_.z, 1.0f, -1000.0f, 1000.0f);

	ImGui::DragFloat("cameraRotX", &cameraRot_.x, 0.1f, -5.0f, 5.0f);
	ImGui::DragFloat("cameraRotY", &cameraRot_.y, 0.1f, -5.0f, 5.0f);
	ImGui::DragFloat("cameraRotZ", &cameraRot_.z, 0.1f, -5.0f, 5.0f);

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
	if (ImGui::Button("rotX"))
	{
		cameraRot_.x = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("rotY"))
	{
		cameraRot_.y = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("rotZ"))
	{
		cameraRot_.z = 0;
	}

	
	
	ImGui::End();

#pragma endregion


#pragma region input

	ImGui::SetNextWindowSize(ImVec2(300, 300));

	ImGui::Begin("input");

	ImGui::Text("pos x:%f y:%f", Input::GetInstance()->GetMousePos().x, Input::GetInstance()->GetMousePos().y);
	ImGui::Text("oldpos x:%f y:%f", Input::GetInstance()->GetOldMousePos().x, Input::GetInstance()->GetOldMousePos().y);
	ImGui::Text("move x:%f y:%f z:%f", Input::GetInstance()->GetMouseMove().x, Input::GetInstance()->GetMouseMove().y, Input::GetInstance()->GetMouseMove().z);

	ImGui::Checkbox("useMouseCamera(B)", &IsUseCameraMouse_);

	if (Input::GetInstance()->GetMouseButton(0))
	{
		ImGui::Text("LHit");
	}

	if (Input::GetInstance()->GetMouseButton(1))
	{
		ImGui::Text("RHit");
	}

	if (Input::GetInstance()->GetIsUseGamePad())
	{
		ImGui::Text("useGamePad");

		ImGui::Text("LStick x:%f y:%f", Input::GetInstance()->GetGamePadLStick().x, Input::GetInstance()->GetGamePadLStick().y);
		ImGui::Text("RStick x:%f y:%f", Input::GetInstance()->GetGamePadRStick().x, Input::GetInstance()->GetGamePadRStick().y);

		ImGui::Text("LStick(key) x:%f y:%f", Input::GetInstance()->GetLStick(true, true).x, Input::GetInstance()->GetLStick(true, true).y);
		ImGui::Text("RStick(key) x:%f y:%f", Input::GetInstance()->GetRStick(true, true).x, Input::GetInstance()->GetRStick(true, true).y);

		if (Input::GetInstance()->GetGamePadButton(XINPUT_GAMEPAD_A))
		{
			ImGui::Text("AButton");
		}

		if (Input::GetInstance()->GetGamePadButton(XINPUT_GAMEPAD_B))
		{
			ImGui::Text("BButton");
		}

		if (Input::GetInstance()->GetGamePadButton(XINPUT_GAMEPAD_X))
		{
			ImGui::Text("XButton");
		}

		if (Input::GetInstance()->GetGamePadButton(XINPUT_GAMEPAD_Y))
		{
			ImGui::Text("YButton");
		}

		if (Input::GetInstance()->GetGamePadButton(XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			ImGui::Text("LButton");
		}

		if (Input::GetInstance()->GetGamePadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			ImGui::Text("RButton");
		}

	}
	else
	{
		ImGui::Text("UnUseGamePad");
	}





	ImGui::End();

#pragma endregion

#pragma region check

	ImGui::Begin("check");

	ImGui::Checkbox("chengCamera", &chengCamera_);
	
	ImGui::Text("%0.0fFPS", ImGui::GetIO().Framerate);

	ImGui::Text("eventEnd:%d", eventManager->GetInstance()->GetPEventPoint()->GetIsFinished());	

	ImGui::End();

#pragma endregion

#pragma region player

	ImGui::Begin("player");

	ImGui::Text("pos:%0.2f,%0.2f,%0.2f", play_.playerObj_.GetWorldPos().x, play_.playerObj_.GetWorldPos().y, play_.playerObj_.GetWorldPos().z);
	ImGui::Checkbox("chengHide", &play_.cameraCheng_);
	ImGui::Text("movetimer:%0.0f", play_.time_);	
	ImGui::Checkbox("playerDebugShot", &play_.isDebugShot_);
	ImGui::InputFloat("playerShotCT", &play_.bulletMaxCT_, 1, 5);
	ImGui::InputFloat("playerShotSpeed", &play_.bulletSpeed_, 1, 5);
	ImGui::Text("playCameraEye:%0.3f,%0.3f,%0.3f", play_.playCamera_.eye_.x, play_.playCamera_.eye_.y, play_.playCamera_.eye_.z);

	ImGui::End();

#pragma endregion

#pragma region enemy

	ImGui::Begin("enemy");

	ImGui::Text("enemyNum:%d", enemys_->enemyCount_);

	ImGui::Checkbox("enemyDebugShot", &enemys_->isDebugShot_);
	ImGui::InputFloat("enemyShotSpeed", &enemys_->bulletSpeed_, 0.1f, 5);

	ImGui::DragFloat3("popEnemyPos", PopPos);
	enemyPopPos = { PopPos[0],PopPos[1],PopPos[2] };

	if (ImGui::Button("popEnemy"))
	{
		enemys_->PopEnemy(enemyPopPos);
	}

	ImGui::End();


#pragma endregion


#ifdef _DEBUG

	ImGui::ShowDemoWindow();
#endif
	if (chengCamera_)
	{
		
		//play_.playerCamera_.upDate();
		//play_.playerCamera_.pos_ = cameraPos_;
		cameobj_ = play_.playerCamera_;
	}
	else
	{
		debugCamera_.eye_ = cameraPos_;
		
		if (IsUseCameraMouse_)
		{
			if (!Input::GetInstance()->PushKey(DIK_LCONTROL))
			{
				WinApp::GetInstance()->SetMousePos(WinApp::SWindowWidth_ / 2, WinApp::SWindowHeight_ / 2);
			}

			Camera mouseCamera = debugCamera_;

			Vector3 mouseMove = Input::GetInstance()->GetMouseMove() / 1000;

			mouseCameraRot += {0, mouseMove.x, 0};

			Object3D cameobj;

			cameobj.SetPos(cameraPos_);
			cameobj.SetRotate(mouseCameraRot);
			cameobj.matWorldGeneration();

			mouseCamera.eye_ = cameobj.GetWorldPos();

			Vector3 forward = { 0.0f, 0.0f, 1.0f };

			forward = VectorMat(forward, cameobj.GetWorldMat());

			mouseCamera.target_.x = mouseCamera.eye_.x + forward.x;
			mouseCamera.target_.z = mouseCamera.eye_.z + forward.z;
			mouseCamera.target_.y -= mouseMove.y + forward.y;



			Vector3 up(0, 1, 0);

			mouseCamera.up_ = VectorMat(up, cameobj.GetWorldMat());

			debugCamera_ = mouseCamera;
		}
		else
		{
			Object3D cameobj;

			cameobj.SetPos(cameraPos_);
			cameobj.SetRotate(cameraRot_);
			cameobj.matWorldGeneration();

			debugCamera_.eye_ = cameobj.GetWorldPos();

			Vector3 forward = { 0.0f, 0.0f, 1.0f };

			forward = VectorMat(forward, cameobj.GetWorldMat());

			debugCamera_.target_ = debugCamera_.eye_ + forward;

			Vector3 up(0, 1, 0);

			debugCamera_.up_ = VectorMat(up, cameobj.GetWorldMat());

		}

		debugCamera_.upDate();

		
		cameobj_.SetCamera(debugCamera_);
		
	}

	reloadLevel(DIK_K, "MapTest");

	
	play_.Update(cameobj_.GetCamera());
	lightManager_->lightGroups_[0].Update();


	objobj3_.Update(cameobj_.GetCamera());

	testFBX_.Update(cameobj_.GetCamera());

	for (LevelObj a : levelObj)
	{
		a.obj.Update(cameobj_.GetCamera());
	}

	for (uint16_t b = 0; b < wallObj_.size(); b++)
	{
		wallObj_[b]->obj.Update(cameobj_.GetCamera());
	}

	enemys_->UpDate(cameobj_.GetCamera(), play_.playerObj_.GetWorldPos());

	CollisionManager::GetInstance()->CheckAllCollisions();

	eventManager->Update();

	EmitterManager::GetInstance()->Update(cameobj_.GetCamera());

}

void GameScene::Draw()
{
#pragma region 描画コマンド

	
	// 4.描画コマンドここから

	



	objobj3_.Draw();

	//testFBX_.FBXDraw(*testModel_.get());

	//test.Draw(directXinit->GetcmdList().Get(), PipeLineRuleFlag, true, true);


	for (LevelObj a : levelObj)
	{
		if (a.name.find("box1")!= std::string::npos)
		{
			a.obj.FBXDraw(*levelModel_.get());
		}
		else if (a.name.find("ball") != std::string::npos)
		{
			a.obj.FBXDraw(*levelBallModel_.get());
		}
		else if(a.name.find("Ground")!=std::string::npos)
		{
			a.obj.FBXDraw(*levelGroundModel_.get());
		}
		else if (a.name.find("building") != std::string::npos)
		{
			a.obj.FBXDraw(*levelBuildingModel_.get());
		}
		else
		{
			a.obj.FBXDraw(*levelModel_.get());
		}
	}

	for (uint16_t b = 0; b < wallObj_.size(); b++)
	{
		wallObj_[b]->obj.Draw(levelBuildingModel_.get());
	}

	play_.Draw();

	enemys_->Draw();


	EmitterManager::GetInstance()->Draw();


	// 4.描画コマンドここまで

	

#pragma endregion

	

	//DirectX毎フレーム処理　ここまで

#pragma endregion



#pragma region 描画処理

}

bool GameScene::CollsionSphere(const Vector3& posA, const float& rA, const Vector3& posB, float rB)
{
	Vector3 AB = posB - posA;

	float range = AB.length();

	if (range < rA + rB)
	{
		return true;
	}
	else
	{
		return false;
	}


}

void GameScene::reloadLevel(const BYTE& CheckKey, std::string filename)
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
				newWall->obj.Init();
				newWall->obj.obj_.Trans_ = Vector3{ objData.trans_.x,objData.trans_.y ,objData.trans_.z };
				newWall->obj.obj_.Rotate_ = Vector3{ Util::AngleToRadian(objData.rot_.x),Util::AngleToRadian(objData.rot_.y) ,Util::AngleToRadian(objData.rot_.z) };
				newWall->obj.obj_.Scale_ = Vector3{ objData.scale_.x,objData.scale_.y ,objData.scale_.z };
				newWall->obj.obj_.matWorldGeneration();
				newWall->name = objData.name_;

				wallObj_.emplace_back(std::move(newWall));
			}

		}
	}
}