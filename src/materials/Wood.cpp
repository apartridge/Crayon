#include "materials/Wood.h"

#include <cassert>

Wood::Wood(Vector3 baseColor, Vector3 highColor, float scale, Vector3 stripesDirection, float stripesScale) : Lambert()
{
	m_baseColor = baseColor;
	m_highColor = highColor;
	m_scale = scale;
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

Vector3 Wood::diffuseColor(const HitInfo& hit) const
{
	Vector3 m_stretchDirection = Vector3(1,0,1) - m_stripesDirection;
	float sinFactor = sin(dot(m_stretchDirection, hit.P*m_stripesScale));
	bool stripe = sinFactor > 0;
	bool subStripe = sin(2*dot(m_stretchDirection, hit.P*m_stripesScale)) > 0;
	const float m_perlinScale = 10;
	
	Vector3 perlinBaseCoord;
	if(stripe)
	{
		perlinBaseCoord = hit.P * m_scale;
	}
	else
	{
		perlinBaseCoord = (hit.P+Vector3(20,-7,10)) * m_scale;
	}

	// Stretch out

	perlinBaseCoord = perlinBaseCoord * (m_stretchDirection*1.5 + Vector3(1,1,1));
	
	float woodDetail = m_perlinScale*PerlinNoise::noise(perlinBaseCoord.x, perlinBaseCoord.y, perlinBaseCoord.z);
	if(woodDetail < 0)
	{
		woodDetail = -woodDetail;
	}

	float g = woodDetail - int(woodDetail);
	g = 0.30 + 0.70*(1 - powf(1-g, 1.3)); // higher is more bright/flat color

	/*
	Details
	*/

	float detailFactor = 0.3;
	float detailScale = m_scale*2;
	Vector3 detailsColor = Vector3(0, 0, 0)/255.0;

	Vector3 perlinDetailCoord;
	if(stripe)
	{
		perlinDetailCoord = hit.P*detailScale;
	}
	else
	{
		perlinDetailCoord = (hit.P+Vector3(-11,12,20))*detailScale;
	}
	
	// Stretch
	perlinDetailCoord = perlinDetailCoord * (m_stretchDirection*7 + Vector3(1,1,1));

	float woodDetailFine = detailFactor*PerlinNoise::noise(perlinDetailCoord.x, perlinDetailCoord.y, perlinDetailCoord.z);
	if(woodDetailFine < 0)
	{
		woodDetailFine = -woodDetailFine;
	}
	float gFine = woodDetailFine - int(woodDetailFine);

	/*
	Stripes
	*/

	Vector3 perlinStripesCoord = hit.P * (m_stretchDirection*100 + Vector3(1,1,1));
	float woodStripes = PerlinNoise::noise(perlinStripesCoord.x, perlinStripesCoord.y, perlinStripesCoord.z);
	if(woodStripes < 0)
	{
		woodStripes = -woodStripes;
	}
	float gStripes = woodStripes - int(woodStripes);
	gStripes = 0.6 + 0.4*gStripes;
	//gStripes = 1- ( 1 - pow(gStripes,14));

	Vector3 color;
	//Vector3 high;
	//Vector3 details;

	float zeroToOne = powf((sinFactor+1)/2,1.35); // Steepness of gradient

	if(stripe)
	{
		float baseFac = subStripe ? 1.2 : 0.9;
		color = m_baseColor*baseFac + (m_highColor-m_baseColor)*(1-zeroToOne);
	}
	else
	{
		color = m_baseColor + (m_highColor-m_baseColor)*zeroToOne;
	}
    return color*gStripes*g;
}

Vector3 Wood::shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0);
 
	L = Lambert::shadeLight(light, ray, hit, scene, depth); // diffuseColor(hit)*Material::lightDiffuseVisiblity(light, hit, scene);

	// Blinn Phong Specular For Glossiness / "Lakkert" wood

	/*if(m_glossFactor > 0)
	{
		Vector3 l = light.getPosition() - hit.P;
		//Vector3 halfway = l - ray.direction();
		Vector3 reflect = Material::reflect(l, hit);
		reflect.normalize();
		L += 0.1*m_glossFactor*pow(dot(hit.n, reflect), m_glossPower)*m_glossColor*(m_glossPower+1)/(2*PI);
	}*/

    return L;
}
