#pragma once

#include <vector>

#include "../renderer/Miro.h"
#include "../materials/Material.h"
#include "BoundingBox.h"

class Object
{
public:
    Object() {}
    virtual ~Object() {}

    void setMaterial(const Material* m) {m_material = m;}

    virtual void renderGL() {}
    virtual void preCalc() {}

	virtual void calculateBoundingBox() = 0;
	const BoundingBox& getBoundingBox(){ return m_boundingBox; }

    virtual bool intersect(HitInfo& result, Ray& ray,
                           float tMin = 0.0f, float tMax = MIRO_TMAX) = 0;

protected:
    const Material* m_material;
	BoundingBox m_boundingBox;
};

typedef std::vector<Object*> Objects;