#include "GameScene.h"

void GameScene::Initialize()
{

	xAudio_ = XAudio::GetInstance();

	test_ = xAudio_->SoundLoadWave("Resources/sound/music_InGame.wav");

	LightGroup::Staticlnitialize();

	//���C�g�̐���
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


	//.obj�̃I�u�W�F�N�g
	//DrawOBJ test(winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());


	charactorObj_.colorChangeInit();
	charactorObj3_.basicInit();
	charactorObj2_.colorChangeInit();



	texname_ = charactorObj2_.loadTexture("Resources/hokehoke.png");

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

	std::unique_ptr<LevelData> levelData = JsonLevelLoader::LoadJsonFile("scenetest4");

	

	for (auto& objData : levelData->objects_)
	{
		LevelObj newObject;
		newObject.obj.FBXInit();
		newObject.obj.Trans_ = Vector3{objData.trans_.x,objData.trans_.y ,objData.trans_.z };
		newObject.obj.Rotate_ = Vector3{objData.rot_.x,objData.rot_.y ,objData.rot_.z };
		newObject.obj.Scale_ = Vector3{objData.scale_.x,objData.scale_.y ,objData.scale_.z };
		newObject.obj.matWorldGeneration();
		newObject.name = objData.name_;

		levelObj.emplace_back(newObject);

	}

	objobj_.objDrawInit("Resources/obj/testBall/", "testBall.obj", true);
	objobj2_.objDrawInit("Resources/obj/collHittest/", "collHitTest.obj");
	objobj3_.objDrawInit("Resources/obj/skydome/", "skydome.obj");

	objobj_.Scale_ = { 0.95f,0.95f,0.95f };
	objobj2_.Scale_ = { 0.95f,0.95f,0.95f };

	testFBX_.FBXInit();

	testModel_ = std::make_unique<AnimationModel>();
	levelModel_ = std::make_unique<AnimationModel>();
	levelBallModel_ = std::make_unique<AnimationModel>();

	testModel_->Load("testFBX", "gltf", "basketballman2");
	levelModel_->Load("testFBX", "gltf", "white1x1");
	levelBallModel_->Load("testGLTFBall", "gltf", "white1x1");

	spritecommon_ = spritecommon_->GetInstance();

	//spritecommon_->initialize();

	sprite_.initialize(spritecommon_, 2);
	sprite2_.initialize(spritecommon_, 1);

	sprite_.pos_ = { 500,200 };

	sprite2_.pos_ = { 700,200 };

	sprite2_.scale_ = { 10,10 };

	sprite_.scale_ = { 10 ,10 };

	objobj_.SetPos({ -100,0,0 });
	objobj3_.SetScale({ 1000,1000,1000 });


	charactorObj3_.SetScale({ 0.001f,0.001f,100.0f });

	XAudio::PlaySoundData(test_, 0.3f, true);

	XAudio::StapSoundData(test_);

	enemys_.Init();

	enemys_.PopEnemy(Vector3(0, 0, -100));
	
}

void GameScene::Finalize()
{

	//XAudio::StapSoundData(test_);

	

}

void GameScene::Update()
{


#pragma region DirectX���t���[������

	//DirectX���t���[�������@��������

#pragma region �L�[�{�[�h���̎擾



#pragma endregion

#pragma region �X�V����





	/*if (input->PushKey(DIK_I))
	{
		sprite.rotate += 1;
	}
	if (input->PushKey(DIK_K))
	{
		sprite.rotate += -1;
	}
	if (input->PushKey(DIK_L))
	{
		sprite.posX = 1;
	}
	if (input->PushKey(DIK_J))
	{
		sprite.posX = -1;
	}*/



	if (Input::GetInstance()->PushKey(DIK_UP))
	{
		lightDir_.z = 1;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN))
	{
		lightDir_.z = -1;
	}
	if (Input::GetInstance()->PushKey(DIK_RIGHT))
	{
		lightDir_.x = 1;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT))
	{
		lightDir_.x = -1;
	}

	/*if (input->PushKey(DIK_W))
	{
		lightDir2.z = 1;
	}
	if (input->PushKey(DIK_S))
	{
		lightDir2.z = -1;
	}
	if (input->PushKey(DIK_D))
	{
		lightDir2.x = 1;
	}
	if (input->PushKey(DIK_A))
	{
		lightDir2.x = -1;
	}*/

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

	if (IsUseCameraMouse_)
	{
		if (!Input::GetInstance()->PushKey(DIK_LCONTROL))
		{
			WinApp::GetInstance()->SetMousePos(WinApp::SWindowWidth_ / 2, WinApp::SWindowHeight_ / 2);
		}

		cameraRot_.y += Input::GetInstance()->GetMouseMove().x / 1000;
		cameraRot_.x += Input::GetInstance()->GetMouseMove().y / 1000;
	}

	if (Input::GetInstance()->TriggerKey(DIK_F5))
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

	
	ImGui::SliderFloat("cameraX", &cameraPos_.x, -1000.0f, 1000.0f, "%.3f");
	ImGui::SliderFloat("cameraY", &cameraPos_.y, -1000.0f, 1000.0f, "%.3f");
	ImGui::SliderFloat("cameraZ", &cameraPos_.z, -1000.0f, 1000.0f, "%.3f");

	ImGui::SliderFloat("cameraRotX", &cameraRot_.x, -5.0f, 5.0f, "%.3f");
	ImGui::SliderFloat("cameraRotY", &cameraRot_.y, -5.0f, 5.0f, "%.3f");
	ImGui::SliderFloat("cameraRotZ", &cameraRot_.z, -5.0f, 5.0f, "%.3f");

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

	ImGui::Checkbox("useMouseCamera(F5)", &IsUseCameraMouse_);

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
	ImGui::Checkbox("chengHide", &play_.cameraCheng_);
	
	ImGui::Text("%0.0fFPS", ImGui::GetIO().Framerate);

	ImGui::End();

