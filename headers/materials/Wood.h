#pragma once

#include "Lambert.h"
#include "../geometry/Ray.h"
#include "../geometry/Scene.h"
#include "Worley.h"
#include "../geometry/Vector3.h"
#include "Perlin.h"

class Wood : public Lambert
{
public:
	Wood(Vector3 baseColor, Vector3 highColor, float scale, float perlinScale);
    virtual ~Wood();
    virtual void preCalc() {};
    
    virtual Vector3 shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;

	void setGlossiness(int power, float factor, Vector3 glossColor = Vector3(1))
	{
		m_glossPower = power;
		m_glossFactor = factor;
		m_glossColor = glossColor;
		
	}// Blinn Phong Specular

private:
    Vector3 WoodTexture(const HitInfo& hit) const;
	Vector3 m_baseColor;
	Vector3 m_highColor;
	float m_scale;
	float m_perlinScale;

	float m_glossPower;
	float m_glossFactor;
	Vector3 m_glossColor;

};
