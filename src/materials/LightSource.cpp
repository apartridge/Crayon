#include "../../headers/materials/LightSource.h"
#include "../../headers/geometry/Ray.h"
#include "../../headers/geometry/Scene.h"

LightSource::LightSource() : Material(Vector3(1,1,1), Medium(1), 1)
{

}

LightSource::~LightSource()
{
}

Vector3 LightSource::shade(const Ray& ray, const HitInfo& hit, const Scene& scene, float alpha, const int depth) const
{
	return Vector3(1,1,1) * alpha;
}
