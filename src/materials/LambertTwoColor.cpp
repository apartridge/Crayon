#include "materials/LambertTwoColor.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include "sysutils/Random.h"


Vector3 LambertTwoColor::diffuseColor(const HitInfo& hit) const
{
	//return m_firstColor;
    Vector3 color;
    float sinf = sin((hit.P.x + hit.P.z)*m_scale) + m_wideness;

	if(sinf > 0)
	{
		color = m_firstColor;
	}
	else
	{
		color = m_secondColor;
	}
    return color;
}
/*
Vector3 LambertTwoColor::shadeLight(const Light& light, const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const
{
    return diffuseColor(hit) * Material::lightDiffuseVisiblity(light, hit, scene);
}*/
/*
Vector3 LambertTwoColor::shadeGlobalIllumination(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    return Lambert::shadeGlobalIllumination(ray, hit, scene, depth);
}
*/