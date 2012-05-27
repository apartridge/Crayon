#pragma once

#include "materials/Material.h"

class LightSource : public Material
{
public:
    LightSource();
    virtual ~LightSource();

    virtual void preCalc() {}
    
    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene, const int depth) const;
};
