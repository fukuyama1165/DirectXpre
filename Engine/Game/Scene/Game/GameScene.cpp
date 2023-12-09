#include "GameScene.h"
#include "EmitterManager.h"
#include "SceneManager.h"
#include "LevelLoader.h"
#include "Easing.h"

void GameScene::Initialize()
{

	xAudio_ = XAudio::GetInstance();

	test_ = xAudio_->SoundLoadWave("Resources/sound/music_InGame.wav");

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

	//lightGroup->SetLightColor({ 1,1,1 });

	lightManager_->lightGroups_[0].SetAmbientColor({ 0.25f,0.25f,0.25f });
	lightManager_->lightGroups_[0].SetDirLightColor(0,{ 0.05f,0.05f,0.05f });

	Object3D::SetLight(&lightManager_->lightGroups_[0]);

	camera_ = Camera((float)WinApp::GetInstance()->getWindowSizeWidth(), (float)WinApp::GetInstance()->getWindowSizeHeight());

	camera_.eye_ = { 0.0f,0.0f,-1000.0f };

	cameobj_ = cameraObj((float)WinApp::GetInstance()->getWindowSizeWidth(), (float)WinApp::GetInstance()->getWindowSizeHeight());

	//初期値店の移動を記録
	playerCameobj_.pos_.z = -200;

	//playerのinit
	play_.Init();

	objobj3_.objDrawInit("Resources/obj/skydome/", "skydome.obj");

	testObj_.FBXInit();

	testObj_.pos_ = { 0.0f,20.0f,10.0f };

	testFBX_.FBXInit();

	testFBX_.pos_ = { 0.0f,20.0f,0.0f };
	testFBX_.Scale_ = { 1.0f,1.0f,5.0f };
	testFBX_.useQuaternion = true;

	LevelLoader::GetInstance()->LoadLevel("MapTest4");

	testModel_ = ModelManager::GetInstance()->SearchModelData("basketballmanBox");
	
	objobj3_.SetScale({ 1000,1000,1000 });


	XAudio::PlaySoundData(test_, 0.1f, true);


	enemys_ = EnemyManager::GetInstance();


	eventManager = EventPointManager::GetInstance();

	
	//イベントデータの読み込み
	//eventManager->LoadEventData("Event2");
	//eventManager->LoadeefmEventData("stage1");
	eventManager->LoadeefmEventData("testEvent");
	//eventManager->LoadeefmEventData("testtest");

	eventManager->isNoTimer = false;


	Texture::GetInstance()->loadTexture("Resources/clearText.png", "clearText");

	Texture::GetInstance()->loadTexture("Resources/basketballman2.dds", "basketballman2");

	clearBackSprite_.initialize("white1x1");
	clearTextSprite_.initialize("clearText");

	clearBackSprite_.setColor({ 0,0,0,0.7f });
	clearBackSprite_.pos_ = { WinApp::GetInstance()->GetWindowCenter()};
	clearBackSprite_.scale_ = { WinApp::SWindowWidthF_,WinApp::SWindowHeightF_ };

	clearTextSprite_.pos_ = { WinApp::SWindowWidthF_/2,(WinApp::SWindowHeightF_/2) - (clearTextSprite_.GetTextureSize().y / 3) };

}

void GameScene::Finalize()
{

	XAudio::StapSoundData(test_);
	CollisionManager::GetInstance()->AllRemoveCollider();
	eventManager->reset();
	enemys_->Reset();
	

}

