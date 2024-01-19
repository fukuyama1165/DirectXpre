#pragma once

/**
 * @file FPS.h
 * @brief FPSの固定を行っている
 * @author フクヤマ
 * @date 2023_12/29
 */

#include <chrono>
#include <thread>


class FPS
{
public:
	FPS();
	~FPS();

	void initialize();

	void update();

private:
	std::chrono::steady_clock::time_point reference_;
};

