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
#include "XAudio.h"

#include "IScene.h"

#include "Object3D.h"

#include "Sprite.h"

#include "camera.h"
#include "Texture.h"
#include "player.h"
#include "enemy.h"

#include "cameraObj.h"
#include "enemyManager.h"

#include "LightManager.h"
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

	bool CollsionSphere(const Vector3& posA, const float& rA, const Vector3& posB, float rB);

private:

	

	//ライト
	LightManager* lightManager_;


	//obj
	Object3D charactorObj_;
	Object3D charactorObj2_;
	Object3D charactorObj3_;

	Object3D objobj_;
	Object3D objobj2_;
	Object3D objobj3_;

	Object3D testFBX_;

	std::unique_ptr<AnimationModel> testModel_;

	//カメラ
	Camera camera_;
	cameraObj cameobj_;

	uint32_t texname_ = 0;

	//当たり判定チェック
	Sphere sphere_;
	Plane plane_;
	Triangle triangle_;
	Ray ray_;

	bool IsUseCameraMouse_ = false;

	//スプライト
	SpriteCommon spritecommon_;

	Sprite sprite_;
	Sprite sprite2_;


	Vector3 lightDir_;

	float ambientColor0_[3] = { 1,1,1 };

	float lightDir0_[3] = { 1,0,0 };
	float lightColor0_[3] = { 0.9f,0.7f,0.7f };

	float lightDir1_[3] = { 1,0,0 };
	float lightColor1_[3] = { 0,0,0 };

	float lightDir2_[3] = { 1,0,0 };
	float lightColor2_[3] = { 0,0,0 };

	Vector3 movecoll_;

	Vector3 cameraPos_ = { 0,20,-200 };
	Vector3 cameraRot_;

	Vector3 spriteMove_ = { 50 ,200 ,0 };

	player play_;

	XAudio* xAudio_;

	std::string test_;
};

