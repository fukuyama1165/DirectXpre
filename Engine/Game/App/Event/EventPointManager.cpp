#include "EventPointManager.h"
#include "ModelManager.h"
#include "Enemy.h"
#include "Easing.h"
#include "WinApp.h"


EventPointManager* EventPointManager::GetInstance()
{
	static EventPointManager instance;

	if (instance.eventModel_ == nullptr)
	{
		instance.Initlize();
	}

	return &instance;
}
EventPointManager::~EventPointManager()
{
}

void EventPointManager::LoadEventData(std::string fileName)
{

	eventSetings_.clear();

	nowEventDataFileName_ = fileName;

	//元から用意していたパスをくっつけて完全に通るパスにする
	const std::string fullPath = SDefaultEventPath_ + fileName + SDefaultEventExtension_;

	//ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullPath);

	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいイベントファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();

	//正しいかどうかチェック
	assert(name.compare("event") == 0);

	//"events"の全オブジェクトを走査
	for (nlohmann::json& events : deserialized["events"])
	{

		EventScanning(deserialized, events);
	}

	//return std::move(levelData);

	if (eventSetings_.empty())
	{
		eventSetings_.push_back(EventSeting());
	}

	eventPoint_ = EventPoint(eventSetings_[0]);

	//最初がバトルの場合倒した後にもう一周してしまうため(移動イベントだとおきない)
	if (eventPoint_.GetEventType() == EventType::BattleEvent)
	{
		eventPoint_.SetIsFinished(true);
	}

	eventAllEnd_ = false;
	
}

void EventPointManager::EventScanning(nlohmann::json deserialized, nlohmann::json& Event)
{


	//typeがなければ止める
	assert(Event.contains("type"));
	//タイプを取得
	std::string type = Event["type"].get<std::string>();



	//moveEventなら
	if (type.compare("moveEvent") == 0)
	{


		
		EventSeting eventData;

		eventData.eventType = EventType::moveEvent;

		//設定のパラメータ読み込み
		nlohmann::json& seting = Event["seting"];

		//移動する場所読み込み
		eventData.movePoint.x = (float)seting["movePoint"][0];
		eventData.movePoint.y = (float)seting["movePoint"][1];
		eventData.movePoint.z = (float)seting["movePoint"][2];

		//移動開始位置取得
		eventData.moveStartPoint.x = (float)seting["moveStartPoint"][0];
		eventData.moveStartPoint.y = (float)seting["moveStartPoint"][1];
		eventData.moveStartPoint.z = (float)seting["moveStartPoint"][2];

		//移動するときの角度読み込み
		eventData.movePointRot.x = (float)seting["movePointRot"][0];
		eventData.movePointRot.y = (float)seting["movePointRot"][1];
		eventData.movePointRot.z = (float)seting["movePointRot"][2];

		//スピードのセット
		eventData.moveSpeed = (float)seting["moveSpeed"];

		eventSetings_.push_back(eventData);


	}
	else if (type.compare("BattleEvent") == 0)
	{

		EventSeting eventData;

		//設定のパラメータ読み込み
		nlohmann::json& seting = Event["seting"];

		//沸き数と画面内の最大数をセット
		eventData.enemyMaxSpawn = seting["enemyMaxSpawn"];
		eventData.enemyNum = seting["enemyNum"];

		//イベントのタイプをセット
		eventData.eventType = BattleEvent;

		//エネミーの数だけ回す
		for (uint16_t i = 0; i < (uint16_t)seting["enemyNum"]; i++)
		{
			//設定されてないやつをみようとしたらそもそもよばないように
			if ((float)seting["spawnPoint"].size() <= i or
				(float)seting["spawnInterval"].size() <= i or
				(float)seting["enemyType"].size() <= i or
				(float)seting["enemySpeed"].size() <= i or
				(float)seting["enemyMovePos"].size() <= i) continue;


			//湧く場所をセット
			eventData.enemySpawnPos.push_back({ (float)seting["spawnPoint"][i][0],(float)seting["spawnPoint"][i][1] ,(float)seting["spawnPoint"][i][2] });

			//湧く間隔をセット
			eventData.enemySpawnInterval.push_back((float)seting["spawnInterval"][i]);

			//エネミーの種類をセット
			eventData.enemyTypes.push_back(seting["enemyType"][i].get<std::string>());

			//エネミーが動く場合動くときの速度をセット
			eventData.enemyMoveSpeed.push_back((float)seting["enemySpeed"][i]);

			//エネミーが動く場合の動く位置
			eventData.enemyMovePos.push_back({ (float)seting["enemyMovePos"][i][0],(float)seting["enemyMovePos"][i][1] ,(float)seting["enemyMovePos"][i][2] });

		}

		eventSetings_.push_back(eventData);

	}




}

void EventPointManager::SetDebugMoveEvent(Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4)
{
	EventSeting debugPoint;

	debugPoint.movePoint = point1;

	eventSetings_.push_back(debugPoint);

	debugPoint.movePoint = point2;

	eventSetings_.push_back(debugPoint);

	debugPoint.movePoint = point3;

	eventSetings_.push_back(debugPoint);

	debugPoint.movePoint = point4;

	eventSetings_.push_back(debugPoint);

	eventAllEnd_ = false;

}

void EventPointManager::SetDebug1MoveEvent(Vector3 point1, float Speed1)
{
	EventSeting debugPoint;

	debugPoint.movePoint = point1;

	debugPoint.moveSpeed = Speed1;

	eventSetings_.push_back(debugPoint);

	eventAllEnd_ = false;

}

