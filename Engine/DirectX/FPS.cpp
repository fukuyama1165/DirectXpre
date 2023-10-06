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

	//迴ｾ蝨ｨ譎る俣繧貞叙蠕励☆繧・
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	//蜑榊屓險倬鹸縺九ｉ邨碁℃譎る俣繧貞叙蠕励☆繧・
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	//1/60遘・繧医ｊ繧上★縺九↓遏ｭ縺・凾髢・邨後▲縺ｦ縺・↑縺・ｴ蜷・
	if (elapsed < kMinTime)
	{

		while (std::chrono::steady_clock::now() - reference_ < kMinTime)
		{
			//1繝槭う繧ｯ繝ｭ遘偵せ繝ｪ繝ｼ繝・
			std::this_thread::sleep_for(std::chrono::microseconds(1));

		}

	}

	//迴ｾ蝨ｨ縺ｮ譎る俣繧定ｨ倬鹸縺吶ｋ
	reference_ = std::chrono::steady_clock::now();

}