#pragma endregion

#pragma region player

	ImGui::Begin("player");

	ImGui::Text("movetimer:%0.0f", play_.time_);
	ImGui::Checkbox("playerDebugShot", &play_.isDebugShot_);
	ImGui::InputFloat("playerShotCT", &play_.bulletMaxCT_, 1, 5);
	ImGui::InputFloat("playerShotSpeed", &play_.bulletSpeed_, 1, 5);

	ImGui::End();

#pragma endregion

#pragma region enemy

	ImGui::Begin("enemy");

	ImGui::Checkbox("enemyDebugShot", &enemys_.isDebugShot_);
	ImGui::InputFloat("enemyShotSpeed", &enemys_.bulletSpeed_, 0.1f, 5);

	ImGui::End();


#pragma endregion


#pragma region sprite

	/*ImGui::Begin("spriteMove");

	ImGui::SetWindowSize(ImVec2(300, 300));

	ImGui::Text("spriteMove");

	ImGui::SliderFloat("X", &spriteMove.x, -500.0f, 500.0f, "%.3f");
	ImGui::SliderFloat("Y", &spriteMove.y, 0.0f, 500.0f, "%.3f");

	ImGui::End();*/

#pragma endregion


#ifdef _DEBUG

	ImGui::ShowDemoWindow();
#endif
	if (chengCamera_)
	{
		
		//play_.playerCamera_.upDate();
		play_.playerCamera_.pos_ = cameraPos_;
		cameobj_ = play_.playerCamera_;
	}
	else
	{
		debugCameobj_.pos_ = cameraPos_;
		debugCameobj_.rotate_ = cameraRot_;
		debugCameobj_.upDate();
		cameobj_ = debugCameobj_;
		
	}

	reloadLevel(DIK_K, "scenetest4");

	

	objobj_.SetPos(movecoll_);
	objobj2_.SetPos(movecoll_);

	
	play_.Update(cameobj_.GetCamera());
	lightManager_->lightGroups_[0].Update();

	sprite_.Update();
	sprite2_.Update();

	charactorObj_.Update(cameobj_.GetCamera());
	charactorObj2_.Update(cameobj_.GetCamera());
	charactorObj3_.Update(cameobj_.GetCamera());

	objobj_.Update(cameobj_.GetCamera());
	objobj2_.Update(cameobj_.GetCamera());
	objobj3_.Update(cameobj_.GetCamera());

	testFBX_.Update(cameobj_.GetCamera());

	for (LevelObj a : levelObj)
	{
		a.obj.Update(cameobj_.GetCamera());
	}

	enemys_.UpDate(cameobj_.GetCamera(), play_.playerObj_.GetWorldPos());

}

void GameScene::Draw()
{
#pragma region �`��R�}���h

	
	// 4.�`��R�}���h��������

	


	//charactorObj.Draw(0,1,0);
	//charactorObj3_.Draw(6,1,1);
	//charactorObj2.Draw(4, 1,0);
	/*if (hit == false)
	{*/
	//objobj_.Draw();
	/*}
	else
	{
		objobj2.Draw();
	}*/

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
		else
		{
			a.obj.FBXDraw(*levelModel_.get());
		}
	}


	play_.Draw();

	enemys_.Draw();

	//sprite_.Draw();
	//sprite2_.Draw();


	// 4.�`��R�}���h�����܂�

	

#pragma endregion

	

	//DirectX���t���[�������@�����܂�

#pragma endregion



#pragma region �`�揈��

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
		std::unique_ptr<LevelData> levelData = JsonLevelLoader::LoadJsonFile(filename);

		for (auto& objData : levelData->objects_)
		{
			LevelObj newObject;
			newObject.obj.FBXInit();
			newObject.obj.Trans_ = Vector3{ objData.trans_.x,objData.trans_.y ,objData.trans_.z };
			newObject.obj.Rotate_ = Vector3{ objData.rot_.x,objData.rot_.y ,objData.rot_.z };
			newObject.obj.Scale_ = Vector3{ objData.scale_.x,objData.scale_.y ,objData.scale_.z };
			newObject.obj.matWorldGeneration();
			newObject.name = objData.name_;

			levelObj.emplace_back(newObject);

		}
	}
}