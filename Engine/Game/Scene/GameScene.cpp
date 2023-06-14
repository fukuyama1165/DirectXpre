#include "GameScene.h"

void GameScene::Initialize()
{
	
	IScene::Initialize();

	

	LightGroup::Staticlnitialize();

	//ライトの生成
	lightManager = LightManager::GetInstance();

	lightManager->CreateLightGroup();

	//lightGroup->SetLightColor({ 1,1,1 });

	Object3D::SetLight(&lightManager->lightGroups_[0]);

	camera = Camera((float)WinApp::GetInstance()->getWindowSizeWidth(), (float)WinApp::GetInstance()->getWindowSizeHeight());

	camera.eye_ = { 0.0f,0.0f,-1000.0f };

	cameobj = cameraObj((float)WinApp::GetInstance()->getWindowSizeWidth(), (float)WinApp::GetInstance()->getWindowSizeHeight());

	//cameobj.cameobj.SetParent(&objobj);

	
	//.objのオブジェクト
	//DrawOBJ test(winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());


	charactorObj.colorChangeInit();
	charactorObj3.basicInit();
	charactorObj2.colorChangeInit();

	

	texname = charactorObj2.loadTexture("Resources/hokehoke.png");

	play.Init("Resources/obj/karaage/", "karaage.obj");

	
	sphere.center_ = { 0,0,0 };
	sphere.radius_ = 1.0f;

	plane.normal_ = { 0,1,0 };
	plane.distance_ = 0.0f;

	triangle.p0_ = { -50.0f,-50.0f,50.0f };
	triangle.p1_ = { 50.0f,-50.0f,50.0f };
	triangle.p2_ = { -50.0f, 50.0f,50.0f };
	triangle.ComputeNormal();
	//triangle.normal = { 0.0f,0.0f,-1.0f,0 };

	ray.start_ = { 0.0f,0.0f,0.0f };
	ray.dir_ = { 0.0f,0.0f,-1.0f };

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

	objobj.objDrawInit("Resources/obj/testBall/", "testBall.obj", true);
	objobj2.objDrawInit("Resources/obj/collHittest/", "collHitTest.obj");
	objobj3.objDrawInit("Resources/obj/skydome/", "skydome.obj");

	objobj.Scale_ = { 0.95f,0.95f,0.95f };
	objobj2.Scale_ = { 0.95f,0.95f,0.95f };

	

	spritecommon.initialize();

	sprite.initialize(&spritecommon, 2);
	sprite2.initialize(&spritecommon, 1);

	sprite.posX_ = 100;
	sprite.posY_ = 200;

	sprite2.posX_ = 50;
	sprite2.posY_ = 200;

	objobj.SetPos({ -100,0,0 });
	objobj3.SetScale({ 1000,1000,1000 });

	
	charactorObj3.SetScale({ 0.001f,0.001f,100.0f });

}

void GameScene::Finalize()
{
	charactorObj.deleteTexture();

	
	//delete(levelData);


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
		lightDir.z = 1;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN))
	{
		lightDir.z = -1;
	}
	if (Input::GetInstance()->PushKey(DIK_RIGHT))
	{
		lightDir.x = 1;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT))
	{
		lightDir.x = -1;
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
		movecoll.y += moveY.y;
	}
	if (Input::GetInstance()->PushKey(DIK_S))
	{
		movecoll.y -= moveY.y;
	}
	Vector4 moveX(0.01f, 0, 0, 0);
	if (Input::GetInstance()->PushKey(DIK_D))
	{
		movecoll.x += moveX.x;
	}
	if (Input::GetInstance()->PushKey(DIK_A))
	{
		movecoll.x -= moveX.x;
	}

	if (Input::GetInstance()->PushKey(DIK_Z))
	{
		movecoll.z += 0.01f;
	}

	if (Input::GetInstance()->PushKey(DIK_X))
	{
		movecoll.z -= 0.01f;
	}

	sphere.center_ = { movecoll.x ,movecoll.y,movecoll.z };

	//ray.start = { movecoll.x ,movecoll.y,movecoll.z};

	sprite2.posX_ = spriteMove.x;
	sprite2.posY_ = spriteMove.y;

	bool hit = Collision::CheckRay2Sphere(ray, sphere);

	lightManager->lightGroups_[0].SetAmbientColor(XMFLOAT3(ambientColor0));

	lightManager->lightGroups_[0].SetDirLightDir(0, { lightDir0[0],lightDir0[1] ,lightDir0[2],0 });
	lightManager->lightGroups_[0].SetDirLightColor(0, { lightColor0[0],lightColor0[1] ,lightColor0[2] });

	lightManager->lightGroups_[0].SetDirLightDir(1, { lightDir1[0],lightDir1[1] ,lightDir1[2],0 });
	lightManager->lightGroups_[0].SetDirLightColor(1, { lightColor1[0],lightColor1[1] ,lightColor1[2] });

	lightManager->lightGroups_[0].SetDirLightDir(2, { lightDir2[0],lightDir2[1] ,lightDir2[2],0 });
	lightManager->lightGroups_[0].SetDirLightColor(2, { lightColor2[0],lightColor2[1] ,lightColor2[2] });
	//lightGroup->SetDirLightDir(2,{ lightDir2.x,lightDir2.y ,lightDir2.z });

	if (IsUseCameraMouse)
	{
		if (!Input::GetInstance()->PushKey(DIK_LCONTROL))
		{
			WinApp::GetInstance()->SetMousePos(WinApp::SWindowWidth_ / 2, WinApp::SWindowHeight_ / 2);
		}

		cameraRot.y += Input::GetInstance()->GetMouseMove().x / 1000;
		cameraRot.x += Input::GetInstance()->GetMouseMove().y / 1000;
	}

	if (Input::GetInstance()->TriggerKey(DIK_F5))
	{
		IsUseCameraMouse = !IsUseCameraMouse;
	}

