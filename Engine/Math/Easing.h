#pragma once
#include <math.h>
#include "PI.h"

//a����b�܂ł̎���(0�`1)�̈ʒu��m�邱�Ƃ��ł���֐�
template<typename T>
T lerp(T a, T b, float t)
{
	return a + t * (b - a);
}

template<typename T>
T easeInSine(T start, T end, float time)
{
	return start + (1 - cos((time * PI) / 2)) * (end - start);
}

template<typename T>
T easeOutSine(T start, T end, float time)
{
	return start + (sin((time * PI) / 2)) * (end - start);
}

template<typename T>
T easeInOutSine(T start, T end, float time)
{
	return start + (-(cos(PI * time) - 1) / 2) * (end - start);
}

template<typename T>
T easeOutQuad(T start, T end, float time)
{
	return start + (1 - (1 - time) * (1 - time)) * (end - start);
}

//start�͎n�߂���W end�͈�ԍŌ�̍��W time�͌o�ߎ���(�����͎����ɏ���Ă�)
//��{�I�� �C�[�W���O���������l = �Ŏg��
//���̌������g�������ꍇ�� return start + (����) * (end - start); �Ŏg����
template<typename T>
T easeInQuint(T start, T end, float time)
{
	return start + (time * time * time * time * time) * (end - start);
}

template<typename T>
T easeOutQuint(T start, T end, float time)
{
	return (T)(start + (1 - (float)(pow(1 - time, 5))) * (end - start));
}



template<typename T>
T easeInOutQuint(T start, T end, float time)
{
	return start + (time < 0.5 ? 16 * pow(time, 5) : 1 - pow(-2 * time + 2, 5) / 2) * (end - start);
}

template<typename T>
T easeInBack(T start, T end, float time)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return start + (c3 * time * time * time - c1 * time * time) * (end - start);
}

template<typename T>
T easeOutBack(T start, T end, float time)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return start + (1 + c3 * pow(time - 1, 3) + c1 * pow(time - 1, 2)) * (end - start);
}

template<typename T>
T easeOutElastic(T start, T end, float time)
{
	const float c4 = (2 * PI) / 3;

	return start + (time == 0
		? 0
		: time == 1
		? 1
		: pow(2, -10 * time) * sin((time * 8 - 0.75) * c4) + 1) * (end - start);
}