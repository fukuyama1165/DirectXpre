#pragma once
#include "IScene.h"

#include<Windows.h>

//D3Dコンパイラ
#include<d3dcompiler.h>

#include<vector>
#include<string>

#include<d3d12.h>
#include<dxgi1_6.h>

#include <imgui.h>

#include "ImGuiManager.h"

#include <dxgidebug.h>

#include "Input.h"
#include "XAudio.h"

#include "Object3D.h"

#include "Sprite.h"

#include "camera.h"
#include "Texture.h"
#include "Player.h"

#include "cameraObj.h"
#include "EnemyManager.h"

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

	void reloadLevel(const BYTE& CheckKey,std::string filename);

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

	std::unique_ptr<AnimationModel> levelModel_;
	std::unique_ptr<AnimationModel> levelBallModel_;

	//カメラ
	Camera camera_;
	cameraObj cameobj_;
	cameraObj debugCameobj_;
	cameraObj playerCameobj_;

	uint32_t texname_ = 0;

	//当たり判定チェック
	Sphere sphere_;
	Plane plane_;
	Triangle triangle_;
	Ray ray_;

	bool IsUseCameraMouse_ = false;

	//スプライト
	SpriteCommon* spritecommon_;

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

	float pointPos[3] = { 0.0f,0.0f,0.0f };
	Vector3 pointLightPos_ = { 0.5f,1.0f,2.0f };
	float pointLightColor_[3] = { 1.0f,1.0f,1.0f };
	float pointLightAtten_[3] = { 0.3f,0.1f,0.1f };

	float spotPos_[3] = { 0.0f,-5.0f,0.0f };
	float spotDir_[3] = { 0.0f,5.0f,0.0f };
	Vector3 spotLightPos_ = { 0.5f,1.0f,2.0f };
	float spotLightColor_[3] = { 1.0f,1.0f,1.0f };
	float spotLightAtten_[3] = { 1.0f,2.0f,4.0f };
	float spotLightFactorAngle_[2] = { 20.0f,30.0f};

	Vector3 movecoll_;

	Vector3 cameraPos_ = { 0,20,-200 };
	Vector3 cameraRot_;

	Vector3 spriteMove_ = { 50 ,200 ,0 };

	Player play_;

	EnemyManager enemys_;

	XAudio* xAudio_;

	std::string test_;

	struct LevelObj
	{
		Object3D obj;
		std::string name;
	};

	std::vector<LevelObj> levelObj;

	bool chengCamera_ = false;

	

};


