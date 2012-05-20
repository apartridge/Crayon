#pragma once

#include "../../headers/materials/Material.h"

class LightSource : public Material
{
public:
    LightSource();
    virtual ~LightSource();

    virtual void preCalc() {}
    
    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene, float alpha, const int depth) const;
};
