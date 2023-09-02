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
	fileName;

	eventSetings_.clear();

	if (eventSetings_.empty())
	{
		eventSetings_.push_back(EventSeting());
	}

	eventPoint_ = EventPoint(eventSetings_[0]);

	eventAllEnd_ = false;
	
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

	waitSprite_.pos_ = { WinApp::SWindowWidth_ / 2,WinApp::SWindowHeight_ / 2 };

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

			eventPoint_ = EventPoint(eventSetings_[eventCount_]);
			eventCount_++;
			if (eventCount_ > 1)
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

		if (!nextTime_)
		{
			eventPoint_.Update();
		}
	}
	else
	{
		//nextSprite_.pos_ = easeOutQuad(Vector2{ -nextSprite_.GetTextureSize().x,WinApp::SWindowHeight_ / 2}, Vector2{WinApp::SWindowWidth_+ nextSprite_.GetTextureSize().x/2 ,WinApp::SWindowHeight_ / 2}, nextMoveTime_ / nextMoveMaxTime_);
		
		//イベントの違いで変更
		if (eventPoint_.GetEventType() == EventType::moveEvent)
		{
			nextSprite_.setColor({ 1,0,0,1 });
		}
		else
		{
			nextSprite_.setColor({ 1,1,1,1 });
		}

		if (eventPoint_.GetEventType() == EventType::BattleEvent)
		{

			if (nextMoveTime_ < nextMoveMaxTime_)
			{
				nextSprite_.pos_ = easeOutQuad(Vector2{ -nextSprite_.GetTextureSize().x,WinApp::SWindowHeight_ / 2 }, Vector2{ WinApp::SWindowWidth_ / 2,WinApp::SWindowHeight_ / 2 }, nextMoveTime_ / nextMoveMaxTime_);
				nextMoveTime_++;
			}
			else if (nextMoveTime2_ < nextMoveMaxTime2_)
			{
				nextSprite_.pos_ = easeInQuint(Vector2{ WinApp::SWindowWidth_ / 2,WinApp::SWindowHeight_ / 2 }, Vector2{ WinApp::SWindowWidth_ + nextSprite_.GetTextureSize().x / 2,WinApp::SWindowHeight_ / 2 }, nextMoveTime2_ / nextMoveMaxTime2_);
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
			if (nextMoveTime_ < nextMoveMaxTime_)
			{
				float a = easeOutQuad(0.0f, 20.0f, nextMoveTime_ / nextMoveMaxTime_);

				waitSprite_.setColor({ 1,1,1,sinf(a) });
				nextMoveTime_++;
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