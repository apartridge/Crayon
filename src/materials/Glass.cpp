#include "materials/Glass.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"

Glass::Glass(float refractiveIndex, Vector3 rt, Vector3 rd) : Material(rd, Vector3(0), rt)
{
	shininess = 500;
    indexOfRefraction = refractiveIndex;
}

Glass::~Glass()
{
}

Vector3 Glass::shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0);

	Vector3 visibility = Material::lightDiffuseVisiblity(light, hit, scene);

	Vector3 l = (light.getPosition() - hit.P).normalized();

	L = diffuseColor(hit)*visibility;
	float costheta = dot(hit.N, l);
	Vector3 wr = reflect(ray, hit);
    L += visibility * Rs() * (pow(dot(wr, l), shininess) / costheta);

    return L;
}

Vector3 Glass::shadeReflectance(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0, 0, 0);
    float n1 = ray.mediumOfTravel.indexOfRefraction, 
          n2 = indexOfRefraction, 
          R  = reflectance(dot(hit.N, -ray.direction()), n1, n2);
    
    bool outside = dot(-ray.direction(), hit.N) > 0;

    // Assume going back to air when hitting from the inside
    if (!outside)
    {
        n1 = indexOfRefraction;
        n2 = 1.0; 
        R  = reflectance(dot(hit.N, ray.direction()), n1, n2);
    }

    Vector3 refractVector = refract(ray, hit, n1, n2);
    
    if (refractVector != Vector3(0))
    {
        Ray refractionRay (hit.P, refractVector);
        refractionRay.mediumOfTravel.indexOfRefraction = n2;
        HitInfo refractionHit;

        if (scene.trace(refractionHit, refractionRay, epsilon))
        {
            Vector3 refractedColor = refractionHit.material->shade(refractionRay, refractionHit, scene, depth + 1);
            L += (1 - R) * refractedColor;
        }
    }
    
    HitInfo refObjHit;
	Vector3 direction = reflect(ray, hit);
    Ray refRay (hit.P, direction);

    if (outside && scene.trace(refObjHit, refRay, epsilon)) 
    {
        Vector3 reflectedColor = refObjHit.material->shade(refRay, refObjHit, scene, depth + 1);
        L += R * reflectedColor;
    }

    return Rt() * L;
}
