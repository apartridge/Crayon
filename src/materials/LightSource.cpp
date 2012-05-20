#include "../../headers/materials/LightSource.h"
#include "../../headers/geometry/Ray.h"
#include "../../headers/geometry/Scene.h"

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
