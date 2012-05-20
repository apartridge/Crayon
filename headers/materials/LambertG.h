#pragma once

#include "Material.h"

class LambertG : public Material
{
public:
	LambertG(const Vector3 & diffuse);

    virtual ~LambertG();

    const Vector3 & ka() const {return m_ka;}

    void setKa(const Vector3 & ka) {m_ka = ka;}

	void setReflectiveness(float reflectiveness)
	{
		m_reflectiveness = reflectiveness;
	}

    virtual void preCalc() {}
    
    virtual Vector3 shade(const Ray& ray, const HitInfo& hit, const Scene& scene, float alpha, int depth) const;
protected:

    Vector3 m_ka;
	float m_reflectiveness;
};
