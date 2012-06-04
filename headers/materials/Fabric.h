#pragma once

#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include "geometry/Vector3.h"
#include "materials/Lambert.h"
#include "materials/Worley.h"
#include "materials/Perlin.h"

class Fabric : public Lambert
{
public:
	Fabric(Vector3 color);
    virtual ~Fabric();
    virtual void preCalc() {};
    
    //virtual Vector3 shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;
    //virtual Vector3 shadeGlobalIllumination(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;
	virtual Vector3 diffuseColor(const HitInfo& hit) const;

protected:

private:
	Vector3 m_color;
};
