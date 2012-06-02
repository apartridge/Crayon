#ifndef CRAYON_LAMBERT_H_INCLUDED
#define CRAYON_LAMBERT_H_INCLUDED

#include "materials/Material.h"

class Lambert : public Material
{
public:
    Lambert(const Vector3 & kd = Vector3(0.5));
    virtual ~Lambert();

    virtual void preCalc() {}
    virtual Vector3 shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;
   // virtual Vector3 shadeGlobalIllumination(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;
};

#endif