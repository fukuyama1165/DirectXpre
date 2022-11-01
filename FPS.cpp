#include "FPS.h"


FPS::FPS()
{

}

FPS::~FPS()
{

}

void FPS::initialize()
{

	reference = std::chrono::steady_clock::now();

}

void FPS::update()
{

	const std::chrono::microseconds kMinTime(uint16_t(1000000.0f / 60.0f));

	const std::chrono::microseconds kMinCheckTime(uint16_t(1000000.0f / 65.0f));

	//現在時間を取得する
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	//前回記録から経過時間を取得する
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference);

	//1/60秒(よりわずかに短い時間)経っていない場合
	if (elapsed < kMinTime)
	{

		while (std::chrono::steady_clock::now()-reference<kMinTime)
		{
			//1マイクロ秒スリープ
			std::this_thread::sleep_for(std::chrono::microseconds(1));

		}

	}

	//現在の時間を記録する
	reference = std::chrono::steady_clock::now();

}