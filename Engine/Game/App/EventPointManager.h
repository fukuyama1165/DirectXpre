#pragma once

#include "EventPoint.h"
#include <sstream>
#include "Sprite.h"

class EventPointManager
{
public:

	static EventPointManager* GetInstance();

	//外にあるイベントのデータを持つファイルからデータを入れたい
	void LoadEventData(std::string fileName);

	void Initlize();

	void Update();

	void Draw();

	void SetDebugMoveEvent(Vector3 point1 = { 0,0,50 }, Vector3 point2 = { 50,0,50 }, Vector3 point3 = { 50,0,0 }, Vector3 point4 = { 0,0,0 });
	void SetDebug1MoveEvent(Vector3 point1 = { 0,0,50 }, float Speed1 = 1.0f);
	void SetDebugBattleEvent(Vector3 point1 = { 0,0,50 }, float Speed1 = 1.0f,float interval1=50, Vector3 point2 = { 50,0,50 }, float Speed2 = 1.0f, float interval2=50, Vector3 point3 = { 50,0,0 }, float Speed3 = 1.0f, float interval3=50, Vector3 point4 = { 0,0,0 }, float Speed4 = 1.0f);

	EventPoint* GetPEventPoint() { return &eventPoint_; };

	bool GetEventAllEnd() { return eventAllEnd_; };

	void SetEventAllEnd(bool flag) { eventAllEnd_ = flag; };

	bool GetNextTime() { return nextTime_; };

	void reset() { eventSetings_.clear(); eventCount_ = 0; eventPoint_ = EventPoint(); }

private:

	EventPointManager() = default;
	~EventPointManager();

	EventPointManager(const EventPointManager&) = delete;
	EventPointManager& operator=(const EventPointManager&) = delete;


private:

	std::vector<EventSeting> eventSetings_;

	EventPoint eventPoint_;

	//前回のイベントを参照
	uint16_t beforeEventPointType_ = 0;

	AnimationModel* eventModel_ = nullptr;

	bool isActive = false;

	uint32_t eventCount_ = 0;

	bool eventAllEnd_ = false;

	Sprite nextSprite_;
	Sprite waitSprite_;

	bool nextTime_ = false;

	float nextMoveTime_ = 0;
	float nextMoveMaxTime_ = 150;

	float nextMoveTime2_ = 0;
	float nextMoveMaxTime2_ = 150;

};