void GameScene::Update()
{


#pragma region DirectX毎フレーム処理

	//DirectX毎フレーム処理　ここから

#pragma region キーボード情報の取得



#pragma endregion

#pragma region 更新処理

	DebugUpdate();

	if (!pause_)
	{

		play_.Update();
		lightManager_->lightGroups_[0].Update();


		objobj3_.Update();
		testObj_.Update();

		testFBX_.Update();

		LevelLoader::GetInstance()->Update();

		enemys_->UpDate(play_.playerCamera_.GetCamera().eye_);

		CollisionManager::GetInstance()->CheckAllCollisions();

		eventManager->Update();

		EmitterManager::GetInstance()->Update();

		//ここに確認したい物とか動きを書いたらテストイベントで動いてくれるはず
		if (eventManager->NowEventDataFileName_ == "testEvent")
		{

			if (Input::GetInstance()->PushKey(DIK_T))
			{
				play_.playerCamera_.rotate_.x += 0.01f;
			}
			if (Input::GetInstance()->PushKey(DIK_F))
			{
				play_.playerCamera_.rotate_.y-= 0.01f;
			}
			if (Input::GetInstance()->PushKey(DIK_G))
			{
				play_.playerCamera_.rotate_.x-= 0.01f;
			}
			if (Input::GetInstance()->PushKey(DIK_H))
			{
				play_.playerCamera_.rotate_.y+= 0.01f;
			}

			if (Input::GetInstance()->PushKey(DIK_N))
			{
				ang_+=0.01f;
			}
			if (Input::GetInstance()->PushKey(DIK_M))
			{
				ang_-= 0.01f;
			}

			

			testObj_.SetPos({ sinf(ang_) * 10, h_,cosf(ang_)*10});

			//Quaternion rot2 = rot;

			Vector3 nowvec = Quaternion::RotateVector({ 0,0,1 }, testFBX_.quaternionRot_).normalize();
			//
			//nowvec.normalize();

			rot = Quaternion::DirectionToDirection({ 0,0,1 }, Vector3::normalize(testObj_.GetPos() - testFBX_.GetPos()));


			if (nowvec!= Vector3::normalize(testObj_.GetPos() - testFBX_.GetPos()))
			{
				testFBX_.quaternionRot_ =Quaternion::Normalize(rot);

				
			}
			

			ImGui::Begin("bebug");

			ImGui::Text("ang:%0.2ff", ang_);
			ImGui::Text("rot:%0.2ff,%0.2ff,%0.2ff,%0.2ff", rot.v.x, rot.v.y, rot.v.z, rot.w);
			ImGui::Text("now:%0.2ff,%0.2ff,%0.2ff", nowvec.x, nowvec.y, nowvec.z);
			ImGui::Text("quaternionRot:%0.2ff,%0.2ff,%0.2ff,%0.2ff", testFBX_.quaternionRot_.v.x, testFBX_.quaternionRot_.v.y, testFBX_.quaternionRot_.v.z, testFBX_.quaternionRot_.w);
			ImGui::DragFloat("h", &h_,0.01f, -50.0f, 50.0f, "%.2f");

			ImGui::End();

		}


		//ゲームオーバー処理
		if (play_.hp_ <= 0)
		{
#ifdef _DEBUG

#else
			SceneManager::GetInstance()->ChangeScene("TITLE");

#endif
		}

		//ゲームクリア処理
		if (eventManager->GetEventAllEnd() && eventManager->NowEventDataFileName_ != "testEvent")
		{

			if (clearEffectTime_ >= clearEffectMaxTime_ && (Input::GetInstance()->GetMouseButtonDown(0) || Input::GetInstance()->GetGamePadButtonDown(XINPUT_GAMEPAD_A)))
			{
				SceneManager::GetInstance()->ChangeScene("TITLE");
			}

			//連打防止したい
			if (clearEffectTime_ < clearEffectMaxTime_)
			{
				clearEffectTime_++;
			}

			if (clearEffectTime_ < clearEffectMaxTime_ && (Input::GetInstance()->GetMouseButtonDown(0) || Input::GetInstance()->GetGamePadButtonDown(XINPUT_GAMEPAD_A)))
			{
				clearEffectTime_ = clearEffectMaxTime_;
			}

			clearTextSprite_.scale_ = easeOutQuint(clearTextStartScale_, clearTextEndScale_, clearEffectTime_ / clearEffectMaxTime_);

			float textAlpha = easeInSine(0.1f, 1.0f, clearEffectTime_ / clearEffectAlphaMaxTime_);

			clearTextSprite_.setColor({ 1.0f,1.0f,1.0f,textAlpha });

			clearBackSprite_.Update();
			clearTextSprite_.Update();


		}
	}

}

