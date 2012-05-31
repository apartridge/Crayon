#pragma once

#include "Lambert.h"
#include "../geometry/Ray.h"
#include "../geometry/Scene.h"
#include "Worley.h"
#include "../geometry/Vector3.h"
#include "Perlin.h"

class Leather : public Lambert
{
public:
	Leather();
    virtual ~Leather();
    virtual void preCalc() {};
    
    virtual Vector3 shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;

private:

};
