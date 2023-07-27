#include "PI.h"
#include <random>

//ラジアンから角度
float RadianToAngle(float Radian)
{
	return Radian * (180 / PI);
}

//角度からラジアン
float AngleToRadian(float Angle)
{
	return Angle * (PI / 180);
}

uint32_t Util::Rand(uint32_t min, uint32_t max)
{
	std::random_device seed;
	std::default_random_engine engine(seed());

	std::uniform_int_distribution<uint32_t> dest(min, max);

	return dest(engine);
}
uint16_t Util::Rand(uint16_t min, uint16_t max)
{
	std::random_device seed;
	std::default_random_engine engine(seed());

	std::uniform_int_distribution<uint16_t> dest(min, max);

	return dest(engine);
}

float Util::Rand(float min, float max)
{
	std::random_device seed;
	std::default_random_engine engine(seed());

	std::uniform_real_distribution<float> dest(min, max);

	return dest(engine);
}
double Util::Rand(double min, double max)
{
	std::random_device seed;
	std::default_random_engine engine(seed());

	std::uniform_real_distribution<double> dest(min, max);

	return dest(engine);
}