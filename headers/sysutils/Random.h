#pragma once
#include <cstdlib>
#include <ctime>


class Random
{
public:
	static void seed()
	{
		srand(time(0));
	}

	static float uniformRand()
	{
		return rand() / double(RAND_MAX + 1);
	}

};