#pragma once

#include "geometry/BoundingBox.h"
#include "geometry/Ray.h"
#include "geometry/Triangle.h"

static inline int maxmm(int a, int b)
{
	if(a < b ) return b; else return a;
}

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

	int leaf_nodes()
	{
		if(num_leaves > 0)
		{
			return 1;
		}
		else return children[0].leaf_nodes() + children[1].leaf_nodes();
	}

	int interior_nodes()
	{
		if(num_leaves > 0)
		{
			return 0;
		}
		else return (1 + children[0].interior_nodes()  + children[1].interior_nodes());
	}

	int height()
	{
		if(num_leaves > 0)
		{
			return 1;
		}
		else
		{
			return 1 + maxmm(children[0].height(), children[1].height());
		}
	}

};