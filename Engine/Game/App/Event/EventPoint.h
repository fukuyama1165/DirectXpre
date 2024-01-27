#pragma once

/**
 * @file EventPoint.h
 * @brief ゲームの進行をするためのイベントの処理を行うクラス
 * @author フクヤマ
 * @date 2023_11/10
 */

#include <cstdint>
#include "Vector3.h"
#include <vector>
#include "Object3D.h"
#include "ExplosionObj.h"

enum EventType
{
	moveEvent,
	BattleEvent
};

enum playerHideVectorType
{
	Down,
	Right,
	Left
};

struct EventSeting
{
	uint16_t eventType = moveEvent;
	Vector3 movePoint = { 0,0,0 };
	Vector3 moveStartPoint = { 0,0,0 };
	Vector3 movePointRot = { 0,0,0 };
	float moveSpeed = 1;
	float moveRotTime = 1;
	int32_t enemyNum = 0;
	int32_t enemyMaxSpawn = 0;
	std::vector<Vector3> enemySpawnPos;
	std::vector<Vector3> enemyMovePos;
	std::vector<float> enemyMoveSpeed;
	std::vector<float> enemySpawnInterval;
	std::vector<uint32_t> enemyBulletCT;
	std::vector<std::string> enemyTypes;
	float playerHideVector = playerHideVectorType::Down;
	Vector3 playerPos = { 0,0,0 };

	float addTimer = 0;

	int32_t explosionObjNum = 0;
	std::vector<Vector3> explosionObjPos;
	std::vector<Vector3> explosionObjSize;
	std::vector<Vector3> explosionObjExplosionSize;
	std::vector<float> explosionObjExplosionTime;

};

class EventPoint
{
public:
	EventPoint();
	EventPoint(const EventSeting& seting);
	~EventPoint();

	void Initlize();

	void Update();

	void Draw(AnimationModel* model);

	//時間の取得
	float GetTimeCount() { return timeCount_; };

	//終わったか
	bool GetIsFinished() { return IsFinished_; };

	//終わるフラグを設定
	void SetIsFinished(bool isFinished) { IsFinished_ = isFinished; };

	//イベントのタイプを取得
	uint16_t GetEventType() { return seting_.eventType; };

	//移動位置取得
	Vector3 GetMovePoint() { return movePoint_; };

	//移動開始位置取得
	Vector3 GetMoveStartPoint() { return moveStartPoint_; };

	//移動速度取得
	float GetMoveSpeed() { return seting_.moveSpeed; };

	//イベントの角度取得(プレイヤーの向きとか)
	Vector3 GetMovePointRot() { return movePointRot_; };

	//イベントの設定取得
	EventSeting GetEventSeting() { return seting_; };

	//プレイヤーが隠れる方向を取得
	float GetPlayerHideVector() { return seting_.playerHideVector; };


private:

	//移動する位置
	Vector3 movePoint_ = {};

	//動く角度
	Vector3 movePointRot_ = {};

	//移動開始位置
	Vector3 moveStartPoint_ = {};

	//待ち時間
	float timeCount_ = 0;

	//本体の位置
	Object3D obj_;

	//終わったか
	bool IsFinished_ = false;

	//イベント用のカウンター
	int32_t count_ = 0;

	//イベントの設定
	EventSeting seting_;

};

