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

class EventEditorScene : public IScene
{
public:
	EventEditorScene();
	~EventEditorScene();

	void Initialize()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

private:
	Object3D objobj3_;

	cameraObj cameobj_;

	Vector3 cameraPos_ = { 0,20,-200 };

	bool IsUseCameraMouse_ = false;

	EventPointManager* eventManager_ = nullptr;

	EnemyManager* enemys_ = nullptr;

	//モデル
	AnimationModel* testModel_ = nullptr;

	//出力用のイベントの設定
	std::vector<EventSeting> seting_;

	//種類設定用変数
	uint32_t eventTypeNum_ = 0;
	std::vector<uint32_t> enemyTypeNum_;

	//イベントの中身設定用変数
	uint16_t eventType_ = EventType::moveEvent;
	float movePoint_[3] = {0,0,0};
	float movePointRot_[3] = { 0,0,0 };
	float moveSpeed_ = 1;

	
	uint16_t enemyNum_ = 0;
	uint16_t enemyMaxSpawn_ = 0;
	std::vector<Vector3> enemySpawnPos_;
	std::vector<Vector3> enemyMovePos_;
	std::vector<float> enemyMoveSpeed_;
	std::vector<float> enemySpawnInterval_;
	std::vector<std::string> enemyTypes_;

};