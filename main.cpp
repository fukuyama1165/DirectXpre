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

//#include "DrawingObj.h"

#include "WinApp.h"

#include "DirectXInit.h"

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
#include "Collison.h"

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

	//入力の初期化
	Input* input = input->GetInstance();

	input->init(winApp->getW(), winApp->getHwnd());

	LightGroup::Staticlnitialize(directXinit->Getdev().Get());

	LightGroup* lightGroup = nullptr;

	//ライトの生成
	lightGroup = LightGroup::Create();

	lightGroup->SetLightColor({ 1,1,1 });

	Object3D::SetLight(lightGroup);

	//obj
	Object3D charactorObj;
	Object3D charactorObj2;

	Object3D objobj;
	Object3D objobj2;

	Camera camera(winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());
	camera.eye_ = { 0,0,-1000 };

	cameraObj cameobj(winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());

	cameobj.pos = { 0,0,-200 };
	cameobj.rotate = { 0,0,0 };

	player play;
	//.objのオブジェクト
	//DrawOBJ test(winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());


	//charactorObj.colorChangeInit(directXinit->Getdev().Get());
	charactorObj2.basicInit(directXinit->Getdev().Get());

	int texname = 0;

	texname = charactorObj2.loadTexture("Resources/hokehoke.png");

	play.Init(directXinit->Getdev().Get(), "Resources/obj/karaage/", "karaage.obj");
	
	int texname2 = 0;
	int texname3 = 0;

	Sphere sphere;
	Plane plane;
	Triangle triangle;
	
	sphere.center = { 0,2,0,1 };
	sphere.radius = 1.0f;

	plane.normal = { 0,1,0,0 };
	plane.distance = 0.0f;

	triangle.p0 = { -1.0f,0.0f,-1.0f,1 };
	triangle.p1 = { -1.0f,0.0f,1.0f,1 };
	triangle.p2 = { 1.0f,0.0f,-1.0f,1 };
	triangle.normal = { 0.0f,1.0f,0.0f,0 };

	//eenemy.PopEnemy(directXinit->Getdev().Get(), { 0,0,0 });

	
	//texname = charactorObj2.loadTexture("Resources/player_shade.png");
	//test.basicInit((directXinit->Getdev().Get()));

	//objobj.objDrawInit(directXinit->Getdev().Get(), "Resources/obj/karaage/", "karaage.obj");
	//objobj2.objDrawInit(directXinit->Getdev().Get(), "Resources/obj/100t/", "100t.obj");

	SpriteCommon spritecommon;

	spritecommon.initialize(directXinit->Getdev().Get());

	Sprite sprite;

	sprite.initialize(directXinit->Getdev().Get(), &spritecommon);

	

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

	charactorObj2.SetPos({ 100,0,0 });
	objobj.SetPos({ -100,0,0 });

	Vector3 lightDir;

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

		if (input->PushKey(DIK_I))
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
		}

		
	
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

		Vector4 moveY(0, 0.01f, 0, 0);
		if (input->PushKey(DIK_W))
		{
			sphere.center += moveY;
		}
		if (input->PushKey(DIK_S))
		{
			sphere.center -= moveY;
		}
		Vector4 moveX(0.01f, 0, 0, 0);
		if (input->PushKey(DIK_D))
		{
			sphere.center += moveX;
		}
		if (input->PushKey(DIK_A))
		{
			sphere.center -= moveX;
		}

		bool hit = Collison::CheckSphere2Triangle(sphere, triangle);

		lightGroup->SetLightDir({ lightDir.x,lightDir.y ,lightDir.z });

		cameobj.upDate();
		play.Update(cameobj);
		lightGroup->Update();
		
		sprite.Update();

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


		//charactorObj.Draw(directXinit->GetcmdList().Get(), PipeLineRuleFlag, ChangeSquareFlag);
		//charactorObj2.Draw(directXinit->GetcmdList().Get(), 3, PipeLineRuleFlag);
		//objobj.Draw(directXinit->GetcmdList().Get());
		//objobj2.Draw(directXinit->GetcmdList().Get());

		//test.Draw(directXinit->GetcmdList().Get(), PipeLineRuleFlag, true, true);

		
		//sprite.Draw(directXinit->GetcmdList().Get());
		
		
		play.Draw(directXinit->GetcmdList().Get());
			
	
		// 4.描画コマンドここまで

#pragma endregion

		directXinit->DrawEnd();

		//DirectX毎フレーム処理　ここまで

#pragma endregion

		std::wstringstream stream;
		std::wstringstream hits;

		stream << sphere.center.x << " x" << std::endl;
		
		OutputDebugString(stream.str().c_str());
		stream << sphere.center.y << " y" << std::endl;
		
		OutputDebugString(stream.str().c_str());
		stream << sphere.center.z << " z" << std::endl;

		OutputDebugString(stream.str().c_str());
		if (hit)
		{
			hits << " hit" << std::endl;
			OutputDebugString(hits.str().c_str());
		}

#pragma region 描画処理

		//charactorObj.colorMap(Red, Green, Blue);

#pragma endregion



	}

	delete winApp;

	//texture.hを読み込んでいるobjctが増えてもdelteするのは一回にすること
	charactorObj.deleteTexture();

	input->instanceDelete();

	delete(lightGroup);

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

