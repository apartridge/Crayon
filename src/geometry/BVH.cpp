#include "geometry/BVH.h"
#include "../../headers\sysutils\PerformanceTimer.h"
#include <cassert>

#define SPLITS 8
#define SPLIT_ALL_AXES

namespace
{

	/*
	// Get vector3 field based on axis
	*/

	float fromVectorToCorrectFieldX(const Vector3& b) { return b.x; }
	float fromVectorToCorrectFieldY(const Vector3& b) { return b.y; }
	float fromVectorToCorrectFieldZ(const Vector3& b) { return b.z; }

	float (*axisCoordinateFunction(BVH::SplitAxis axis))(const Vector3& b)
	{
		switch(axis)
		{
			case BVH::AXIS_X:
				return &fromVectorToCorrectFieldX;
			case BVH::AXIS_Y:
				return &fromVectorToCorrectFieldY;
			case BVH::AXIS_Z:
				return &fromVectorToCorrectFieldZ;
		}
		return NULL;
	}

	/*
	// Sorting function based on axis
	*/

	bool sortObjectsX(Object* a, Object* b)
	{
		return a->getBoundingBox().midpoint().x 
			   < b->getBoundingBox().midpoint().x;
	}

	bool sortObjectsY(Object* a, Object* b)
	{
		return a->getBoundingBox().midpoint().y 
			   < b->getBoundingBox().midpoint().y;
	}

	bool sortObjectsZ(Object* a, Object* b)
	{
		return a->getBoundingBox().midpoint().z 
			   < b->getBoundingBox().midpoint().z;
	}

	bool (*sortingFunction(BVH::SplitAxis axis))(Object*, Object*)
	{
		switch(axis)
		{
			case BVH::AXIS_X:
				return &sortObjectsX;
			case BVH::AXIS_Y:
				return &sortObjectsY;
			case BVH::AXIS_Z:
				return &sortObjectsZ;
		}

		return NULL;
	}

}

/*
// Construct the Bounding Volume Hierarchy
*/

void BVH::build(Objects* objects, BoundingVolumeNode& root)
{

	PerformanceTimer bvh_construct_timer;
	bvh_construct_timer.start();

	BoundingBox entire_scene;

	// Covering the entire scene

	int triangles = 0;
	for(Objects::iterator it = objects->begin(); it < objects->end(); it++, triangles++) 
	{
		(*it)->calculateBoundingBox();
		const BoundingBox& bb = (*it)->getBoundingBox();

		if(triangles == 0)
		{
			entire_scene = bb;
		}
		else
		{
			entire_scene.expand(bb);
		}
	}

	m_rootNode = new BoundingVolumeNode;
	m_rootNode->box = entire_scene;

	splitNode(m_rootNode, objects, objects->begin(), objects->end(), AXIS_NONE, 0);
	
	root = *m_rootNode; // for scene drawing

	bvh_construct_timer.stop();

	printf("Scene BVH of %d triangles created in %g msecs.\n",
		triangles, bvh_construct_timer.elapsedMSec() );
	printf("Nodes: %d. ", root.interior_nodes() + root.leaf_nodes());
	printf("Leaves: %d.\n", root.leaf_nodes());
}

/*
	Split a node given by rootNode with triangles in objects limited
	by iterators begin and end (which is one past the last element).
	TODO make member of BoundingVolumeNode
*/

