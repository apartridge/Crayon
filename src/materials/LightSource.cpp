#include "materials/LightSource.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"

LightSource::LightSource() : Material(Vector3(0))
{
}

LightSource::~LightSource()
{
}

Vector3 LightSource::shade(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
	return Vector3(1,1,1);
}
