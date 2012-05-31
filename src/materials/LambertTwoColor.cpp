#include "materials/LambertTwoColor.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include "sysutils/Random.h"


Vector3 LambertTwoColor::shadeLight(const Light& light, const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const
{

	float sinf = sin((hit.P.x + hit.P.z)*m_scale) + m_wideness;
	Vector3 L;

	if(sinf > 0)
	{
		L = m_firstColor;
	}
	else
	{
		L = m_secondColor;
	}

	float visibility = light.visibility(hit.P, scene);
	Vector3 l = light.getPosition() - hit.P;
	float falloff = l.length2();
	l /= sqrt(falloff);
    float nDotL = dot(hit.N, l);		
    L *= Rd() * std::max(0.0f, nDotL/falloff * light.power() / PI) * light.color();

	return L; 

}