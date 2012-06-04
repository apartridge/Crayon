#pragma once

#include "materials/Material.h"

class Metal : public Material
{
public:
	Metal(Vector3 metalColor, Vector3 specularColor, float specular_intensity) : Material(metalColor, Vector3(0,0,0))
    {
		m_specularIntensity = specular_intensity;
	}

	virtual ~Metal() {}

    virtual void preCalc() {}

    virtual Vector3 shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;
    //virtual Vector3 shadeGlobalIllumination(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;
	virtual Vector3 shadeReflectance(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;


protected:
	Vector3 m_specularColor;
	float m_specularIntensity;
};