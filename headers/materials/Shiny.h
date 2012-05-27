#ifndef CRAYON_SHINY_H_INCLUDED
#define CRAYON_SHINY_H_INCLUDED

#include "materials/Material.h"

class Shiny : public Material
{
public:
	Shiny(const Vector3 & diffuse, const Medium & medium = Medium(1), float opacity = 1, const Vector3 & ambient = Vector3(0), float reflectiveness = 0);

    virtual ~Shiny();

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

#endif // CRAYON_LAMBERT_H_INCLUDED
