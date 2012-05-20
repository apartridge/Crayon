#ifndef CRAYON_MATERIAL_H_INCLUDED
#define CRAYON_MATERIAL_H_INCLUDED

#include "../renderer/Miro.h"
#include "../geometry/Vector3.h"
#include "../geometry/Medium.h"

class Material
{
public:
    Material(Vector3 a, Medium m, float f);
    virtual ~Material();

    virtual void preCalc() {}

	const Vector3 & Material::getDiffuseColor() const;

	Medium getMedium() const
	{
		return m_medium;
	}

	float getOpacity() const {
		return m_opacity;
	}

    virtual Vector3 shade(const Ray& ray, const HitInfo& hit, const Scene& scene,
						  float alpha, const int depth) const;

private:
	Medium m_medium;
	Vector3 m_diffuseColor;
	float m_opacity;
};

#endif // CRAYON_MATERIAL_H_INCLUDED
