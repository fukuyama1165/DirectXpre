#include "GameScene.h"

void GameScene::Initialize()
{
	
	IScene::Initialize();

	xAudio_ = XAudio::GetInstance();

	xAudio_->Init();

	test_= xAudio_->SoundLoadWave("Resources/sound/music_InGame.wav");

	LightGroup::Staticlnitialize();

	//ライトの生成
	lightManager_ = LightManager::GetInstance();

	lightManager_->CreateLightGroup();

	//lightGroup->SetLightColor({ 1,1,1 });

	Object3D::SetLight(&lightManager_->lightGroups_[0]);

	camera_ = Camera((float)WinApp::GetInstance()->getWindowSizeWidth(), (float)WinApp::GetInstance()->getWindowSizeHeight());

	camera_.eye_ = { 0.0f,0.0f,-1000.0f };

	cameobj_ = cameraObj((float)WinApp::GetInstance()->getWindowSizeWidth(), (float)WinApp::GetInstance()->getWindowSizeHeight());

	//cameobj.cameobj.SetParent(&objobj);

	
	//.objのオブジェクト
	//DrawOBJ test(winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());


	charactorObj_.colorChangeInit();
	charactorObj3_.basicInit();
	charactorObj2_.colorChangeInit();

	

	texname_ = charactorObj2_.loadTexture("Resources/hokehoke.png");

	//play_.Init("Resources/obj/karaage/", "karaage.obj");

	
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

	/*LevelData* levelData = JsonLevelLoader::LoadJsonFile("scenetest3");

	std::vector<Object3D> levelObj;

	for (auto& objData : levelData->objects)
	{
		Object3D newObject;
		newObject.objDrawInit("Resources/obj/testBox/", "testBox.obj");
		newObject.Trans_ = Vector3{objData.trans.x,objData.trans.y ,objData.trans.z };
		newObject.Rotate_ = Vector3{objData.rot.x,objData.rot.y ,objData.rot.z };
		newObject.Scale_ = Vector3{objData.scale.x,objData.scale.y ,objData.scale.z };
		newObject.matWorldGeneration();

		levelObj.emplace_back(newObject);

	}*/

	objobj_.objDrawInit("Resources/obj/testBall/", "testBall.obj", true);
	objobj2_.objDrawInit("Resources/obj/collHittest/", "collHitTest.obj");
	objobj3_.objDrawInit("Resources/obj/skydome/", "skydome.obj");

	objobj_.Scale_ = { 0.95f,0.95f,0.95f };
	objobj2_.Scale_ = { 0.95f,0.95f,0.95f };

	testFBX_.FBXInit();

	testModel_ = std::make_unique<AnimationModel>();

	testModel_->Load("testFBX", "gltf","white1x1");

	spritecommon_.initialize();

	sprite_.initialize(&spritecommon_, 2);
	sprite2_.initialize(&spritecommon_, 1);

	sprite_.posX_ = 100;
	sprite_.posY_ = 200;

	sprite2_.posX_ = 50;
	sprite2_.posY_ = 200;

	objobj_.SetPos({ -100,0,0 });
	objobj3_.SetScale({ 1000,1000,1000 });

	
	charactorObj3_.SetScale({ 0.001f,0.001f,100.0f });

	XAudio::PlaySoundData(test_,0.3f,true);

	XAudio::StapSoundData(test_);

}

void GameScene::Finalize()
{
	charactorObj_.deleteTexture();

	
	//delete(levelData);

	XAudio::StapSoundData(test_);

	XAudio::Final();


	IScene::Finalize();
}

