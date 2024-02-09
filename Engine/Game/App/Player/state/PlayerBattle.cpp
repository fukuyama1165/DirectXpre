#include "PlayerBattle.h"
#include "Player.h"
#include "Easing.h"
#include "EventPointManager.h"
#include "XAudio.h"


PlayerBattle::PlayerBattle()
{
	input_ = Input::GetInstance();
}

PlayerBattle::~PlayerBattle()
{

}

void PlayerBattle::Update(Player* player)
{
	//ゲームシーンなら
	if (!player->isTitle_)
	{

		//隠れる動作用のフラグ立てる場所(リロードも)
		if (((input_->PushKey(DIK_SPACE) && input_->GetIsUseKeybord()) || (input_->GetGamePadButton(XINPUT_GAMEPAD_A) && !input_->GetIsUseKeybord())) && time_ <= 20 && EventPointManager::GetInstance()->GetPEventPoint()->GetEventType() != moveEvent)
		{
			player->attackFlag_ = true;
			player->Reload();


		}

		//隠れた時一回だけ音を鳴らしたい
		if ((input_->TriggerKey(DIK_SPACE) || input_->GetGamePadButtonDown(XINPUT_GAMEPAD_A)) && EventPointManager::GetInstance()->GetPEventPoint()->GetEventType() != moveEvent)
		{
			XAudio::PlaySoundData(player->GetGunShotSount(), 1.0f);
		}

		if ((input_->ReleaseKey(DIK_SPACE) && input_->GetIsUseKeybord()) || (input_->GetGamePadButtonUp(XINPUT_GAMEPAD_A) && !input_->GetIsUseKeybord()))
		{
			player->attackFlag_ = false;
		}
		//隠れる動作用のフラグ立てる場所(リロードも)

	}

	//移動中ではないなら
	if (EventPointManager::GetInstance()->GetPEventPoint()->GetEventType() != moveEvent)
	{
		if (EventPointManager::GetInstance()->GetPEventPoint()->GetPlayerHideVector() == (float)playerHideVectorType::Right)
		{
			HideRightWall(player);
		}
		else if (EventPointManager::GetInstance()->GetPEventPoint()->GetPlayerHideVector() == (float)playerHideVectorType::Down)
		{
			HideDownWall(player);
		}
		else
		{
			HideLeftWall(player);
		}
	}

	if (EventPointManager::GetInstance()->GetPEventPoint()->GetEventType() == EventType::BattleEvent && !EventPointManager::GetInstance()->GetPEventPoint()->GetIsFinished())
	{
		if (battleEventStart_ == false)
		{
			//開始位置を取得
			player->playerCamera_.pos_ = EventPointManager::GetInstance()->GetPEventPoint()->GetEventSeting().playerPos;
			player->SetOriginalPos(EventPointManager::GetInstance()->GetPEventPoint()->GetEventSeting().playerPos);
			battleEventStart_ = true;
		}
	}
	//バトルイベント終了時の処理
	else if (EventPointManager::GetInstance()->GetPEventPoint()->GetEventType() == EventType::BattleEvent)
	{

		battleEventStart_ = false;

	}

	if (!player->isTitle_)
	{
		if ((!player->attackFlag_ && EventPointManager::GetInstance()->GetPEventPoint()->GetEventType() != moveEvent && player->GetBulletNum() > 0) || player->isDebugShot_)
		{
			player->Attack();
		}
	}
}

void PlayerBattle::HideRightWall(Player* player)
{

	if (player->attackFlag_)
	{
		if (time_ < maxMoveTime_)
		{
			time_++;
		}

	}
	else
	{
		if (time_ > 0)
		{
			time_--;
		}

	}


	Vector3 camerapos = {};

	camerapos = DirectXpre::easeOutQuint(Vector3{ player->GetOriginalPos()}, player->GetOriginalPos() + (Vector3::normalize(player->playerCamera_.GetCamera().rightDirection) * hideDistanceX_), time_ / maxMoveTime_);

	player->playerCamera_.pos_ = camerapos;

}

void PlayerBattle::HideDownWall(Player* player)
{
	if (player->attackFlag_)
	{
		if (time_ < maxMoveTime_)
		{
			time_++;
		}

	}
	else
	{
		time_ = 0;
	}

	Vector3 camerapos = {};

	camerapos = DirectXpre::easeOutQuint(Vector3{ player->GetOriginalPos() }, Vector3{ player->GetOriginalPos().x, player->GetOriginalPos().y - hideDistanceY_, player->GetOriginalPos().z }, time_ / maxMoveTime_);

	player->playerCamera_.pos_ = camerapos;
}

void PlayerBattle::HideLeftWall(Player* player)
{
	if (player->attackFlag_)
	{
		if (time_ < maxMoveTime_)
		{
			time_++;
		}

	}
	else
	{
		if (time_ > 0)
		{
			time_--;
		}

	}


	Vector3 camerapos = {};

	camerapos = DirectXpre::easeOutQuint(Vector3{ player->GetOriginalPos() }, player->GetOriginalPos() + (-Vector3::normalize(player->playerCamera_.GetCamera().rightDirection) * hideDistanceX_), time_ / maxMoveTime_);

	player->playerCamera_.pos_ = camerapos;

}