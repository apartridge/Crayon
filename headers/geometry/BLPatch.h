/*#ifndef CRAYON_BL_PATCH_H_INCLUDED
#define CRAYON_BL_PATCH_H_INCLUDED

#include "Vector3.h"
#include "Object.h"

class BLPatch : public Object
{
public:
    BLPatch();
    virtual ~BLPatch();

    Vector3 & vertex(int i)             {return m_verts[i];}
    const Vector3 & vertex(int i) const {return m_verts[i];}

    virtual void renderGL();
	void calculateBoundingBox()
	{
	}
    virtual bool intersect(HitInfo& result, const Ray& ray,
                           float tMin = 0.0f, float tMax = MIRO_TMAX);

protected:
    Vector3 m_verts[4];
};

#endif // CRAYON_BL_PATCH_H_INCLUDED
*/