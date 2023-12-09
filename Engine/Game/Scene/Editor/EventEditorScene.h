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

	//制作中のバトルイベントのエネミーの描画用構造体
	struct EventEnemyData
	{
		std::vector<Object3D> enemys;
		std::vector<std::string> enemyTypes;
		std::vector<Object3D> endPoint;
		std::vector<Object3D> move;
		Object3D playerPoint;

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

private:

	//設定してイベントを追加するところ
	void AddEvent();

	//追加するときの各イベント用処理
	void AddMoveEvent();
	void AddBattleEvent();
	void AddBattleEventEnemy();

	//Addのボタンが押された時の処理
	void AddButtonBattleEvent();
	void AddButtonBattleEventDebugObj();

	//デバック用のオブジェクトの登録関数
	void AddEventDebugObj();
	void AddMoveEventDebugObj();
	void AddBattleEventDebugObj();

	//現在入っているイベントを編集したり消すところ
	void EditEvent();

	//イベントデータを保存したり読み込んだりするところ
	void SaveAndLoadEvent();

	//エディタシーンのマップを変更できるところ
	void ChangeMap();

	//制作中のイベントの描画用のオブジェクトの更新用の関数
	void DrawEventDataUpdate();

	//イベントデータを書き出し
	void SaveEventData(const std::string fileName);

	//フルパスでのファイルの書き込み
	void SaveEventFullPathData(const std::string fileName);

	//現在制作中のイベントを動かす
	void TestEvent();

	//デバック用の更新とか処理
	void DebugUpdate();

	//windowsの機能を使ってeefmファイルを保存
	void WindowsSaveEEFMFile();

	//windowsの機能を使ってeefmファイルを開く
	bool WindowsOpenEEFMFile();

	//開くを使うための関数(イベントマネージャーにもあるがこっちのシーンで編集するためにこちらで読み込みたいので)

	//フルパスで指定された場所にあるイベントのデータが入っているファイルからデータを取得(フォルダ名に日本語が入っていると無理)
	bool LoadFullPathEventData(std::string fileName);

	//イベントの中身読み込むよう
	bool EventScanning(nlohmann::json deserialized, nlohmann::json& Event);

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

	Player player_;

	//モデル
	AnimationModel* testModel_ = nullptr;

	//出力用のイベントの設定
	std::vector<EventSeting> seting_;

	//種類設定用変数
	uint32_t eventTypeNum_ = 0;
	std::vector<uint32_t> enemyTypeNum_;
	uint32_t playerHideTypeNum_ = 0;

	

	//作成中のイベントの中身保持用
	std::vector<EventEnemyData> enemyDatas_;
	std::vector<EventMovePointData> movePointDatas_;

	//移動イベントのどういう風に動くかのためのタイマー
	float moveEventMoveTimer = 0;
	float moveEventMoveMaxTime = 50;

	//描画用モデル
	AnimationModel* enemyModel_ = nullptr;
	AnimationModel* moveEventModel_ = nullptr;

	//イベントの中身設定用変数
	float addTime_ = 0;
	
	//移動イベント用
	uint16_t eventType_ = EventType::moveEvent;
	float movePoint_[3] = {0,0,0};
	float movePointRot_[3] = { 0,0,0 };
	float moveStartPoint_[3] = { 0,0,0 };
	float moveSpeed_ = 1;
	float moveRotTime_ = 1;

	//バトルイベント用
	int32_t enemyNum_ = 0;
	int32_t enemyMaxSpawn_ = 1;
	std::vector<Vector3> enemySpawnPos_;
	std::vector<Vector3> enemyMovePos_;
	std::vector<float> enemyMoveSpeed_;
	std::vector<float> enemySpawnInterval_;
	std::vector<uint32_t> enemyBulletCT_;
	std::vector<std::string> enemyTypes_;
	float playerHideType_ = playerHideVectorType::Down;
	float playerPos_[3] = { 0,0,0 };

	//イベントの種類
	const char* EventTypeChar[2] = { "moveEvent","BattleEvent" };
	//敵の種類
	const char* EnemyTypeChar[3] = { "moveOnly","moveAttack","Attack" };

	const char* playerHideTypeChar[2] = { "Down","Right" };

	//jsonファイル書き出し用のファイル名を入れるところ
	char str1[128] = "";

	//テスト中か
	bool isTest_ = false;

	//一時停止しているか
	bool pause_ = false;

	//ちゃんとロードできた?
	bool isLoad_ = true;

	//エラーが出た時の出力先
	std::string loadErrorText_ = "";

	uint32_t eventNum_ = 1;
};