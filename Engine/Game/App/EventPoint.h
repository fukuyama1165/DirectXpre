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
	float moveSpeed = 1;
	uint16_t enemyNum = 0;
	uint16_t enemyMaxSpawn = 0;
	std::vector<Vector3> enemySpawnPos;
	std::vector<Vector3> enemyMovePos;
	std::vector<float> enemyMoveSpeed;
	std::vector<float> enemySpawnInterval;
	std::vector<uint16_t> enemyTypes;

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

	float GetTimeCount() { return timeCount_; };

	bool GetIsFinished() { return IsFinished_; };
	void SetIsFinished(bool isFinished) { IsFinished_ = isFinished; };

	uint16_t GetEventType() { return seting_.eventType; };

	Vector3 GetMovePoint() { return movePoint_; };

	EventSeting GetEventSeting() { return seting_; };


private:

	Vector3 movePoint_ = {};

	float timeCount_ = 0;

	Object3D obj_;

	bool IsFinished_ = false;

	uint32_t count_ = 0;

	EventSeting seting_;

};

