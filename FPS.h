#pragma once
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

