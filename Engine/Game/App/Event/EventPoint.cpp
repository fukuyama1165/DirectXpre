#include "EventPoint.h"
#include "EnemyManager.h"
#include <imgui.h>

EventPoint::EventPoint()
{	
	EventSeting seting;
	seting_ = seting;
}

EventPoint::EventPoint(EventSeting seting)
{
	seting_ = seting;
	movePoint_ = seting_.movePoint;
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
		movePointRot_ = seting_.movePointRot;

		break;

	case BattleEvent:

		if (timeCount_ == 0 and EnemyManager::GetInstance()->enemyCount_ < seting_.enemyMaxSpawn and count_ < seting_.enemyNum)
		{
			timeCount_ = seting_.enemySpawnInterval[count_];

			EnemyManager::GetInstance()->PopEnemy(seting_.enemyTypes[count_], seting_.enemySpawnPos[count_], seting_.enemyMovePos[count_],seting_.enemyMoveSpeed[count_]);
			count_++;

		}

		if (timeCount_ > 0)
		{
			timeCount_--;
		}
		
		if (timeCount_ == 0 and count_ >= seting_.enemyNum and EnemyManager::GetInstance()->enemyCount_<=0)
		{
			IsFinished_ = true;
		}

#ifdef _DEBUG

		ImGui::Begin("check");

		ImGui::Text("count:%d", count_);

		ImGui::End();

#endif


		break;


	default:
		break;
	}
}

void EventPoint::Draw(AnimationModel* model)
{
	obj_.FBXDraw(*model, false);
}