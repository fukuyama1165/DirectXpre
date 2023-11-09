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

	//設定してイベントを追加するところ
	void AddEvent();

	//現在入っているイベントを編集したり消すところ
	void EditEvent();

	//制作中のイベントの描画用のオブジェクトの更新用の関数
	void DrawEventDataUpdate();

private:
	//天球
	Object3D objobj3_;

	//カメラ
	cameraObj cameobj_;

	//カメラの位置(保存用)
	Vector3 cameraPos_ = { 0,20,-200 };

	//マウスを使ってカメラを動かすか
	bool IsUseCameraMouse_ = false;

	//イベントマネージャー
	EventPointManager* eventManager_ = nullptr;

	//エネミーマネージャー
	EnemyManager* enemys_ = nullptr;

	//モデル
	AnimationModel* testModel_ = nullptr;

	//出力用のイベントの設定
	std::vector<EventSeting> seting_;

	//種類設定用変数
	uint32_t eventTypeNum_ = 0;
	std::vector<uint32_t> enemyTypeNum_;

	//制作中のバトルイベントのエネミーの描画用構造体
	struct EventEnemyData
	{
		std::vector<Object3D> enemys;
		bool isEnd = false;
	};

	//作成中の移動イベントの描画用構造体
	struct EventMovePointData
	{
		Object3D startPoint;
		Object3D endPoint;
		Object3D move;
		bool isEnd = false;
	};

	//作成中のイベントの中身保持用
	std::vector<EventEnemyData> enemyDatas_;
	std::vector<EventMovePointData> movePointDatas_;

	//移動イベントのどういう風に動くかのためのタイマー
	float moveEventMoveTimer = 0;
	float moveEventMoveMaxTime = 50;

	//描画用モデル
	AnimationModel* enemyModel_;
	AnimationModel* moveEventModel_;

	//イベントの中身設定用変数
	//移動イベント用
	uint16_t eventType_ = EventType::moveEvent;
	float movePoint_[3] = {0,0,0};
	float movePointRot_[3] = { 0,0,0 };
	float moveStartPoint_[3] = { 0,0,0 };
	float moveSpeed_ = 1;

	//バトルイベント用
	uint16_t enemyNum_ = 0;
	uint16_t enemyMaxSpawn_ = 0;
	std::vector<Vector3> enemySpawnPos_;
	std::vector<Vector3> enemyMovePos_;
	std::vector<float> enemyMoveSpeed_;
	std::vector<float> enemySpawnInterval_;
	std::vector<std::string> enemyTypes_;

	//イベントの種類
	const char* EventTypeChar[2] = { "moveEvent","BattleEvent" };
	//敵の種類
	const char* EnemyTypeChar[3] = { "moveOnly","moveAttack","Attack" };
};