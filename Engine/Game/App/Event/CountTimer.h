#pragma once
#include "Sprite.h"

class CountTimer
{
public:
	CountTimer();
	~CountTimer();

	//初期化
	void Initialize();

	//終了処理
	void Finalize();

	//更新
	void Update();

	//描画
	void Draw();

	float GetTimer() { return timer_; };

	float SetMaxTime(float time) { MaxTime_ = time; };

private:

	float timer_ = 0;
	float MaxTime_ = 99;

	//位描画用スプライト
	Sprite numRankTen_;
	Sprite numRankOne_;
	Sprite numRankComma_;
	Sprite numRankDecimalPointOne_;
	Sprite numRankDecimalPointTen_;

	uint16_t rankTenNum_;
	uint16_t rankOneNum_;
	uint16_t rankCommaNum_;
	uint16_t rankDecimalPointOneNum_;
	uint16_t rankDecimalPointTenNum_;

};

