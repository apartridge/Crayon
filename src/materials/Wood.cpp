#include "materials/Wood.h"

#include <cassert>

Wood::Wood(Vector3 baseColor, Vector3 highColor, float scale, float perlinScale, Vector3 stripesDirection, float stripesScale) : Lambert()
{
	m_baseColor = baseColor;
	m_highColor = highColor;
	m_scale = scale;
	m_perlinScale = perlinScale;
	m_glossFactor = 0;
	m_stripesDirection = stripesDirection;
	m_stripesScale = stripesScale;
}

Wood::~Wood()
{
}

void normalize(float& x)
{
	if(x < 0)
	{
		x  = -x;
	}
	else if(x > 1)
	{
		x -= 1;
	}
}

Vector3 Wood::proceduralColor(const HitInfo& hit) const
{
	float stripesFactor = sin(dot(m_stripesDirection, hit.P*m_stripesScale));
	bool stripe = stripesFactor > 0;


    Vector3 normal = hit.N;
    float woodDetail;
	if(stripe)
	{
		woodDetail = m_perlinScale*PerlinNoise::noise(hit.P.x*m_scale, hit.P.y*m_scale, hit.P.z*m_scale);
	}
	else
	{
		woodDetail = m_perlinScale*PerlinNoise::noise((hit.P.y+10)*m_scale, (hit.P.z+7)*m_scale, (hit.P.x+1)*m_scale);
	}
	
	
	
	if(woodDetail < 0)
	{
		woodDetail = -woodDetail;
	}

	/*if(stripe)
	{
		woodDetail += hit.P.y*hit.P.z;
		normalize(woodDetail);
	}*/

	float g = woodDetail - int(woodDetail);

	float detailFactor = 0.1;
	float detailScale = 15;
	Vector3 detailsColor = Vector3(0, 0, 0)/255.0;



	float woodDetailFine;
	if(stripe)
	{
		woodDetailFine = detailFactor*PerlinNoise::noise(hit.P.x*detailScale, hit.P.y*detailScale, hit.P.z*detailScale);
	}
	else
	{
		woodDetailFine = detailFactor*PerlinNoise::noise((hit.P.y+hit.P.z)/2*detailScale, (hit.P.x+hit.P.z)/2*detailScale, (hit.P.x+hit.P.y)/2*detailScale);
	}
	

	if(woodDetailFine < 0)
	{
		woodDetailFine = -woodDetailFine;
	}

	/*if(stripe)
	{
		woodDetailFine += hit.P.x;
		normalize(woodDetailFine);
	}*/

	float gFine = woodDetailFine - int(woodDetailFine);

	Vector3 base;
	Vector3 high;
	Vector3 details;

	if(stripe)
	{
		base = m_baseColor;
		high = m_highColor;
		details = detailsColor;
	}
	else
	{
		base = m_baseColor + (m_highColor-m_baseColor) / 5;
		high = m_highColor*0.95;
		details = detailsColor*1.01;
	}

	//return stripe ? Vector3(1,0,0) :  Vector3(0,0,1);


    return (base + 0.75*g*high - gFine*detailsColor);
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
		//Vector3 halfway = l - ray.direction();
		Vector3 reflect = Material::reflect(ray, hit);
		reflect.normalize();
		L += 0.1*m_glossFactor*pow(dot(hit.N, reflect), m_glossPower)*m_glossColor*(m_glossPower+1)/(2*PI);
	}

    return L*visibility;
}

Vector3 Wood::shadeGlobalIllumination(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    return proceduralColor(hit) * Lambert::shadeGlobalIllumination(ray, hit, scene, depth);
}