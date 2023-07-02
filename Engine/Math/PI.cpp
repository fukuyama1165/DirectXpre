#include "PI.h"

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