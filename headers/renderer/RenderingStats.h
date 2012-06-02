#pragma once
#define RENDERING_STATS 0

#if RENDERING_STATS
#include "../pthread/pthread.h"
class RenderingStats
{
public:
	unsigned long long rays;
	unsigned long long box_intersections;
	unsigned long long triangle_intersections;
	int bvh_nodes;
	int bvh_leaf_nodes;

	void lock()
	{
		pthread_mutex_lock(&mutex);
	}

	void unlock()
	{
		pthread_mutex_unlock(&mutex);
	}

	void increment_box_intsects(int d)
	{
		lock();
		box_intersections += d;
		unlock();
	}

	void increment_triangle_intsects(int d)
	{
		lock();
		triangle_intersections += d;
		unlock();
	}

	RenderingStats()
	{
		rays = 0;
		box_intersections = 0;
		triangle_intersections = 0;
		bvh_nodes = 0;
		bvh_leaf_nodes = 0;
		mutex = PTHREAD_MUTEX_INITIALIZER;
	}
private:
	pthread_mutex_t mutex;
};
#endif