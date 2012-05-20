#include "../../headers/geometry/BoundingVolumeNode.h"
#include "../../headers/renderer/RenderingStats.h"

#if RENDERING_STATS
extern RenderingStats* rendering_statistics;
#endif

bool BoundingVolumeNode::intersect(Ray& ray, HitInfo& minhit, float tMin, 
									float tMax, int level)
{

#if RENDERING_STATS
	rendering_statistics->lock();
	rendering_statistics->box_intersections += 1;
	rendering_statistics->unlock();
#endif

	if(!box.intersectedByRay(ray, tMin, minhit.t))
	{
		return false;
	}

	if(this->num_leaves > 0)
	{
		HitInfo tempMinHit;
		tempMinHit.t = minhit.t;
		bool hit = false;
		int boxIntersections = 0;
		int triangleIntersections = 0;

		for(int i = 0; i < num_leaves; i++)
		{
			if( num_leaves == 1 || ++boxIntersections && leaves[i]->getBoundingBox().intersectedByRay( ray, tMin, minhit.t)   )
			{
				triangleIntersections++;
				if ( leaves[i]->intersect(tempMinHit, ray, tMin, minhit.t)  )
				{
					if (tempMinHit.t < minhit.t)
					{
						hit = true;
						minhit = tempMinHit;
					}
				}
			}
		}

		
#if RENDERING_STATS
		rendering_statistics->lock();
		rendering_statistics->triangle_intersections += triangleIntersections;
		rendering_statistics->box_intersections += boxIntersections;
		rendering_statistics->unlock();
#endif

		return hit;

	}
	
	bool left = this->children[0].intersect(ray, minhit, tMin, tMax, level+1);
	bool right = this->children[1].intersect(ray, minhit, tMin, tMax, level+1);
	
	return left || right;

}

void BoundingVolumeNode::renderGL()
{
	if(num_leaves == 0)
	{
		this->children[0].renderGL();
		this->children[1].renderGL();
	}
	else
	{
		for(int i = 0; i < num_leaves; i++)
		{
			leaves[i]->renderGL();
		}
	}
}