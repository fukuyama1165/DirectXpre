#pragma once

/**
 * @file EventEditorScene.h
 * @brief イベント作成シーンクラス
 * @author フクヤマ
 * @date 2023_12/29
 */

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
#include "Video.h"
#include "ExplosionObjManager.h"
#include <list>

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

	//作成中の移動イベントの描画用構造体
	struct EventExplosionObjData
	{
		std::vector<Object3D> obj;
		std::vector<Object3D> explosion;
		std::vector<Vector3> endSize;
		bool isEnd = false;
	};

private:

	//設定してイベントを追加するところ
	void AddEvent();

	//追加するときの各イベント用処理
	void AddMoveEvent();
	void AddBattleEvent();
	void AddBattleEventEnemy();
	void AddBattleEventExplosionObj();

	//Addのボタンが押された時の処理
	void AddButtonBattleEvent();
	void AddButtonBattleEventDebugObj();
	void AddButtonBattleEventExplosionObjDebugObj();

	//デバック用のオブジェクトの登録関数
	void AddEventDebugObj();
	void AddMoveEventDebugObj();
	void AddBattleEventDebugObj();
	void AddBattleEventExplosionObjDebugObj();

	//デバックのオブジェクトの中身を編集するのに使う
	void AddBattleEventDebugObjRandomAccess(uint16_t num);
	void AddBattleEventExplosionObjDebugObjRandomAccess(uint16_t num);

	//現在入っているイベントを編集したり消すところ
	void EditEvent();

	//イベントデータを保存したり読み込んだりするところ
	void SaveAndLoadEvent();

	//エディタシーンのマップを変更できるところ
	void ChangeMap();

	//制作中のイベントの描画用のオブジェクトの更新用の関数
	void DrawEventDataUpdate();

	/// <summary>
	/// イベントデータを書き出し
	/// </summary>
	/// <param name="fileName">書き出し用の名前</param>
	void SaveEventData(const std::string& fileName);

	/// <summary>
	/// フルパスでのファイルの書き込み
	/// </summary>
	/// <param name="fileName">ファイルへのフルパス</param>
	void SaveEventFullPathData(const std::string& fileName);

	//現在制作中のイベントを動かす
	void TestEvent();

	//デバック用の更新とか処理
	void DebugUpdate();

	//問答無用でテスト中でもデバックを行うためのデバック用の更新とか処理
	void TestDebugUpdate();

	//windowsの機能を使ってeefmファイルを保存
	void WindowsSaveEEFMFile();

	//すでに持っているファイルパスを使って保存
	void SaveAsEEFMFile();

	//windowsの機能を使ってeefmファイルを開く
	bool WindowsOpenEEFMFile();

	//開くを使うための関数(イベントマネージャーにもあるがこっちのシーンで編集するためにこちらで読み込みたいので)

	/// <summary>
	/// フルパスで指定された場所にあるイベントのデータが入っているファイルからデータを取得(フォルダ名に日本語が入っていると無理)
	/// </summary>
	/// <param name="fileName">ファイルへのフルパス</param>
	/// <returns></returns>
	bool LoadFullPathEventData(const std::string& fileName);

	/// <summary>
	/// イベントの中身読み込むよう
	/// </summary>
	/// <param name="Event">イベントの中身のjson文字列</param>
	/// <returns></returns>
	bool EventScanning(const nlohmann::json& Event);

	/// <summary>
	/// イベントのimguizmoの操作位置を変更するやつ(移動イベントのスタート位置)
	/// </summary>
	/// <param name="count">イベントを保持している変更したい配列の位置</param>
	void EventImguizmoMoveStartPointFlag(const uint32_t& count);

	/// <summary>
	/// イベントのimguizmoの操作位置を変更するやつ(移動イベントの終了位置)
	/// </summary>
	/// <param name="count">イベントを保持している配列の変更したい位置</param>
	void EventImguizmoMovePointFlag(const uint32_t& count);

	/// <summary>
	/// イベントのimguizmoの操作位置を変更するやつ(バトルイベントのプレイヤー位置)
	/// </summary>
	/// <param name="count">イベントを保持している配列の変更したい位置</param>
	void EventImguizmoBattleEventPlayerPoint(const uint32_t& count);

	/// <summary>
	/// イベントのimguizmoの操作位置を変更するやつ(爆発するオブジェクトの位置)
	/// </summary>
	/// <param name="count">イベントを保持している配列の変更したい位置</param>
	/// <param name="ExplosionObjCount">爆発するオブジェクトの配列の位置</param>
	void EventImguizmoExplosionObjPosFlag(const uint32_t& count,const uint32_t& ExplosionObjCount);

	/// <summary>
	/// イベントのimguizmoの操作位置を変更するやつ(敵の位置)
	/// </summary>
	/// <param name="count">イベントを保持している配列の変更したい位置</param>
	/// <param name="enemyCount">敵の配列の位置</param>
	void EventImguizmoEnemySpawnPosFlag(const uint32_t& count, const uint32_t& enemyCount);

	/// <summary>
	/// イベントのimguizmoの操作位置を変更するやつ(敵の移動先)
	/// </summary>
	/// <param name="count">イベントを保持している配列の変更したい位置</param>
	/// <param name="enemyCount">敵の配列の位置</param>
	void EventImguizmoEnemyMoveEndPointFlag(const uint32_t& count, const uint32_t&enemyCount);

	//undoをするためのチェック
	void UndoCheck(const uint32_t& count);

	/// <summary>
	/// undoをするためのチェックと移動イベントのimguizmoの操作のフラグの切り替え
	/// </summary>
	/// <param name="count">イベントを保持している配列の変更したい位置</param>
	/// <param name="enemyType">操作したいやつ(0でスタート位置,1で移動位置)</param>
	void UndoAndMoveImguizmoMoveCheck(const uint32_t& count,int32_t enemyType);

	/// <summary>
	/// undoをするためのチェックと敵のimguizmoの操作のフラグの切り替え
	/// </summary>
	/// <param name="count">イベントを保持している配列の変更したい位置</param>
	/// <param name="enemyCount">敵の配列の位置</param>
	/// <param name="enemyType">操作したいやつ(0で生成位置,1で移動位置)</param>
	void UndoAndEnemyImguizmoMoveCheck(const uint32_t& count, const uint32_t& enemyCount, int32_t enemyType);

	//操作しているイベントの描画を追加するためのやつ
	void UseEventViewAdd(const uint32_t& count);

	//undo用
	void Undo();

	//redo用
	void Redo();

	//undo用追加用
	void AddUndo();

	//redo用追加用
	void AddRedo();

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

	//プレイヤー
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
	std::vector<EventExplosionObjData> explosionObjDatas_;

	//変更しているときのやつ
	EventEnemyData useEnemyData_;
	EventMovePointData useMovePointData_;
	EventExplosionObjData useExplosionObjData_;

	uint16_t useEventType = EventType::none;

	//操作しているイベントの描画用のイベント位置
	uint32_t useEventCount_ = 0;

	//現在操作しているイベント位置
	uint32_t operationEventCount_ = 0;

	//undoとか用の保持用
	std::list<std::vector<EventSeting>> saveSeting_;
	std::list<std::vector<EventEnemyData>> saveEnemyDatas_;
	std::list<std::vector<EventMovePointData>> saveMovePointDatas_;
	std::list<std::vector<EventExplosionObjData>> saveExplosionObjDatas_;

	std::list<std::vector<EventSeting>> reSaveSeting_;
	std::list<std::vector<EventEnemyData>> reSaveEnemyDatas_;
	std::list<std::vector<EventMovePointData>> reSaveMovePointDatas_;
	std::list<std::vector<EventExplosionObjData>> reSaveExplosionObjDatas_;

	//undoの登録用のひとつ前のデータ保持用
	std::vector<EventSeting> saveStackSeting_;
	std::vector<EventEnemyData> saveStackEnemyDatas_;
	std::vector<EventMovePointData> saveStackMovePointDatas_;
	std::vector<EventExplosionObjData> saveStackExplosionObjDatas_;

	//移動イベントのどういう風に動くかのためのタイマー
	float moveEventMoveTimer = 0;
	float moveEventMoveMaxTime = 50;

	//描画用モデル
	AnimationModel* enemyModel_ = nullptr;
	AnimationModel* moveEventModel_ = nullptr;
	AnimationModel* explosionModel_ = nullptr;

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

	int32_t explosionObjNum_ = 0;
	std::vector<Vector3> explosionObjPos_;
	std::vector<Vector3> explosionObjSize_;
	std::vector<Vector3> explosionObjExplosionSize_;
	std::vector<float> explosionObjExplosionTime_;

	//イベントの種類
	const char* EventTypeChar[2] = { "moveEvent","BattleEvent" };
	//敵の種類
	const char* EnemyTypeChar[3] = { "moveOnly","moveAttack","Attack" };

	const char* playerHideTypeChar[3] = { "Down","Right","Left"};

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

	float test_[3] = {};
	float test2_[3] = {};

	Sprite testSpite;

	ExplosionObj testExplosionObj;

	Enemy testEnemy1;
	Enemy testEnemy2;

	//セーブする場所保持
	std::string saveFileName_ = "";

	//undo用セーブ名保持
	std::list<std::string> unSaveFileName_;
	//redo用セーブ名保持
	std::list<std::string> reSaveFileName_;

	//エディタのメニュー用のフラグ
	bool testflag = false;
	bool* p_open= &testflag;

	bool imguiCheckWindow_ = false;
	bool imguiCameraWindow_ = false;
	bool imguiAddEventWindow_ = false;

	bool imguiEventSaveWindow_ = false;
	bool imguiEventOverwriteSaveWindow_ = false;
	bool imguiEventLoadWindow_ = false;
	bool imguiChangeMapWindow_ = false;
	
	//imguizmoのテスト用フラグ
	Object3D testObj_;
	Object3D testObj2_;
	bool testimguizmoFlag = false;

	bool testImguiFlag_ = false;

	bool undoFlag_ = false;

	//イベント用のフラグのまとめ
	//imguizmo操作用のフラグ
	struct EventFlagBuff
	{
		uint16_t eventType = EventType::moveEvent;
		//imguizmo用フラグ群
		bool isMoveStratPoint = false;
		bool isMoveEndPoint = false;

		bool isBattlePlayerPoint = false;
		std::vector<bool> isExplosionObjPoints;
		std::vector<bool> isEnemySpawnPoss;
		std::vector<bool> isEnemyMoveEndPoint;

	};

	std::vector<EventFlagBuff> eventFlags_;

};