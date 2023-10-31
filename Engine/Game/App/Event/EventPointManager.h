#pragma once

#include "EventPoint.h"
#include <sstream>
#include "Sprite.h"
#include "json.hpp"

class EventPointManager
{
public:

	static EventPointManager* GetInstance();

	//外にあるイベントのデータを持つファイルからデータを入れたい
	void LoadEventData(std::string fileName);

	void Initlize();

	void Update();

	void Draw();

	//デバック用のイベントデータ追加
	void SetDebugMoveEvent(Vector3 point1 = { 0,0,50 }, Vector3 point2 = { 50,0,50 }, Vector3 point3 = { 50,0,0 }, Vector3 point4 = { 0,0,0 });
	void SetDebug1MoveEvent(Vector3 point1 = { 0,0,50 }, float Speed1 = 1.0f);
	void SetDebugBattleEvent(Vector3 point1 = { 0,0,50 }, float Speed1 = 1.0f,float interval1=50, Vector3 point2 = { 50,0,50 }, float Speed2 = 1.0f, float interval2=50, Vector3 point3 = { 50,0,0 }, float Speed3 = 1.0f, float interval3=50, Vector3 point4 = { 0,0,0 }, float Speed4 = 1.0f);


	//設定済みイベントデータに変更
	void setEventSeting(const std::vector<EventSeting>& eventSetings) { eventSetings_ = eventSetings; };

	//現在実行中のイベントデータ取得
	EventPoint* GetPEventPoint() { return &eventPoint_; };

	//全部のイベントが終わっているか
	bool GetEventAllEnd() { return eventAllEnd_; };

	//どうしても全てのイベントが終わったことにしたい場合使う
	void SetEventAllEnd(bool flag) { eventAllEnd_ = flag; };

	//イベント変更の演出の時間取得
	bool GetNextTime() { return nextTime_; };

	//今何個目のイベントかを取得
	uint32_t GetEventCount() { return eventCount_; };

	//イベントの数を取得
	uint16_t GetEventNum() { return (uint16_t)eventSetings_.size(); };

	//設定を全部消す
	void reset() { eventSetings_.clear(); eventCount_ = 0; eventPoint_ = EventPoint(); }

public:

	//読み込む際のファイルパス(ファイル名だけで指定するため)
	static const inline std::string SDefaultEventPath_="Resources/json/";

	//ファイル拡張子
	static const inline std::string SDefaultEventExtension_= ".json";

	std::string nowEventDataFileName_;

private:

	EventPointManager() = default;
	~EventPointManager();

	EventPointManager(const EventPointManager&) = delete;
	EventPointManager& operator=(const EventPointManager&) = delete;

	void EventScanning(nlohmann::json deserialized, nlohmann::json& Event);


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

