#pragma once
#include "PlayerState.h"
#include "Vector3.h"

class PlayerMove :public PlayerState
{
public:
	PlayerMove();
	~PlayerMove();
	//毎フレーム更新
	void Update(Player* player)override;

private:

	Vector3 pos_ = {};

	Vector3 moveVec_ = {};

	//壁に隠れたりする用の基準位置
	Vector3 originalPos_ = {};

	float rotTimer_ = 0;

	//移動開始のための事前処理用フラグ
	bool moveEventStart_ = false;

	//回転保存用のやつ
	Vector3 rotVec_ = { 0,0,0 };

};