void BVH::splitNode(BoundingVolumeNode* rootnode, Objects* objects, 
					Objects::iterator begin, Objects::iterator end,
					SplitAxis parent_splitaxis, int level)
{

	int num_triangles = end - begin;

	if(num_triangles >= 4)
	{

		SplitAxis axis;

		// Find axis to split on! We split on the axis that is widest ...
#ifndef SPLIT_ALL_AXES
		float D[4];

		D[AXIS_X] = rootnode->box.max.x - rootnode->box.min.x;
		D[AXIS_Y] = rootnode->box.max.y - rootnode->box.min.y;
		D[AXIS_Z] = rootnode->box.max.z - rootnode->box.min.z;

		axis = AXIS_X;

		if(D[AXIS_Y] > D[AXIS_X])
		{
			axis = AXIS_Y;
		}

		if(D[AXIS_Z] > D[axis])
		{
			axis = AXIS_Z;
		}
#endif
	
		/*
		// Objects are sorted from small to high on the active axis.
		*/

		float root_area = rootnode->box.surfaceArea();
		float node_cost = sahNode(num_triangles, root_area);

		float best_cost = -100;
		BoundingBox best_left;
		BoundingBox best_right;
		Objects::iterator best_right_first;
		SplitAxis best_axis;
		
#ifdef SPLIT_ALL_AXES
		for(int axisIndex = 0; axisIndex < 3; axisIndex++)
		{
			axis = (SplitAxis)((axisIndex+parent_splitaxis) % 3);
#endif
			float (*axisCoord)(const Vector3&) = axisCoordinateFunction(axis);

			if(axis != parent_splitaxis)
			{
				sort(begin, end, sortingFunction(axis));
			}

			/*
			// Try splitting planes on axis
			// We have SPLITS + 1 splites, where the last split is split-in-the-middle of objects
			// This guarantees we'll have one with elements on both sides
			*/

			Objects::iterator middle = begin + num_triangles / 2;

			float step = (axisCoord(rootnode->box.max) - axisCoord(rootnode->box.min)) / SPLITS;
			float c = axisCoord(rootnode->box.min);

			int candidate_tris[2] = {0,0};
			BoundingBox candidate[2];
			bool startedLeft = false; // Started on the left box?

			for(int split = 0; split < SPLITS + 1; split++, c += step)
			{
				if(split == SPLITS)
				{
					candidate_tris[0] = 0;
				}

				candidate_tris[1] = 0;

				bool rightB = false;

				// Go thru all triangles, they are sorted on split axis
				// from left to right

				for(Objects::iterator it = begin + candidate_tris[0]; it < end; it++) 
				{
					const BoundingBox& currBB = (*it)->getBoundingBox();

					if(split < SPLITS)
					{
						rightB = rightB || c <= axisCoord(currBB.midpoint());
					}
					else
					{
						rightB = it > middle;
					}

					int index = rightB ? 1 : 0;

					if(candidate_tris[index] == 0 && (!startedLeft || index == 1 )   )
					{
						if(index == 0)
						{
							startedLeft = true;
						}
						candidate[index] = currBB;
					}
					else
					{
						candidate[index].expand(currBB);
					}

					candidate_tris[index]++;

				}

				// Better than previous best

				if(candidate_tris[0] > 0 && candidate_tris[1] > 0)
				{
					float cost = sahSplit(candidate_tris[0], candidate[0].surfaceArea(),
										  candidate_tris[1], candidate[1].surfaceArea(),
										  root_area);

					if(cost < best_cost || best_cost < 0)
					{
						best_cost = cost;
						best_left = candidate[0];
						best_right = candidate[1];
						best_right_first = begin + candidate_tris[0];
						best_axis = axis;
					}
				}

			}

#ifdef SPLIT_ALL_AXES
		}
#endif
		
		// If Best Cost Is Too Low

		if(best_cost >= 0 && best_cost < node_cost)
		{

			// Have to re-sort on best axis

			if(axis != best_axis)
			{
				sort(begin, end, sortingFunction(best_axis));
			}

			// Create interior node

			rootnode->children = new BoundingVolumeNode[2];
			rootnode->children[0].box = best_left;
			splitNode(rootnode->children, objects, begin, best_right_first, best_axis, level+1);

			rootnode->children[1].box = best_right;
			splitNode(rootnode->children + 1, objects, best_right_first, end, best_axis, level+1);

			return;

		}
		/*else if(best_cost < 0 ) 
		{
			printf("Best cost %g", best_cost);
			throw 1;
		}*/

	}
	
	// Create leaf node with these objects

	rootnode->leaves = &(*begin);
	rootnode->num_leaves = num_triangles;
}

/*
// The Surface Area Heuristic
*/

static const float cost_box = 1;
static const float cost_tri = 4;

float BVH::sahSplit(int N1, float area1, int N2, float area2, float area_root)
{
	float cost_left = float(N1)*area1/area_root;
	float cost_right = float(N2)*area2/area_root;
	return 2*cost_box + cost_tri*(cost_left+cost_right);
}

float BVH::sahNode(int N, float area)
{
	return cost_box + N*cost_tri;
}

/*
// Intersect a ray with this BVH
*/

bool BVH::intersect(HitInfo& minHit, Ray& ray, float tMin, float tMax) const
{
	minHit.t = tMax;
	return m_rootNode->intersect(ray, minHit, tMin, tMax, 0);
}