#include<Windows.h>

//D3Dコンパイラ
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<vector>
#include<string>

#include<d3d12.h>
#include<dxgi1_6.h>

//数学ライブラリ(最適化されすぎて使いにくいらしいのであとで自作しろって)
#include<DirectXMath.h>
using namespace DirectX;

//#pragma commentとは、オブジェクトファイルに、
//リンカでリンクするライブラリの名前を記述するもの
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

//キーボードやコントローラーなどの入力するヘッダとライブラリのリンク
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

//const float PI = 3.141592653589f;

#include <DirectXTex.h>
#include <imgui.h>

//#include "DrawingObj.h"

#include "WinApp.h"

#include "DirectXInit.h"

#include "ImGuiManager.h"

#include <dxgidebug.h>

#include "Input.h"

//#include "DrawOBJ.h"
#include "Object3D.h"

#include "Sprite.h"

#include "camera.h"
#include "Texture.h"
#include "player.h"
#include "enemy.h"

#include "cameraObj.h"
#include "enemyManager.h"

#include "LightGroup.h"
#include "CollisionPrimirive.h"
#include "Collision.h"

#include <sstream>

#pragma region ウィンドウプロシージャ


//LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
//{
//	switch (msg)
//	{
//	case WM_DESTROY://ウィンドウが破棄された
//		PostQuitMessage(0);//OSに対して、終わったことを伝える
//		return 0;
//	}
//	return DefWindowProc(hwnd, msg, wparam, lparam);//標準の処理をする
//}

#pragma endregion


//デバイス発見時の実行される
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}


/// <summary>
/// 二次元のアフィン変換をする関数
/// </summary>
/// <param name="box">頂点データ(X,Y)</param>
/// <param name="box">図形の中の原点(X,Y)</param>
/// <param name="moveX">x方向の移動量</param>
/// <param name="moveY">y方向の移動量</param>
/// <param name="rotate">回転角度(度数法)</param>
/// <param name="scaleX">x方向の拡大率</param>
/// <param name="scaleY">y方向の拡大率</param>
XMFLOAT3 Afin(XMFLOAT3 box, XMFLOAT3 box2, float moveX, float moveY, float rotate, float scaleX, float scaleY);

