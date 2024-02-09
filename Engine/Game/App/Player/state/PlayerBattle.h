#pragma once
#include "PlayerState.h"
#include "Input.h"
#include "Vector3.h"

class PlayerBattle :public PlayerState
{
public:
	PlayerBattle();
	~PlayerBattle();
	//毎フレーム更新
	void Update(Player* player)override;

private:

	//右に隠れる処理
	void HideRightWall(Player* player);

	//下に隠れる処理
	void HideDownWall(Player* player);

	//左に隠れる処理
	void HideLeftWall(Player* player);

private:

	Input* input_ = nullptr;

	//隠れるときの時間計測用の変数
	float time_ = 0;

	//戦闘開始のための事前処理用フラグ
	bool battleEventStart_ = false;

	//隠れるときの動く大きさ
	float hideDistanceX_ = 2;
	float hideDistanceY_ = 2;

	//滑らかな移動のため
	float maxTime_ = 500;
	//カメラの始点の移動するときの時間
	float maxMoveTime_ = maxTime_ / 15;

};