void GameScene::Draw()
{
#pragma region 描画コマンド

	
	// 4.描画コマンドここから

	objobj3_.Draw();
	
	LevelLoader::GetInstance()->Draw();
	
	enemys_->Draw();
	
	play_.Draw();

	eventManager->Draw();

	EmitterManager::GetInstance()->Draw();

	//クリアしたときの描画(イベントがテスト用なら描画しない)
	if (eventManager->GetEventAllEnd() && eventManager->NowEventDataFileName_ != "testEvent")
	{

		clearBackSprite_.Draw();
		clearTextSprite_.Draw();
		

	}

	//ここに確認したい物とか動きを書いたらテストイベントで動いてくれるはず
	if (eventManager->NowEventDataFileName_ == "testEvent")
	{

		testFBX_.FBXDraw(*testModel_);
		testObj_.FBXDraw(*testModel_);

	}


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


void GameScene::DebugUpdate()
{
#ifdef _DEBUG
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

	Vector4 moveY(1.0f, 1.0f, 1.0f, 0);
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
		movecoll_.z += moveY.z;
	}
	if (Input::GetInstance()->PushKey(DIK_X))
	{
		movecoll_.z -= moveY.z;
	}

	//イベントが終わってテスト用のイベントなら動かせるように
	if (eventManager->GetEventAllEnd() && eventManager->NowEventDataFileName_ == "testEvent")
	{
		play_.playerCamera_.pos_ = movecoll_;
	}

	//sphere_.center_ = { movecoll_.x ,movecoll_.y,movecoll_.z };

	//ray.start = { movecoll.x ,movecoll.y,movecoll.z};

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
	lightManager_->lightGroups_[0].SetSpotLightFactorAngle(0, { spotLightFactorAngle_[0],spotLightFactorAngle_[1] });

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

	ImGui::Text("eye:%0.2f,%0.2f,%0.2f", debugCamera_.eye_.x, debugCamera_.eye_.y, debugCamera_.eye_.z);
	ImGui::Text("target:%0.2f,%0.2f,%0.2f", debugCamera_.target_.x, debugCamera_.target_.y, debugCamera_.target_.z);
	ImGui::Text("up:%0.2f,%0.2f,%0.2f", debugCamera_.up_.x, debugCamera_.up_.y, debugCamera_.up_.z);

	ImGui::Text("forward:%0.2f,%0.2f,%0.2f", debugCamera_.forward_.x, debugCamera_.forward_.y, debugCamera_.forward_.z);
	ImGui::Text("rightDirection:%0.2f,%0.2f,%0.2f", debugCamera_.rightDirection.x, debugCamera_.rightDirection.y, debugCamera_.rightDirection.z);

	ImGui::End();

#pragma endregion


#pragma region input

	ImGui::SetNextWindowSize(ImVec2(300, 300));

	ImGui::Begin("input");

	ImGui::Text("pos x:%f y:%f", Input::GetInstance()->GetMousePos().x, Input::GetInstance()->GetMousePos().y);
	ImGui::Text("oldpos x:%f y:%f", Input::GetInstance()->GetOldMousePos().x, Input::GetInstance()->GetOldMousePos().y);
	ImGui::Text("move x:%f y:%f z:%f", Input::GetInstance()->GetMouseMove().x, Input::GetInstance()->GetMouseMove().y, Input::GetInstance()->GetMouseMove().z);

	ImGui::Text("use KEY:%d pad:%d Mouse:%d", Input::GetInstance()->AllKeyCheck(), Input::GetInstance()->GetGamePadInput(), Input::GetInstance()->GetMouseInput());
	ImGui::Text("mouseInWindow:%d", Input::GetInstance()->GetMouseInWindow());

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

	if (ImGui::Button("pause"))
	{
		pause_ = !pause_;
	}

	ImGui::Text("%0.0fFPS", ImGui::GetIO().Framerate);

	ImGui::Text("eventEnd:%d", eventManager->GetInstance()->GetPEventPoint()->GetIsFinished());
	ImGui::Text("eventAllEnd:%d", eventManager->GetInstance()->GetEventAllEnd());

	//ImGui::Text("pos:%0.2f,%0.2f,%0.2f", wallObj_[0]->obj.obj_.GetWorldPos().x, wallObj_[0]->obj.obj_.GetWorldPos().y, wallObj_[0]->obj.obj_.GetWorldPos().z);

	ImGui::End();

#pragma endregion

#pragma region player

	ImGui::Begin("player");

	ImGui::Text("pos:%0.2f,%0.2f,%0.2f", play_.playerObj_.GetWorldPos().x, play_.playerObj_.GetWorldPos().y, play_.playerObj_.GetWorldPos().z);
	ImGui::Text("pos:%0.2f,%0.2f,%0.2f", play_.reticle3DObj_.GetWorldPos().x, play_.reticle3DObj_.GetWorldPos().y, play_.reticle3DObj_.GetWorldPos().z);
	ImGui::Text("objrot:%0.2f,%0.2f,%0.2f", play_.playerObj_.Rotate_.x, play_.playerObj_.Rotate_.y, play_.playerObj_.Rotate_.z);
	ImGui::Text("camerapos:%0.2f,%0.2f,%0.2f", play_.playerCamera_.pos_.x, play_.playerCamera_.pos_.y, play_.playerCamera_.pos_.z);
	ImGui::Text("hp:%0.0f", play_.hp_);
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
	ImGui::Checkbox("ShotStop", &enemys_->isDebugShotStop_);
	ImGui::InputFloat("enemyShotSpeed", &enemys_->bulletSpeed_, 0.1f, 5);

	ImGui::End();


#pragma endregion



#ifdef _DEBUG

	ImGui::ShowDemoWindow();
#endif


	LevelLoader::GetInstance()->reloadLevel(DIK_K, "MapTest");
#endif	

	if (chengCamera_)
	{
		cameobj_ = play_.playerCamera_;
		Camera::nowCamera = play_.playerCamera_.GetCameraP();
	}
	else
	{
		cameobj_.IsUseCameraMouse_ = IsUseCameraMouse_;
		cameobj_.pos_ = cameraPos_;

		cameobj_.upDate();
		Camera::nowCamera = cameobj_.GetCameraP();

		debugCamera_ = cameobj_.GetCamera();
	}
}
