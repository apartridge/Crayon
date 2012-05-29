#ifndef CRAYON_MIRROR_H
#define CRAYON_MIRROR_H

#include "materials/Material.h"

class Mirror : public Material
{
public:
	Mirror();

    virtual Vector3 shadeLight(const Light& light, const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const;
    
    virtual Vector3 shadeReflectance(const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const;
};

#endif
