#pragma once

#include "../renderer/Miro.h"
#include "../geometry/Object.h"
#include "../geometry/TriangleMesh.h"
#include "../geometry/BoundingVolumeNode.h"
#include "../geometry/Ray.h"
#include "../sysutils/Console.h"
#include "../geometry/Triangle.h"
#include "../geometry/BoundingVolumeNode.h"
#include <math.h>
#include <algorithm>

class BVH
{
public:

	enum SplitAxis {AXIS_X, AXIS_Y, AXIS_Z, AXIS_NONE};


    void build(Objects* objects, TriangleMeshes* meshes,  BoundingVolumeNode&);

	void splitNode(BoundingVolumeNode* rootnode, Objects* objects, 
					Objects::iterator from, Objects::iterator to,
					 SplitAxis axis, int level);

    bool intersect(HitInfo& result, Ray& ray,
                   float tMin = 0.0f, float tMax = MIRO_TMAX) const;

	//int trianglesInsideVolume(TriangleMeshes*, BoundingBox&);

	static float sahSplit(int N1, float area1, int N2, float area2, float area_root);
	static float sahNode(int N, float area);

protected:
    //const Objects* m_objects;
	BoundingVolumeNode* m_rootNode;
};