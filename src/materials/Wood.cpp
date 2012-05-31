#include "materials/Wood.h"

#include <cassert>

Wood::Wood(Vector3 baseColor, Vector3 highColor, float scale, float perlinScale) : Lambert()
{
	m_baseColor = baseColor;
	m_highColor = highColor;
	m_scale = scale;
	m_perlinScale = perlinScale;
	m_glossFactor = 0;
}

Wood::~Wood()
{
}

Vector3 Wood::proceduralColor(const HitInfo& hit) const
{
    Vector3 normal = hit.N;
    float woodDetail = m_perlinScale*PerlinNoise::noise(hit.P.x*m_scale, hit.P.y*m_scale, hit.P.z*m_scale);
	if(woodDetail < 0)
	{
		woodDetail = -woodDetail;
	}

	float g = woodDetail - int(woodDetail);

	float detailFactor = 0.05;
	
	float detailScale = 11;
	float detailsPerlinScale = 3;
	Vector3 detailsColor = Vector3(99, 66, 33)/255.0;

	float woodDetailFine = detailFactor*detailsPerlinScale*PerlinNoise::noise(hit.P.x*detailScale, hit.P.y*detailScale, hit.P.z*detailScale);
	if(woodDetailFine < 0)
	{
		woodDetailFine = -woodDetailFine;
	}

	float gFine = woodDetailFine - int(woodDetailFine);

    return (m_baseColor + 0.5*g*m_highColor + gFine*detailsColor);
}

Vector3 Wood::shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0);
    Vector3 l = light.getPosition() - hit.P;

	Vector3 woodenColor = proceduralColor(hit);

    // Calculate shadow ray
    HitInfo shadowHit;
    Ray shadowRay (hit.P, l.normalized());

	float visibility = light.visibility(hit.P, scene);
	
	float falloff = l.length2();
	l /= sqrt(falloff);

    float nDotL = dot(hit.N, l);
	
    L += woodenColor * std::max(0.0f, nDotL/falloff * light.power() / PI) * light.color();

	// Blinn Phong Specular For Glossiness / "Lakkert" wood (Phong in Lambert won't do ...)

	if(m_glossFactor > 0)
	{
		Vector3 halfway = l - ray.direction();
		halfway.normalize();
		L += 0.1*m_glossFactor*pow(dot(hit.N, halfway), m_glossPower)*m_glossColor*(m_glossPower+1)/(2*PI);
	}

    return L*visibility;
}

Vector3 Wood::shadeGlobalIllumination(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    return proceduralColor(hit) * Lambert::shadeGlobalIllumination(ray, hit, scene, depth);
}