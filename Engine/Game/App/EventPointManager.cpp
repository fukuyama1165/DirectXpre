#include "EventPointManager.h"

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

}

void EventPointManager::SetDebugBattleEvent(Vector3 point1, float interval1, Vector3 point2, float interval2, Vector3 point3, float interval3, Vector3 point4)
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

	
	eventSetings_.push_back(debugPoint);


}

void EventPointManager::Initlize()
{
	eventModel_ = std::make_unique<AnimationModel>();

	eventModel_->Load("testGLTFBall", "gltf", "white1x1");

	eventPoint_.SetIsFinished(true);


}

void EventPointManager::Update()
{

	if (eventSetings_.empty())
	{
		return;
	}

	if (eventPoint_.GetIsFinished() and eventSetings_.size() > eventCount_)
	{
		
		eventPoint_ = EventPoint(eventSetings_[eventCount_]);
		eventCount_++;
	}
	else if (eventPoint_.GetIsFinished())
	{
		eventAllEnd_ = true;
	}

	eventPoint_.Update();


}

void EventPointManager::Draw()
{
	eventPoint_.Draw(eventModel_.get());
}