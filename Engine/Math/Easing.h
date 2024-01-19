#pragma once

/**
 * @file Easing.h
 * @brief イージング関数クラス
 * @author フクヤマ
 * @date 2023_12/29
 */

#include <math.h>
#include "Util.h"

 //startは始める座標 endは一番最後の座標 timeは経過時間(ここは資料に乗ってる)
 //基本的に イージングさせたい値 = で使う
 //他の公式を使いたい場合は return start + (公式) * (end - start); で使える

//aからbまでの時間(0～1)の位置を知ることができる関数
template<typename T>
T lerp(T a, T b, float t)
{
	return a + t * (b - a);
}

//sine

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

//Quad

template<typename T>
T easeInQuad(T start, T end, float time)
{
	return start + time * time * (end - start);
}

template<typename T>
T easeOutQuad(T start, T end, float time)
{
	return start + (1 - (1 - time) * (1 - time)) * (end - start);
}

template<typename T>
T easeInOutQuad(T start, T end, float time)
{
	return (T)(start + (time < 0.5 ? 2 * time * time : 1 - (float)pow(-2 * time + 2, 2) / 2) * (end - start));
}

//Quart

template<typename T>
T easeInQuart(T start, T end, float time)
{
	return start + (time * time * time * time) * (end - start);
}

template<typename T>
T easeOutQuart(T start, T end, float time)
{
	return (T)(start + (1 - (float)(pow(1 - time, 4))) * (end - start));
}

template<typename T>
T easeInOutQuartt(T start, T end, float time)
{
	return start + (time < 0.5 ? 8 * pow(time, 4) : 1 - pow(-2 * time + 2, 4) / 2) * (end - start);
}

//Quint

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

//Expo

template<typename T>
T easeInExpo(T start, T end, float time)
{
	return start + (time == 0 ? 0 : pow(2, 10 * time - 10)) * (end - start);
}

template<typename T>
T easeOutExpo(T start, T end, float time)
{
	return (T)(start + (time == 1 ? 1 : 1 - pow(2, -10 * time)) * (end - start));
}

template<typename T>
T easeInOutExpo(T start, T end, float time)
{
	return start + (time == 0
		? 0
		: time == 1
		? 1
		: time < 0.5 ? pow(2, 20 * time - 10) / 2
		: (2 - pow(2, -20 * time + 10)) / 2) * (end - start);
}

//Circ

template<typename T>
T easeInCirc(T start, T end, float time)
{
	return start + (1 - sqrt(1 - pow(time, 2))) * (end - start);
}

template<typename T>
T easeOutCirc(T start, T end, float time)
{
	return (T)(start + ((float)sqrt(1 - pow(time - 1, 2))) * (end - start));
}

template<typename T>
T easeInOutCirc(T start, T end, float time)
{
	return start + (time < 0.5
		? (1 - sqrt(1 - pow(2 * time, 2))) / 2
		: (sqrt(1 - pow(-2 * time + 2, 2)) + 1) / 2) * (end - start);
}

//Back

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
T easeInOutBack(T start, T end, float time)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	return start + (time < 0.5
		? (pow(2 * time, 2) * ((c2 + 1) * 2 * time - c2)) / 2
		: (pow(2 * time - 2, 2) * ((c2 + 1) * (time * 2 - 2) + c2) + 2) / 2) * (end - start);
}

//Elastic

template<typename T>
T easeInElastic(T start, T end, float time)
{
	const float c4 = (2 * PI) / 3;

	return start + (time == 0
		? 0
		: time == 1
		? 1
		: -pow(2, 10 * time - 10) * sin((time * 10 - 10.75) * c4)) * (end - start);
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

template<typename T>
T easeInOutElastic(T start, T end, float time)
{
	const float c5 = (2 * PI) / 4.5;

	return start + (time == 0
		? 0
		: time == 1
		? 1
		: time < 0.5
		? -(pow(2, 20 * time - 10) * sin((20 * time - 11.125) * c5)) / 2
		: (pow(2, -20 * time + 10) * sin((20 * time - 11.125) * c5)) / 2 + 1) * (end - start);
}


//Bounce

template<typename T>
T easeOutBounce(T start, T end, float time)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (time < 1 / d1) {
		return start + (n1 * time * time) * (end - start);
	}
	else if (time < 2 / d1) {
		return start + (n1 * (time -= 1.5 / d1) * time + 0.75) * (end - start);
	}
	else if (time < 2.5 / d1) {
		return start + (n1 * (time -= 2.25 / d1) * time + 0.9375) * (end - start);
	}
	else {
		return start + (n1 * (time -= 2.625 / d1) * time + 0.984375) * (end - start);
	}

}

template<typename T>
T easeInBounce(T start, T end, float time)
{
	return start + (1 - easeOutBounce(start, end, 1 - time)) * (end - start);
}

template<typename T>
T easeInOutBounce(T start, T end, float time)
{
	return start + (time < 0.5
		? (1 - easeOutBounce(start, end, 1 - 2 * time)) / 2
		: (1 + easeOutBounce(start, end, 2 * time - 1)) / 2) * (end - start);
}