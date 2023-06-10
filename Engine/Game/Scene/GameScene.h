#pragma once

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



#include <DirectXTex.h>
#include <imgui.h>



#include "WinApp.h"

#include "DirectXInit.h"

#include "ImGuiManager.h"

#include <dxgidebug.h>

#include "Input.h"

#include "IScene.h"

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

#include "JsonLevelLoader.h"

#include <sstream>

#include <cstdint>

class GameScene:public IScene
{
public:

	void Initialize()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

private:

	bool CollsionSphere(Vector3 posA, float rA, Vector3 posB, float rB);

private:

	//windowAPI
	std::unique_ptr<WinApp> winApp = std::make_unique<WinApp>();

	

	//directXの初期化
	DirectXInit* directXinit = DirectXInit::GetInstance();

	ImGuiManager imGuiManager;

	//入力の初期化
	Input* input = input->GetInstance();

	//ライト
	LightGroup* lightGroup = nullptr;


	//obj
	Object3D charactorObj;
	Object3D charactorObj2;
	Object3D charactorObj3;

	Object3D objobj;
	Object3D objobj2;
	Object3D objobj3;

	//カメラ
	Camera camera;
	cameraObj cameobj;

	uint32_t texname = 0;

	//当たり判定チェック
	Sphere sphere;
	Plane plane;
	Triangle triangle;
	Ray ray;


	//スプライト
	SpriteCommon spritecommon;

	Sprite sprite;
	Sprite sprite2;


	Vector3 lightDir;

	float ambientColor0[3] = { 1,1,1 };

	float lightDir0[3] = { 1,0,0 };
	float lightColor0[3] = { 0.9f,0.7f,0.7f };

	float lightDir1[3] = { 1,0,0 };
	float lightColor1[3] = { 0,0,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,0 };

	Vector3 movecoll;

	Vector3 cameraPos = { 0,20,-200 };
	Vector3 cameraRot;

	Vector3 spriteMove = { 50 ,200 ,0 };

	player play;
};

