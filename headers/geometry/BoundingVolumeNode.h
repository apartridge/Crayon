#pragma once

#include "BoundingBox.h"
#include "Ray.h"
#include "Triangle.h"

class BoundingVolumeNode
{
public:
	BoundingBox box;
	int num_leaves;
	union
	{
		Object** leaves;
		BoundingVolumeNode* children;
	};

	BoundingVolumeNode()
	{
		num_leaves = 0;
		children = NULL;
		leaves = NULL;
	}

	bool intersect(Ray& ray, HitInfo& minhit,
				   float tMin, float tMax, int level);

	void renderGL();


};