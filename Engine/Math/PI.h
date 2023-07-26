#pragma once
#include <cstdint>
const float PI = 3.141592653589f;

//���W�A������p�x
float RadianToAngle(float Radian);

//�p�x���烉�W�A��
float AngleToRadian(float Angle);

class Util
{
public:
	static uint32_t Rand(uint32_t min, uint32_t max);
	static uint16_t Rand(uint16_t min, uint16_t max);

	static float Rand(float min, float max);
	static double Rand(double min, double max);
};