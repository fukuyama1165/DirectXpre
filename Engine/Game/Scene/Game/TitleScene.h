#pragma once
#include "IScene.h"
#include "Object3D.h"
#include "cameraObj.h"
#include "Sprite.h"
#include "Player.h"
#include "EventPointManager.h"
#include "JsonLevelLoader.h"
#include "ModelManager.h"
#include "Wall.h"
#include "EnemyManager.h"
#include "LightManager.h"

class TitleScene : public IScene
{
public:
	TitleScene();
	~TitleScene();

	void Initialize()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

private:

	//デバック用の設定とかいじるところを書くところ
	void ImguiUpdate();


private:

	//ライト
	LightManager* lightManager_;

	//天球
	Object3D objobj3_;

	cameraObj cameobj_;

	std::string textureNum_ = "";

	Sprite title_;

	bool debugMenu_ = false;

	Player play_;

	EnemyManager* enemys_ = nullptr;

	//レベル読み込み用
	struct LevelObj
	{
		Object3D obj;
		std::string name;
	};

	struct LevelWallObj
	{
		WallObj obj;
		std::string name;
	};

	std::vector<LevelObj> levelObj;
	std::vector<std::unique_ptr<LevelWallObj>> wallObj_;

	//モデル
	AnimationModel* testModel_ = nullptr;

	AnimationModel* levelModel_ = nullptr;
	AnimationModel* levelBallModel_ = nullptr;
	AnimationModel* levelGroundModel_ = nullptr;
	AnimationModel* levelBuildingModel_ = nullptr;


	EventPointManager* eventManager_ = nullptr;

	//開始の合図
	bool startFlag_ = false;

	//イベントの敵の数
	uint16_t EventEnemyCount_ = 0;

	//次に行くときの待ち時間
	float EventWaitTimer_ = 0;
	float EventWait_ = 20;

	//デバック用のカメラの位置
	Vector3 cameraPos_ = { 0,20,-200 };

	bool chengCamera_ = true;
};