bool CollsionSphere(Vector3 posA, float rA, Vector3 posB, float rB);


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//windowAPI
	WinApp* winApp;

	winApp = new WinApp();

	winApp->initialize();

	//directXの初期化
	DirectXInit* directXinit;

	directXinit = DirectXInit::GetInstance();

	directXinit->Init(winApp->getW(), winApp->getHwnd(), winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());

	ImGuiManager imGuiManager;

	imGuiManager.Init(winApp);

	//入力の初期化
	Input* input = input->GetInstance();

	input->init(winApp->getW(), winApp->getHwnd());

	LightGroup::Staticlnitialize(directXinit->Getdev().Get());

	LightGroup* lightGroup = nullptr;

	//ライトの生成
	lightGroup = LightGroup::Create();

	//lightGroup->SetLightColor({ 1,1,1 });

	Object3D::SetLight(lightGroup);

	//obj
	Object3D charactorObj;
	Object3D charactorObj2;
	Object3D charactorObj3;

	Object3D objobj;
	Object3D objobj2;
	Object3D objobj3;

	Camera camera((float)winApp->getWindowSizeWidth(), (float)winApp->getWindowSizeHeight());
	camera.eye_ = { 0.0f,0.0f,-1000.0f };

	cameraObj cameobj((float)winApp->getWindowSizeWidth(), (float)winApp->getWindowSizeHeight());

	//cameobj.cameobj.SetParent(&objobj);

	player play;
	//.objのオブジェクト
	//DrawOBJ test(winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());


	charactorObj.colorChangeInit();
	charactorObj3.basicInit();
	charactorObj2.colorChangeInit();

	int texname = 0;

	texname = charactorObj2.loadTexture("Resources/hokehoke.png");

	play.Init(directXinit->Getdev().Get(), "Resources/obj/karaage/", "karaage.obj");
	
	int texname2 = 0;
	int texname3 = 0;

	Sphere sphere;
	Plane plane;
	Triangle triangle;
	Ray ray;
	
	sphere.center = { 0,0,0};
	sphere.radius = 1.0f;

	plane.normal = { 0,1,0};
	plane.distance = 0.0f;

	triangle.p0 = { -50.0f,-50.0f,50.0f};
	triangle.p1 = { 50.0f,-50.0f,50.0f};
	triangle.p2 = { -50.0f, 50.0f,50.0f};
	triangle.ComputeNormal();
	//triangle.normal = { 0.0f,0.0f,-1.0f,0 };

	ray.start = { 0.0f,0.0f,0.0f};
	ray.dir = { 0.0f,0.0f,-1.0f};

	//eenemy.PopEnemy(directXinit->Getdev().Get(), { 0,0,0 });

	
	//texname = charactorObj2.loadTexture("Resources/player_shade.png");
	//test.basicInit((directXinit->Getdev().Get()));

	objobj.objDrawInit("Resources/obj/testcoll/", "testcoll.obj");
	objobj2.objDrawInit("Resources/obj/collHittest/", "collHitTest.obj");
	objobj3.objDrawInit("Resources/obj/skydome/", "skydome.obj");

	objobj.Scale_ = { 0.95f,0.95f,0.95f };
	objobj2.Scale_ = { 0.95f,0.95f,0.95f };

	SpriteCommon spritecommon;

	spritecommon.initialize(directXinit->Getdev().Get());

	Sprite sprite;
	Sprite sprite2;

	sprite.initialize(directXinit->Getdev().Get(), &spritecommon,2);
	sprite2.initialize(directXinit->Getdev().Get(), &spritecommon,1);

	sprite.posX = 100;
	sprite.posY = 200;

	sprite2.posX = 50;
	sprite2.posY = 200;

	//パイプラインステート切り替え用フラグ
	bool PipeLineRuleFlag = true;

	//四角形に変更するときのフラグ
	bool ChangeSquareFlag = true;

	bool ChangeTexure = false;


	float Red = 1.0f;
	float Green = 0.0f;
	float Blue = 0.0f;

	float X1 = 0.0f;
	float X2 = 0.0f;
	float Y1 = 0.0f;
	float Y2 = 0.0f;
	float rotate = 0;
	float scaleX = 1;
	float scaleY = 1;

	float angle = 0.0f;//カメラの回転角
	float angleY = 0.0f;//カメラの回転角

	float rotateX = 0;
	float rotateY = 0;

	int time = 0;

	int scene = 0;

	//座標
	Vector3 pos = {};

	//作ったのが小さかったので
	/*test.SetScale({ 20,20,0 });
	test.obj3DUpdate();*/

	//charactorObj2.SetPos({ 100,0,0 });
	objobj.SetPos({ -100,0,0 });
	objobj3.SetScale({ 1000,1000,1000 });

	//charactorObj3.SetRotate({ PI/2,0,0});
	//charactorObj3.SetScale({ 0.1f,0.001f,0.1f});
	charactorObj3.SetScale({ 0.001f,0.001f,100.0f});

	Vector3 lightDir;
	//Vector3 lightDir2;

	float ambientColor0[3] = { 1,1,1 };

	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };
	
	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	Vector3 movecoll;

	Vector3 cameraPos = {0,20,-200};
	Vector3 cameraRot;
	
	Vector3 spriteMove = { 50 ,200 ,0 };

	//ゲームループ
	while (true)
	{

		if (winApp->processMassage() or input->TriggerKey(DIK_ESCAPE))
		{

			break;

		}

#pragma region DirectX毎フレーム処理

		//DirectX毎フレーム処理　ここから

#pragma region キーボード情報の取得

		input->update();

#pragma endregion

#pragma region 更新処理

		imGuiManager.Begin();

		

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

		
	
		if (input->PushKey(DIK_UP))
		{
			lightDir.z = 1;
		}
		if (input->PushKey(DIK_DOWN))
		{
			lightDir.z = -1;
		}
		if (input->PushKey(DIK_RIGHT))
		{
			lightDir.x = 1;
		}
		if (input->PushKey(DIK_LEFT))
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
		if (input->PushKey(DIK_W))
		{
			movecoll.y += moveY.y;
		}
		if (input->PushKey(DIK_S))
		{
			movecoll.y -= moveY.y;
		}
		Vector4 moveX(0.01f, 0, 0, 0);
		if (input->PushKey(DIK_D))
		{
			movecoll.x += moveX.x;
		}
		if (input->PushKey(DIK_A))
		{
			movecoll.x -= moveX.x;
		}

		if (input->PushKey(DIK_Z))
		{
			movecoll.z += 0.01f;
		}

		if (input->PushKey(DIK_X))
		{
			movecoll.z -= 0.01f;
		}

		sphere.center = { movecoll.x ,movecoll.y,movecoll.z};

		//ray.start = { movecoll.x ,movecoll.y,movecoll.z};

		sprite2.posX = spriteMove.x;
		sprite2.posY = spriteMove.y;

		bool hit = Collision::CheckRay2Sphere(ray, sphere);

		lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));

		lightGroup->SetDirLightDir(0,{ lightDir0[0],lightDir0[1] ,lightDir0[2],0 });
		lightGroup->SetDirLightColor(0,{ lightColor0[0],lightColor0[1] ,lightColor0[2]});

		lightGroup->SetDirLightDir(1, { lightDir1[0],lightDir1[1] ,lightDir1[2],0 });
		lightGroup->SetDirLightColor(1, { lightColor1[0],lightColor1[1] ,lightColor1[2] });

		lightGroup->SetDirLightDir(2, { lightDir2[0],lightDir2[1] ,lightDir2[2],0 });
		lightGroup->SetDirLightColor(2, { lightColor2[0],lightColor2[1] ,lightColor2[2] });
		//lightGroup->SetDirLightDir(2,{ lightDir2.x,lightDir2.y ,lightDir2.z });

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

		ImGui::Text("x:%f y:%f z:%f", ray.start.x, ray.start.y, ray.start.z);

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

		/*ImGui::SetNextWindowSize(ImVec2(300, 300));

		ImGui::Begin("input");

		if (input->PushKey(DIK_A))
		{
			ImGui::Text("A");
		}
		if (input->PushKey(DIK_B))
		{
			ImGui::Text("B");
		}
		if (input->PushKey(DIK_C))
		{
			ImGui::Text("C");
		}
		if (input->PushKey(DIK_D))
		{
			ImGui::Text("D");
		}
		if (input->PushKey(DIK_E))
		{
			ImGui::Text("E");
		}
		if (input->PushKey(DIK_F))
		{
			ImGui::Text("F");
		}
		if (input->PushKey(DIK_G))
		{
			ImGui::Text("G");
		}
		if (input->PushKey(DIK_H))
		{
			ImGui::Text("H");
		}
		if (input->PushKey(DIK_I))
		{
			ImGui::Text("I");
		}
		if (input->PushKey(DIK_J))
		{
			ImGui::Text("J");
		}
		if (input->PushKey(DIK_K))
		{
			ImGui::Text("K");
		}
		if (input->PushKey(DIK_L))
		{
			ImGui::Text("L");
		}
		if (input->PushKey(DIK_M))
		{
			ImGui::Text("M");
		}
		if (input->PushKey(DIK_N))
		{
			ImGui::Text("N");
		}
		if (input->PushKey(DIK_O))
		{
			ImGui::Text("O");
		}
		if (input->PushKey(DIK_P))
		{
			ImGui::Text("P");
		}
		if (input->PushKey(DIK_Q))
		{
			ImGui::Text("Q");
		}
		if (input->PushKey(DIK_R))
		{
			ImGui::Text("R");
		}
		if (input->PushKey(DIK_S))
		{
			ImGui::Text("S");
		}
		if (input->PushKey(DIK_T))
		{
			ImGui::Text("T");
		}
		if (input->PushKey(DIK_U))
		{
			ImGui::Text("U");
		}
		if (input->PushKey(DIK_X))
		{
			ImGui::Text("X");
		}
		if (input->PushKey(DIK_Y))
		{
			ImGui::Text("Y");
		}
		if (input->PushKey(DIK_Z))
		{
			ImGui::Text("Z");
		}

		if (input->PushKey(DIK_UP))
		{
			ImGui::Text("Up");
		}

		if (input->PushKey(DIK_DOWN))
		{
			ImGui::Text("Down");
		}

		if (input->PushKey(DIK_RIGHT))
		{
			ImGui::Text("Right");
		}

		if (input->PushKey(DIK_LEFT))
		{
			ImGui::Text("Left");
		}

		if (input->PushKey(DIK_RETURN))
		{
			ImGui::Text("Return");
		}

		if (input->PushKey(DIK_SPACE))
		{
			ImGui::Text("Space");
		}

		if (input->PushKey(DIK_LSHIFT))
		{
			ImGui::Text("Lshift");
		}

		if (input->PushKey(DIK_LCONTROL))
		{
			ImGui::Text("Lcontrol");
		}

		ImGui::End();*/

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

		cameobj.pos = cameraPos;
		cameobj.rotate = cameraRot;

		objobj.SetPos(movecoll);
		objobj2.SetPos(movecoll);

		cameobj.upDate();
		play.Update(cameobj);
		lightGroup->Update();
		
		sprite.Update();
		sprite2.Update();

		charactorObj.Update(cameobj.GetCamera());
		charactorObj2.Update(cameobj.GetCamera());
		charactorObj3.Update(cameobj.GetCamera());

		objobj.Update(cameobj.GetCamera());
		objobj2.Update(cameobj.GetCamera());
		objobj3.Update(cameobj.GetCamera());

		imGuiManager.End();

