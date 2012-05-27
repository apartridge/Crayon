#ifndef GLASS_H_INCLUDED
#define GLASS_H_INCLUDED

#include "materials/Material.h"

class Glass : public Material
{
public:
    Glass();
    virtual ~Glass();

    virtual Vector3 shadeLight(const Light& light, const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const;
    
    virtual Vector3 shadeReflectance(const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const;

    int Shininess;
    float RefractiveIndex;
    Vector3 Rs;
    Vector3 Rd;
};

#endif