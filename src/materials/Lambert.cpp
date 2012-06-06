#include "materials/Lambert.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include "sysutils/Random.h"

Lambert::Lambert(const Vector3& d) : Material(d)
{
}

Lambert::~Lambert()
{
}

Vector3 Lambert::shadeLight(const Light& light, const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const
{
	return diffuseColor(hit)*lightDiffuseVisiblity(light, hit, scene);
}

