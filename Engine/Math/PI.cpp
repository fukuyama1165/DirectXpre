#include "PI.h"

//���W�A������p�x
float RadianToAngle(float Radian)
{
	return Radian * (180 / PI);
}

//�p�x���烉�W�A��
float AngleToRadian(float Angle)
{
	return Angle * (PI / 180);
}