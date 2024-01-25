#include "EventPoint.h"
#include "EnemyManager.h"
#include <imgui.h>

EventPoint::EventPoint()
{	
	EventSeting seting;
	seting_ = seting;
	count_ = 0;
}

EventPoint::EventPoint(EventSeting seting)
{
	seting_ = seting;
	movePoint_ = seting_.movePoint;
	count_ = 0;
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

		//移動位置セット
		movePoint_ = seting_.movePoint;
		movePointRot_ = seting_.movePointRot;
		moveStartPoint_ = seting_.moveStartPoint;

		break;

	case BattleEvent:

		//インターバルが終わり、沸き数上限と出てきた敵の数が設定数を超えていない場合
		if (timeCount_ == 0 && EnemyManager::GetInstance()->enemyCount_ < seting_.enemyMaxSpawn && count_ < seting_.enemyNum)
		{
			//インターバルをセット(秒に直す)
			timeCount_ = seting_.enemySpawnInterval[count_]*60;

			//敵を湧かす
			EnemyManager::GetInstance()->PopEnemy(seting_.enemyTypes[count_], seting_.enemySpawnPos[count_], seting_.enemyMovePos[count_],seting_.enemyMoveSpeed[count_], seting_.enemyBulletCT[count_]);
			
			//湧いた数増加
			count_++;
			
		}
		else if (timeCount_ == 0 && count_ >= seting_.enemyNum && EnemyManager::GetInstance()->enemyCount_ <= 0)
		{
			//終わり
			IsFinished_ = true;
		}

		//インターバルが残っていて画面上にいる敵の数が湧き数上限より少なかったら
		if (timeCount_ > 0 && EnemyManager::GetInstance()->enemyCount_ < seting_.enemyMaxSpawn)
		{
			timeCount_--;
		}
		
		

#ifdef _DEBUG

		ImGui::Begin("check");

		ImGui::Text("count:%0.0f", timeCount_);

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