#pragma region imgui_Light

	ImGui::Begin("Light");
	//ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 200));

	ImGui::ColorEdit3("ambientColor", ambientColor0, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("lightDir0", lightDir0);
	ImGui::ColorEdit3("lightColor0", lightColor0, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("lightDir1", lightDir1);
	ImGui::ColorEdit3("lightColor1", lightColor1, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("lightDir2", lightDir2);
	ImGui::ColorEdit3("lightColor2", lightColor2, ImGuiColorEditFlags_Float);

	ImGui::End();

#pragma endregion

#pragma region Collision_camera

	ImGui::Begin("Collision");

	//ImGui::SetWindowSize(ImVec2(300, 300));

	ImGui::Text("move:WASDZX");

	ImGui::SliderFloat("movecollx", &movecoll.x, -200.0f, 200.0f, "%.3f");
	ImGui::SliderFloat("movecolly", &movecoll.y, -200.0f, 200.0f, "%.3f");
	ImGui::SliderFloat("movecollz", &movecoll.z, -200.0f, 200.0f, "%.3f");

	ImGui::Text("x:%f y:%f z:%f", ray.start_.x, ray.start_.y, ray.start_.z);

	if (hit)
	{
		ImGui::Text("hit");
	}
	if (ImGui::CollapsingHeader("camera"))
	{

		ImGui::SliderFloat("cameraX", &cameraPos.x, -500.0f, 500.0f, "%.3f");
		ImGui::SliderFloat("cameraY", &cameraPos.y, -500.0f, 500.0f, "%.3f");
		ImGui::SliderFloat("cameraZ", &cameraPos.z, -500.0f, 500.0f, "%.3f");

		ImGui::SliderFloat("cameraRotX", &cameraRot.x, -5.0f, 5.0f, "%.3f");
		ImGui::SliderFloat("cameraRotY", &cameraRot.y, -5.0f, 5.0f, "%.3f");
		ImGui::SliderFloat("cameraRotZ", &cameraRot.z, -5.0f, 5.0f, "%.3f");

		ImGui::Text("reset");

		if (ImGui::Button("posX"))
		{
			cameraPos.x = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("posY"))
		{
			cameraPos.y = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("posZ"))
		{
			cameraPos.z = -200;
		}
		if (ImGui::Button("rotX"))
		{
			cameraRot.x = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("rotY"))
		{
			cameraRot.y = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("rotZ"))
		{
			cameraRot.z = 0;
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

		ImGui::Checkbox("useMouseCamera", &IsUseCameraMouse);

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

	cameobj.pos_ = cameraPos;
	cameobj.rotate_ = cameraRot;

	objobj.SetPos(movecoll);
	objobj2.SetPos(movecoll);

	cameobj.upDate();
	play.Update(cameobj);
	lightManager->lightGroups_[0].Update();

	sprite.Update();
	sprite2.Update();

	charactorObj.Update(cameobj.GetCamera());
	charactorObj2.Update(cameobj.GetCamera());
	charactorObj3.Update(cameobj.GetCamera());

	objobj.Update(cameobj.GetCamera());
	objobj2.Update(cameobj.GetCamera());
	objobj3.Update(cameobj.GetCamera());

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
	//charactorObj3.Draw(4,1,1);
	//charactorObj2.Draw(4, 1,0);
	/*if (hit == false)
	{*/
	objobj.Draw();
	/*}
	else
	{
		objobj2.Draw();
	}*/

	objobj3.Draw();

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