#pragma endregion

		directXinit->DrawStart();

#pragma region 描画コマンド

		// 4.描画コマンドここから

		//ビューポート設定
		//分割分のビューポートを用意(見にくいので減らした)
		D3D12_VIEWPORT viewport{};

		viewport.Width = (float)winApp->getWindowSizeWidth();//横幅
		viewport.Height = (float)winApp->getWindowSizeHeight();//縦幅
		viewport.TopLeftX = 0;//左上X
		viewport.TopLeftY = 0;//左上Y
		viewport.MinDepth = 0.1f;//最少深度(0でよい)
		viewport.MaxDepth = 1.0f;//最大深度(１でよい)

		//コマンドリストに追加
		directXinit->GetcmdList()->RSSetViewports(1, &viewport);


		//シザー矩形設定
		D3D12_RECT scissorrect{};

		scissorrect.left = 0;//切り抜き座標左
		scissorrect.right = scissorrect.left + winApp->getWindowSizeWidth();//切り抜き座標右
		scissorrect.top = 0;//切り抜き座標上
		scissorrect.bottom = scissorrect.top + winApp->getWindowSizeHeight();//切り抜き座標下

		directXinit->GetcmdList()->RSSetScissorRects(1, &scissorrect);

		//プリミティブ形状(三角形リスト)
		directXinit->GetcmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		//charactorObj.Draw(directXinit->GetcmdList().Get(), 0, 1,0);
		charactorObj3.Draw(4,1,1);
		//charactorObj2.Draw(directXinit->GetcmdList().Get(), 4, 1,0);
		if (hit == false)
		{
			objobj.Draw();
		}
		else
		{
			objobj2.Draw();
		}

		objobj3.Draw();

		//test.Draw(directXinit->GetcmdList().Get(), PipeLineRuleFlag, true, true);

		
		
		
		
		//play.Draw(directXinit->GetcmdList().Get());

		//sprite.Draw(directXinit->GetcmdList().Get(), 2);
		//sprite2.Draw(directXinit->GetcmdList().Get());
			
	
		// 4.描画コマンドここまで

		imGuiManager.Draw();

#pragma endregion

		directXinit->DrawEnd();

		//DirectX毎フレーム処理　ここまで

#pragma endregion

		

#pragma region 描画処理

		//charactorObj.colorMap(Red, Green, Blue);

#pragma endregion



	}

	delete winApp;

	//texture.hを読み込んでいるobjctが増えてもdelteするのは一回にすること
	charactorObj.deleteTexture();

	input->instanceDelete();

	delete(lightGroup);

	imGuiManager.Finalize();

	directXinit->instanceDelete();

	return 0;


}


bool CollsionSphere(Vector3 posA, float rA, Vector3 posB, float rB)
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

