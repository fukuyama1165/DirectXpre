#include "FPS.h"


FPS::FPS()
{

}

FPS::~FPS()
{

}

void FPS::initialize()
{

	reference_ = std::chrono::steady_clock::now();

}

void FPS::update()
{

	const std::chrono::microseconds kMinTime(uint16_t(1000000.0f / 65.0f));

	const std::chrono::microseconds kMinCheckTime(uint16_t(1000000.0f / 65.0f));

	//現在の時間を取得
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	//かかった時間を計測
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	//1/60満ちていないなら
	if (elapsed < kMinTime)
	{

		while (std::chrono::steady_clock::now() - reference_ < kMinTime)
		{
			//1フレーム待ち
			std::this_thread::sleep_for(std::chrono::microseconds(1));

		}

	}

	//現在を設定
	reference_ = std::chrono::steady_clock::now();

}