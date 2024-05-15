#pragma once

/**
 * @file EventPointManager.h
 * @brief ゲームの進行をするためのイベントの管理を行うクラス
 * @author フクヤマ
 * @date 2023_11/10
 */

#include "EventPoint.h"
#include <sstream>
#include "Sprite.h"
#include "json.hpp"
#include "CountTimer.h"

class EventPointManager
{
public:

	static EventPointManager* GetInstance();

	//外にあるイベントのデータを持つファイルからデータを入れたい
	void LoadEventData(const std::string& fileName);

	//自作のイベントデータを読み込むところ
	void LoadeefmEventData(const std::string& fileName);
	//フルパスで指定された場所にあるイベントのデータが入っているファイルからデータを取得(フォルダ名に日本語が入っていると無理)
	void LoadFullPathEventData(const std::string& fileName);

	void Initlize();

	void Update();

	void Draw();

	//デバック用のイベントデータ追加
	void SetDebugMoveEvent(const Vector3& point1 = { 0,0,50 }, const Vector3& point2 = { 50,0,50 }, const Vector3& point3 = { 50,0,0 }, const Vector3& point4 = { 0,0,0 });
	void SetDebug1MoveEvent(const Vector3& point1 = { 0,0,50 }, float Speed1 = 1.0f);
	void SetDebugBattleEvent(const Vector3& point1 = { 0,0,50 }, float Speed1 = 1.0f,float interval1=50, const Vector3& point2 = { 50,0,50 }, float Speed2 = 1.0f, float interval2=50, const Vector3& point3 = { 50,0,0 }, float Speed3 = 1.0f, float interval3=50, const Vector3& point4 = { 0,0,0 }, float Speed4 = 1.0f);


	//設定済みイベントデータに変更
	void setEventSeting(const std::vector<EventSeting>& eventSetings) { eventSetings_ = eventSetings; eventAllEnd_ = false; isSetExplosionObj = false; };

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

	//指定したイベントの番号に移動(なかったりオーバーしてたら何もしない)
	void MoveEventNum(uint32_t eventCount);

	//タイマーを最大にする
	void ResetTimer() { timer_.ResetTimer(); };

	//タイマーを使わないイベントなら
	bool isNoTimer = false;

	//シーン移動したときの現在読み込んでいるイベントの保存
	void EditorMoveSave();

public:

	//読み込む際のファイルパス(ファイル名だけで指定するため)
	static const inline std::string SDefaultEventPath_="Resources/json/";
	//読み込む際の自作のファイルのファイルパス(ファイル名だけで指定するため)
	static const inline std::string SDefaultEventPath2_="Resources/eefm/";

	//ファイル拡張子
	static const inline std::string SDefaultEventExtension_= ".json";

	//自作のファイル拡張子
	static const inline std::string SDefaultEventExtension2_= ".eefm";

	//現在読み込んでいるファイルの名前
	std::string NowEventDataFileName_;

	//ゲームシーンからイベントエディタにシーンチェンジしたときの為の保持用変数&Flag

	//シーン移動前のイベント位置
	uint32_t oldEventCount_ = 0;

	//シーン移動前の読み込んでいたイベントデータ名
	std::string oldEventDataFileName_;

	//エディタに移動してきたか
	bool isEditorMove_ = false;

private:

	//シングルトン用
	EventPointManager() = default;
	~EventPointManager();

	EventPointManager(const EventPointManager&) = delete;
	EventPointManager& operator=(const EventPointManager&) = delete;

	//イベントの中身読み込むよう
	void EventScanning(nlohmann::json deserialized, nlohmann::json& Event);


private:

	//イベントの設定
	std::vector<EventSeting> eventSetings_;

	//現在実行中のイベント
	EventPoint eventPoint_;

	//前回のイベントを参照
	uint16_t beforeEventPointType_ = 0;

	//描画用
	AnimationModel* eventModel_ = nullptr;

	//今何番目のイベントか
	uint32_t eventCount_ = 0;

	//全てのイベントが終わったか
	bool eventAllEnd_ = false;

	//イベントの種類変更用のスプライト
	Sprite nextSprite_;
	Sprite waitSprite_;

	//スプライトの動作用変数
	bool nextTime_ = false;

	//入り
	float nextMoveTime_ = 0;
	float nextMoveMaxTime_ = 150;

	//引き
	float nextMoveTime2_ = 0;
	float nextMoveMaxTime2_ = 150;

	//タイマーの本体
	CountTimer timer_;

	//爆発するオブジェクトをセットしたか
	bool isSetExplosionObj = false;

};

