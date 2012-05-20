#ifndef CRAYON_MATERIAL_H_INCLUDED
#define CRAYON_MATERIAL_H_INCLUDED

#include "../renderer/Miro.h"
#include "../geometry/Vector3.h"
#include "../geometry/Medium.h"
#include "../lights/Light.h"

class Material
{
public:
    Material(const Vector3& ambient = Vector3(0));
    virtual ~Material();

    virtual void preCalc() {};
    
    /* Implement shading model by calling shadeLight for each light and 
     * adding to ambient, reflected and refracted light */
    virtual Vector3 shade(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;

    virtual Vector3 shadeLight(const Light& light, const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const { return Vector3(0); };
    
    virtual Vector3 shadeGlobalIllumination(const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const { return Vector3(0); };

    virtual Vector3 shadeReflectance(const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const { return Vector3(0); };

protected:
    static float reflectance(float costheta, float n1, float n2);
    static Vector3 refract(const Ray& ray, const HitInfo& hit, float n1, float n2);
    static Vector3 reflect(const Ray& ray, const HitInfo& hit);

private:
    Vector3 ambient;
    static const int RecursionLimit = 5;
};

/*
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
};*/

#endif // CRAYON_MATERIAL_H_INCLUDED
