#pragma once
#include "Sprite.h"

/**
 * @file CountTimer.h
 * @brief 
 * @author フクヤマ
 * @date 2023_11/10
 */

class CountTimer
{
public:
	CountTimer();
	~CountTimer();

	//初期化
	void Initialize();
	void Initialize(float maxTime);

	//終了処理
	void Finalize();

	//更新
	void Update();

	//描画
	void Draw();

	//今の時間を取得
	float GetTimer() { return timer_; };

	//時間を足す
	float AddTimer(float addtime) { return timer_+=addtime; };

	//初期時間を設定し直す
	float SetMaxTime(float time) { MaxTime_ = time; };

private:

	/// <summary>
	/// 設定した桁を表示しつつ入れた値を細かくしてそれを描画するはず
	/// </summary>
	/// <param name="pos">描画位置</param>
	/// <param name="number">表示したい数</param>
	/// <param name="rank">表示したい桁(最大4)</param>
	/// <param name="w">画像一つ一つの横の大きさ</param>
	/// <param name="h">画像一つ一つの縦の大きさ</param>
	void GraphNumberDisplayScore(Vector2 pos, int32_t number, int32_t rank, float w, float h);

	int32_t Numreturn(float number, int32_t& now, int32_t& count);

public:

	//ゼロになったか
	bool isZero_ = false;

	Vector2 pos_ = { 0,0 };
	Vector2 size_ = { 1,1 };

private:

	float timer_ = 0;

	//初期タイム
	float MaxTime_ = 6600;

	int32_t testnum = 0;

	//位描画用スプライト

	Sprite num[4];
	Sprite numRankTen_;
	Sprite numRankOne_;
	Sprite numRankComma_;
	Sprite numRankDecimalPointOne_;
	Sprite numRankDecimalPointTen_;

	uint16_t rankTenNum_=0;
	uint16_t rankOneNum_=0;
	uint16_t rankCommaNum_=0;
	uint16_t rankDecimalPointOneNum_=0;
	uint16_t rankDecimalPointTenNum_=0;

};

