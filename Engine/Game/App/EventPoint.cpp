#include "EventPoint.h"
#include "EnemyManager.h"

EventPoint::EventPoint()
{	
	EventSeting seting;
	seting_ = seting;
}

EventPoint::EventPoint(EventSeting seting)
{
	seting_ = seting;
}


EventPoint::~EventPoint()
{
}

void EventPoint::Initlize()
{

	obj_.FBXInit();

}

void EventPoint::Update()
{
	switch (seting_.eventType)
	{
	case moveEvent:

		movePoint_ = seting_.movePoint;

		break;

	case BattleEvent:

		if (timeCount_ == 0 and EnemyManager::GetInstance()->enemyCount_ < seting_.enemyMaxSpawn and count_ < seting_.enemyNum)
		{
			timeCount_ = seting_.enemySpawnInterval[count_];

			EnemyManager::GetInstance()->PopEnemy(seting_.enemySpawnPos[count_]);
			count_++;

		}

		if (timeCount_ > 0)
		{
			timeCount_--;
		}
		
		if (count_ >= seting_.enemyNum and EnemyManager::GetInstance()->enemyCount_<=0)
		{
			IsFinished_ = true;
		}

		break;


	default:
		break;
	}
}

void EventPoint::Draw(AnimationModel* model)
{
	obj_.FBXDraw(*model, false);
}