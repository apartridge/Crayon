#pragma once
#include <Windows.h>
#include <time.h>

class PerformanceTimer
{
public:
	PerformanceTimer();
	void start();
	void stop();
	float elapsedSec();
	float elapsedMSec();
	void reset();
private:
	float elapsedTime;
	LARGE_INTEGER frequency, tick_start;
	bool running;
};