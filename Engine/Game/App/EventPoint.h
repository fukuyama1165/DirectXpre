#pragma once
#include <cstdint>
#include "Vector3.h"
#include <vector>
#include "Object3D.h"

enum EventType
{
	moveEvent,
	BattleEvent
};

struct EventSeting
{
	uint16_t eventType = moveEvent;
	Vector3 movePoint = { 0,0,0 };
	Vector3 movePointRot = { 0,0,0 };
	float moveSpeed = 1;
	uint16_t enemyNum = 0;
	uint16_t enemyMaxSpawn = 0;
	std::vector<Vector3> enemySpawnPos;
	std::vector<Vector3> enemyMovePos;
	std::vector<float> enemyMoveSpeed;
	std::vector<float> enemySpawnInterval;
	std::vector<std::string> enemyTypes;

};

class EventPoint
{
public:
	EventPoint();
	EventPoint(EventSeting seting);
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

	//イベントの角度取得(バトルイベントの向きとか)
	Vector3 GetMovePointRot() { return movePointRot_; };

	//イベントの設定取得
	EventSeting GetEventSeting() { return seting_; };


private:

	//移動する位置
	Vector3 movePoint_ = {};

	//動く角度
	Vector3 movePointRot_ = {};

	//待ち時間
	float timeCount_ = 0;

	//本体の位置
	Object3D obj_;

	//終わったか
	bool IsFinished_ = false;

	//敵の個数等のカウンター
	uint32_t count_ = 0;

	//イベントの設定
	EventSeting seting_;

};

