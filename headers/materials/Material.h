#ifndef CRAYON_MATERIAL_H
#define CRAYON_MATERIAL_H

#include "renderer/Miro.h"
#include "geometry/Vector3.h"
#include "geometry/Medium.h"
#include "lights/Light.h"

class Light;

class Material
{
public:
    Material(const Vector3& d = Vector3(0), const Vector3& s = Vector3(0), const Vector3& t = Vector3(0));
    virtual ~Material();

    virtual void preCalc() {};
    
    /* Implement shading model by calling shadeLight for each light and 
     * adding to ambient, reflected and refracted light */
    virtual Vector3 shade(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;

    virtual Vector3 shadeLight(const Light& light, const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const { return Vector3(0); };
    
    Vector3 shadeGlobalIllumination(const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const; // { return Vector3(0); };

    virtual Vector3 shadeReflectance(const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const { return Vector3(0); };

    //const Vector3& Rd() const { return rd; }
    const Vector3& Rs() const { return rs; }
    const Vector3& Rt() const { return rt; }
	
	/*
	This can be overriden by subclasses and is the diffuse color of the material
	*/
	virtual Vector3 diffuseColor(const HitInfo& hit) const
	{
		return rd;
	}

    Ray refractRay(const Ray& ray, const HitInfo& hit) const;

    static float reflectance(float costheta, float n1, float n2);
    static Vector3 refract(const Ray& ray, const HitInfo& hit, float n1, float n2); // Deprecated for member function
    static Vector3 reflect(const Ray& ray, const HitInfo& hit);
	static Vector3 lightDiffuseVisiblity(const Light& light, const HitInfo& hit, const Scene& scene);

protected:
    // Diffuse, specular and transmittance components
    Vector3 rd, rs, rt;

    float indexOfRefraction;
    float shininess;

private:
    //Vector3 ambient; // Deprecated
    static const int RecursionLimit = 5;
};

#endif 
