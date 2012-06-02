#pragma once

#include "materials/Lambert.h"

class LambertTwoColor : public Lambert
{
public:
	LambertTwoColor(Vector3 firstColor, Vector3 secondColor, float wideness, float scale) : Lambert(firstColor)
    {
		m_firstColor = firstColor;
		m_secondColor = secondColor;
		m_wideness = wideness;
		m_scale = scale;
	}

	virtual ~LambertTwoColor() {}

    virtual void preCalc() {}

    //virtual Vector3 shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;
    //virtual Vector3 shadeGlobalIllumination(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;
    
    virtual Vector3 diffuseColor (const HitInfo& hit) const; 

protected:
    Vector3 m_firstColor;
	Vector3 m_secondColor;
	float m_wideness;
	float m_scale;
};