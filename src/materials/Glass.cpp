#include "materials/Glass.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"

Glass::Glass() : Material(Vector3(0), Vector3(0.3), Vector3(0.7))
{
    Rd = Vector3(0);
    Rs = Vector3(0.8);
    Shininess = 500;
    RefractiveIndex = 1.5; // Deprecated
    indexOfRefraction = 1.5; // Override Material
}

Glass::~Glass()
{
}

Vector3 Glass::shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0);
    Vector3 l = light.getPosition() - hit.P;
    float r2 = l.length2();
    l /= sqrt(r2);
    
    HitInfo shadowHit;
    Ray shadowRay (hit.P, l);
    /*shadowRay.d = l; 
    shadowRay.o = hit.P;*/
    bool inShadow = scene.trace(shadowHit, shadowRay, epsilon);
    bool outside = dot(-ray.direction(), hit.N) > 0;

    if (outside && !inShadow)
    {
        // Diffuse shading
        float costheta = dot(hit.N, l);
        L += Rd * std::max(0.0f, costheta/r2 * light.power() / PI) * light.color();

        // Specular highlight (Phong)
        // Missed something here last, tme, what was that??
        Vector3 wr = reflect(ray, hit);
        L += Rs * (pow(dot(wr, l), Shininess) / costheta) * light.color();
    }

    return L;
}

Vector3 Glass::shadeReflectance(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0, 0, 0);
    float n1 = ray.mediumOfTravel.indexOfRefraction, 
          n2 = RefractiveIndex, 
          R  = reflectance(dot(hit.N, -ray.direction()), n1, n2);
    
    bool outside = dot(-ray.direction(), hit.N) > 0;

    // Assume going back to air when hitting from the inside
    if (!outside)
    {
        n1 = RefractiveIndex;
        n2 = 1.0; 
        R  = reflectance(dot(hit.N, ray.direction()), n1, n2);
    }

    Vector3 refractVector = refract(ray, hit, n1, n2);
    
    if (refractVector != Vector3(0))
    {
        Ray refractionRay (hit.P, refractVector);
        /*refractionRay.o = hit.P;
        refractionRay.d = refractVector;*/
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
    /*refRay.o = hit.P;
    refRay.d = */

    if (outside && scene.trace(refObjHit, refRay, epsilon)) 
    {
        Vector3 reflectedColor = refObjHit.material->shade(refRay, refObjHit, scene, depth + 1);
        L += R * reflectedColor;
    }
    return L;
}
