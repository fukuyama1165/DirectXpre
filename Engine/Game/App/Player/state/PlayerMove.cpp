#include "PlayerMove.h"
#include "EventPointManager.h"
#include "Player.h"

PlayerMove::PlayerMove()
{
}

PlayerMove::~PlayerMove()
{
}

void PlayerMove::Update(Player* player)
{

	//移動イベント処理
	if (EventPointManager::GetInstance()->GetPEventPoint()->GetEventType() == EventType::moveEvent && !EventPointManager::GetInstance()->GetPEventPoint()->GetIsFinished())
	{
		//移動を開始していないなら
		if (moveEventStart_ == false)
		{
			//移動中は隠れているのをやめて元の位置に戻す
			if (EventPointManager::GetInstance()->GetEventCount() != 0)
			{
				player->playerCamera_.pos_ = originalPos_;
				player->SetOriginalPos(originalPos_);
				player->attackFlag_ = false;
			}
			//開始位置を取得して進む大きさを決定
			pos_ = EventPointManager::GetInstance()->GetPEventPoint()->GetMoveStartPoint();
			moveVec_ = nainavec3(EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint(), pos_).normalize();
			player->playerCamera_.pos_ = EventPointManager::GetInstance()->GetPEventPoint()->GetMoveStartPoint();
			moveEventStart_ = true;
		}

		//角度を動かす時間があるなら動かす
		if (EventPointManager::GetInstance()->GetPEventPoint()->GetEventSeting().moveRotTime != 0)
		{
			player->playerCamera_.rotate_ = lerp(rotVec_, EventPointManager::GetInstance()->GetPEventPoint()->GetMovePointRot(), rotTimer_ / EventPointManager::GetInstance()->GetPEventPoint()->GetEventSeting().moveRotTime);
		}

		if (rotTimer_ < EventPointManager::GetInstance()->GetPEventPoint()->GetEventSeting().moveRotTime)
		{
			rotTimer_++;
		}

		//ちょっとずれてもいいように
		if (((player->playerCamera_.pos_.x <= EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint().x + EventPointManager::GetInstance()->GetPEventPoint()->GetMoveSpeed()) &&
			(player->playerCamera_.pos_.x >= EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint().x - EventPointManager::GetInstance()->GetPEventPoint()->GetMoveSpeed())) &&
			((player->playerCamera_.pos_.y <= EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint().y + EventPointManager::GetInstance()->GetPEventPoint()->GetMoveSpeed()) &&
				(player->playerCamera_.pos_.y >= EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint().y - EventPointManager::GetInstance()->GetPEventPoint()->GetMoveSpeed())) &&
			((player->playerCamera_.pos_.z <= EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint().z + EventPointManager::GetInstance()->GetPEventPoint()->GetMoveSpeed()) &&
				(player->playerCamera_.pos_.z >= EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint().z - EventPointManager::GetInstance()->GetPEventPoint()->GetMoveSpeed())))
		{
			//回転が終わったことも確認
			if (rotTimer_ >= EventPointManager::GetInstance()->GetPEventPoint()->GetEventSeting().moveRotTime)
			{
				moveVec_ = { 0,0,0 };
				originalPos_ = EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint();
				player->playerCamera_.pos_ = EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint();
				EventPointManager::GetInstance()->GetPEventPoint()->SetIsFinished(true);
				rotVec_ = EventPointManager::GetInstance()->GetPEventPoint()->GetMovePointRot();
				moveEventStart_ = false;
				rotTimer_ = 0;
			}
		}
		else
		{
			//player->playerCamera_.pos_ += moveVec_ * EventPointManager::GetInstance()->GetPEventPoint()->GetMoveSpeed();
			player->PlayerAddVec(moveVec_ * EventPointManager::GetInstance()->GetPEventPoint()->GetMoveSpeed());
		}

	}
}