void GameScene::Update()
{
	
	IScene::Update();

	

#pragma region DirectX毎フレーム処理

	//DirectX毎フレーム処理　ここから

#pragma region キーボード情報の取得

	

#pragma endregion

#pragma region 更新処理

	



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

	sprite2_.posX_ = spriteMove_.x;
	sprite2_.posY_ = spriteMove_.y;

	bool hit = Collision::CheckRay2Sphere(ray_, sphere_);

	lightManager_->lightGroups_[0].SetAmbientColor(XMFLOAT3(ambientColor0_));

	lightManager_->lightGroups_[0].SetDirLightDir(0, { lightDir0_[0],lightDir0_[1] ,lightDir0_[2],0 });
	lightManager_->lightGroups_[0].SetDirLightColor(0, { lightColor0_[0],lightColor0_[1] ,lightColor0_[2] });

	lightManager_->lightGroups_[0].SetDirLightDir(1, { lightDir1_[0],lightDir1_[1] ,lightDir1_[2],0 });
	lightManager_->lightGroups_[0].SetDirLightColor(1, { lightColor1_[0],lightColor1_[1] ,lightColor1_[2] });

	lightManager_->lightGroups_[0].SetDirLightDir(2, { lightDir2_[0],lightDir2_[1] ,lightDir2_[2],0 });
	lightManager_->lightGroups_[0].SetDirLightColor(2, { lightColor2_[0],lightColor2_[1] ,lightColor2_[2] });
	//lightGroup->SetDirLightDir(2,{ lightDir2.x,lightDir2.y ,lightDir2.z });

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
	ImGui::InputFloat3("lightDir0", lightDir0_);
	ImGui::ColorEdit3("lightColor0", lightColor0_, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("lightDir1", lightDir1_);
	ImGui::ColorEdit3("lightColor1", lightColor1_, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("lightDir2", lightDir2_);
	ImGui::ColorEdit3("lightColor2", lightColor2_, ImGuiColorEditFlags_Float);

	ImGui::End();

#pragma endregion

#pragma region Collision_camera

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
	if (ImGui::CollapsingHeader("camera"))
	{

		ImGui::SliderFloat("cameraX", &cameraPos_.x, -500.0f, 500.0f, "%.3f");
		ImGui::SliderFloat("cameraY", &cameraPos_.y, -500.0f, 500.0f, "%.3f");
		ImGui::SliderFloat("cameraZ", &cameraPos_.z, -500.0f, 500.0f, "%.3f");

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

	}

	ImGui::End();

#pragma endregion

#pragma region input

		ImGui::SetNextWindowSize(ImVec2(300, 300));

		ImGui::Begin("input");

		ImGui::Text("pos x:%f y:%f",Input::GetInstance()->GetMousePos().x, Input::GetInstance()->GetMousePos().y);
		ImGui::Text("oldpos x:%f y:%f",Input::GetInstance()->GetOldMousePos().x, Input::GetInstance()->GetOldMousePos().y);
		ImGui::Text("move x:%f y:%f z:%f",Input::GetInstance()->GetMouseMove().x, Input::GetInstance()->GetMouseMove().y, Input::GetInstance()->GetMouseMove().z);

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

	cameobj_.pos_ = cameraPos_;
	cameobj_.rotate_ = cameraRot_;

	objobj_.SetPos(movecoll_);
	objobj2_.SetPos(movecoll_);

	cameobj_.upDate();
	//play_.Update(cameobj_);
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

	/*for (Object3D a : levelObj)
	{
		a.Update(cameobj.GetCamera());
	}*/

	ImGuiManager::GetInstance()->End();
	
}

void GameScene::Draw()
{
#pragma region 描画コマンド

	DirectXInit::GetInstance()->DrawStart();
	// 4.描画コマンドここから

	//ビューポート設定
	//分割分のビューポートを用意(見にくいので減らした)
	D3D12_VIEWPORT viewport{};

	viewport.Width = (float)WinApp::GetInstance()->getWindowSizeWidth();//横幅
	viewport.Height = (float)WinApp::GetInstance()->getWindowSizeHeight();//縦幅
	viewport.TopLeftX = 0;//左上X
	viewport.TopLeftY = 0;//左上Y
	viewport.MinDepth = 0.1f;//最少深度(0でよい)
	viewport.MaxDepth = 1.0f;//最大深度(１でよい)

	//コマンドリストに追加
	DirectXInit::GetInstance()->GetcmdList()->RSSetViewports(1, &viewport);


	//シザー矩形設定
	D3D12_RECT scissorrect{};

	scissorrect.left = 0;//切り抜き座標左
	scissorrect.right = scissorrect.left + WinApp::GetInstance()->getWindowSizeWidth();//切り抜き座標右
	scissorrect.top = 0;//切り抜き座標上
	scissorrect.bottom = scissorrect.top + WinApp::GetInstance()->getWindowSizeHeight();//切り抜き座標下

	DirectXInit::GetInstance()->GetcmdList()->RSSetScissorRects(1, &scissorrect);

	//プリミティブ形状(三角形リスト)
	DirectXInit::GetInstance()->GetcmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//charactorObj.Draw(0,1,0);
	//charactorObj3_.Draw(6,1,1);
	//charactorObj2.Draw(4, 1,0);
	/*if (hit == false)
	{*/
	objobj_.Draw();
	/*}
	else
	{
		objobj2.Draw();
	}*/

	objobj3_.Draw();

	testFBX_.FBXDraw(*testModel_.get());

	//test.Draw(directXinit->GetcmdList().Get(), PipeLineRuleFlag, true, true);


	/*for (Object3D a : levelObj)
	{
		a.Draw();
	}*/


	//play.Draw(directXinit->GetcmdList().Get());

	//sprite.Draw(directXinit->GetcmdList().Get(), 2);
	//sprite2.Draw(directXinit->GetcmdList().Get());


	// 4.描画コマンドここまで

	ImGuiManager::GetInstance()->Draw();

#pragma endregion

	DirectXInit::GetInstance()->DrawEnd();

	//DirectX毎フレーム処理　ここまで

#pragma endregion



#pragma region 描画処理
	
}

bool GameScene::CollsionSphere(const Vector3& posA, const float& rA, const Vector3& posB,float rB)
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