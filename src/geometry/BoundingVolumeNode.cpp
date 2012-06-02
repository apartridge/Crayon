#include "geometry/BoundingVolumeNode.h"

bool BoundingVolumeNode::intersect(Ray& ray, HitInfo& minhit, float tMin, 
									float tMax, int level)
{

	if(this->num_leaves > 0)
	{
		HitInfo tempMinHit;
		tempMinHit.t = minhit.t;
		bool hit = false;
		int boxIntersections = 0;
		int triangleIntersections = 0;

		for(int i = 0; i < num_leaves; i++)
		{
			if( num_leaves <= 2 || (leaves[i]->getBoundingBox().intersectedByRay( ray, tMin, minhit.t) < 1E35 )  )
			{
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
		return hit;
	}
	
	bool box_closest = false, box_farthest = false;
	float leftResult = this->children[0].box.intersectedByRay(ray, tMin, minhit.t);
	float rightResult = this->children[1].box.intersectedByRay(ray, tMin, minhit.t);

	if(leftResult < 1E35 && leftResult < rightResult)
	{
		if(minhit.t > leftResult) 
		{
			box_closest = this->children[0].intersect(ray, minhit, tMin, minhit.t, level+1);
			if( rightResult < 1E35 && minhit.t > rightResult)
			{
				box_farthest = this->children[1].intersect(ray, minhit, tMin, minhit.t, level+1);
			}
		}
	}
	else if(rightResult < 1E35 && minhit.t > rightResult)
	{
		box_closest = this->children[1].intersect(ray, minhit, tMin, minhit.t, level+1);
		if(minhit.t > leftResult)
		{
			box_farthest = this->children[0].intersect(ray, minhit, tMin, minhit.t, level+1);
		}
	}
	
	return box_closest || box_farthest;
	
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