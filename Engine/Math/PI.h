#pragma once
#include <cstdint>
const float PI = 3.141592653589f;



class Util
{
public:

	//���W�A������p�x
	static float RadianToAngle(float Radian);

	//�p�x���烉�W�A��
	static float AngleToRadian(float Angle);

	static uint32_t Rand(uint32_t min, uint32_t max);
	static uint16_t Rand(uint16_t min, uint16_t max);

	static float Rand(float min, float max);
	static double Rand(double min, double max);
};