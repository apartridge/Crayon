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
    
    static const int WORLEY_ORDER = 4;
    
    virtual Vector3 shade(const Ray& ray, const HitInfo& hit, const Scene& scene, float alpha, const int depth) const;
};

#endif