#ifndef STONE_H_INCLUDED
#define STONE_H_INCLUDED

#include "Material.h"
#include "../geometry/Ray.h"
#include "../geometry/Scene.h"
#include "Worley.h"
#include "../geometry/Vector3.h"
#include "Perlin.h"

class Stone : public Material
{
public:
    Stone();
    virtual ~Stone();
    virtual void preCalc() {};
    
    virtual Vector3 shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;
    static const int WORLEY_ORDER = 4;

private:
    Vector3 stoneTexture(const HitInfo& hit) const;
};

#endif