void EventPointManager::SetDebugBattleEvent(Vector3 point1, float Speed1, float interval1, Vector3 point2, float Speed2, float interval2, Vector3 point3, float Speed3, float interval3, Vector3 point4, float Speed4)
{

	EventSeting debugPoint;

	debugPoint.enemyMaxSpawn = 5;
	debugPoint.enemyNum = 4;
	
	debugPoint.eventType = BattleEvent;

	debugPoint.enemySpawnPos.push_back(point1);
	debugPoint.enemySpawnPos.push_back(point2);
	debugPoint.enemySpawnPos.push_back(point3);
	debugPoint.enemySpawnPos.push_back(point4);

	debugPoint.enemySpawnInterval.push_back(0);
	debugPoint.enemySpawnInterval.push_back(interval1);
	debugPoint.enemySpawnInterval.push_back(interval2);
	debugPoint.enemySpawnInterval.push_back(interval3);

	debugPoint.enemyTypes.push_back(EnemyType::Attack);
	debugPoint.enemyTypes.push_back(EnemyType::moveOnly);
	debugPoint.enemyTypes.push_back(EnemyType::Attack);
	debugPoint.enemyTypes.push_back(EnemyType::moveAttack);

	debugPoint.enemyMoveSpeed.push_back(Speed1);
	debugPoint.enemyMoveSpeed.push_back(0.1f);
	debugPoint.enemyMoveSpeed.push_back(Speed3);
	debugPoint.enemyMoveSpeed.push_back(Speed4);
	Speed2;

	debugPoint.enemyMovePos.push_back({ 10, 10, 50 });
	debugPoint.enemyMovePos.push_back({ -10, 10, 50 });
	debugPoint.enemyMovePos.push_back({ 0, 10, 50 });
	debugPoint.enemyMovePos.push_back({ 20, 10, 50 });

	
	eventSetings_.push_back(debugPoint);

	eventAllEnd_ = false;


}

void EventPointManager::Initlize()
{

	eventModel_ = ModelManager::GetInstance()->SearchModelData("whiteBox");

	eventPoint_.SetIsFinished(true);

	eventAllEnd_ = false;

	Texture::GetInstance()->loadTexture("Resources/NEXT.png", "NEXT");
	Texture::GetInstance()->loadTexture("Resources/wait.png", "WAIT");

	nextSprite_.initialize("NEXT");
	waitSprite_.initialize("WAIT");

	waitSprite_.pos_ = { (float)WinApp::SWindowWidth_ / 2,(float)WinApp::SWindowHeight_ / 2 };

}

void EventPointManager::Update()
{

	if (eventSetings_.empty())
	{
		return;
	}

	

	if (!nextTime_)
	{

		if (eventPoint_.GetIsFinished() and eventSetings_.size() > eventCount_)
		{
			beforeEventPointType_ = eventPoint_.GetEventType();
			eventPoint_ = EventPoint(eventSetings_[eventCount_]);
			eventCount_++;
			if (eventCount_ > 1 and (beforeEventPointType_!=eventPoint_.GetEventType() or (beforeEventPointType_==EventType::moveEvent and eventPoint_.GetEventType()==EventType::moveEvent)))
			{
				nextTime_ = true;
				nextMoveTime_ = 0;
				nextMoveTime2_ = 0;
			}
		}
		else if (eventPoint_.GetIsFinished())
		{
			eventAllEnd_ = true;
		}
	
	}

	if (!nextTime_ or eventPoint_.GetEventType() == EventType::moveEvent)
	{
		eventPoint_.Update();
	}

	if(nextTime_)
	{
		//nextSprite_.pos_ = easeOutQuad(Vector2{ -nextSprite_.GetTextureSize().x,WinApp::SWindowHeight_ / 2}, Vector2{WinApp::SWindowWidth_+ nextSprite_.GetTextureSize().x/2 ,WinApp::SWindowHeight_ / 2}, nextMoveTime_ / nextMoveMaxTime_);

		if (eventPoint_.GetEventType() == EventType::BattleEvent)
		{

			if (nextMoveTime_ < nextMoveMaxTime_)
			{
				nextSprite_.pos_ = easeOutQuad(Vector2{ -nextSprite_.GetTextureSize().x,(float)WinApp::SWindowHeight_ / 2 }, Vector2{ (float)WinApp::SWindowWidth_ / 2,(float)WinApp::SWindowHeight_ / 2 }, nextMoveTime_ / nextMoveMaxTime_);
				nextMoveTime_++;
			}
			else if (nextMoveTime2_ < nextMoveMaxTime2_)
			{
				nextSprite_.pos_ = easeInQuint(Vector2{ (float)WinApp::SWindowWidth_ / 2,(float)WinApp::SWindowHeight_ / 2 }, Vector2{ (float)WinApp::SWindowWidth_ + nextSprite_.GetTextureSize().x / 2,(float)WinApp::SWindowHeight_ / 2 }, nextMoveTime2_ / nextMoveMaxTime2_);
				nextMoveTime2_++;

			}
			else
			{
				nextTime_ = false;
			}

			nextSprite_.Update();
		}
		else if(eventPoint_.GetEventType() == EventType::moveEvent)
		{
			if (!eventPoint_.GetIsFinished())
			{
				//float a = easeOutQuad(0.0f, 20.0f, nextMoveTime_ / nextMoveMaxTime_);

				waitSprite_.setColor({ 1,1,1,sinf(nextMoveTime_) });
				nextMoveTime_+=0.1f;
			}
			else
			{
				nextTime_ = false;
			}

			waitSprite_.Update();
		}
	}

}

void EventPointManager::Draw()
{
	//eventPoint_.Draw(eventModel_);

	if (nextTime_ && eventPoint_.GetEventType() == EventType::BattleEvent)
	{
		nextSprite_.Draw();
	}
	else if(nextTime_ && eventPoint_.GetEventType() == EventType::moveEvent)
	{
		waitSprite_.Draw();
	}
}