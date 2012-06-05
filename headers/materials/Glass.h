#ifndef GLASS_H_INCLUDED
#define GLASS_H_INCLUDED

#include "materials/Material.h"

class Glass : public Material
{
public:
    Glass(float refractiveIndex = 1.5, Vector3 rt = Vector3(1,1,1), Vector3 rd = Vector3(0));
    virtual ~Glass();

    virtual Vector3 shadeLight(const Light& light, const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const;
    
    virtual Vector3 shadeReflectance(const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const;
};

class FrostedGlass : public Material
{
public:
    FrostedGlass(Vector3 d, Vector3 t);
    
    virtual Vector3 shadeLight(const Light& light, const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const;
    
    virtual Vector3 shadeReflectance(const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const;

};

#endif