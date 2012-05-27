#include "sysutils/PerformanceTimer.h"
#include <cassert>

PerformanceTimer::PerformanceTimer()
{
	QueryPerformanceFrequency(&frequency);
	elapsedTime = 0;
	running = false;
}

void PerformanceTimer::start()
{
	assert(!running);
	running = true;
	QueryPerformanceCounter(&tick_start);
}

void PerformanceTimer::stop()
{
	assert(running);
	running = false;
	LARGE_INTEGER tick_end;
	QueryPerformanceCounter(&tick_end);
	elapsedTime += (float)(tick_end.QuadPart - tick_start.QuadPart)/frequency.QuadPart;
}

float PerformanceTimer::elapsedSec()
{
	return elapsedTime;
}

float PerformanceTimer::elapsedMSec()
{
	return elapsedTime*1000;
}

void PerformanceTimer::reset()
{
	elapsedTime = 0;
	running = false;
}