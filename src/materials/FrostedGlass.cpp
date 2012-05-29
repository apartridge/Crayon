#include "materials/Glass.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include "sysutils/Random.h"


FrostedGlass::FrostedGlass(Vector3 d, Vector3 t) : Material(d, Vector3(0), t)
{
    this->shininess = 500;
}

Vector3 FrostedGlass::shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0);
    Vector3 l = light.getPosition() - hit.P;
    float r2 = l.length2();
    l /= sqrt(r2);
    
    HitInfo shadowHit;
    Ray shadowRay;
    shadowRay.d = l; 
    shadowRay.o = hit.P;
    bool inShadow = scene.trace(shadowHit, shadowRay, epsilon);
    bool outside = dot(-ray.d, hit.N) > 0;

    if (outside && !inShadow)
    {
        // Diffuse shading
        float costheta = dot(hit.N, l);
        L += Rd() * std::max(0.0f, costheta/r2 * light.power() / PI) * light.color();

        // Specular highlight (Phong)
        // Missed something here last, tme, what was that??
        Vector3 wr = reflect(ray, hit);
        L += Rs() * (pow(dot(wr, l), shininess) / costheta) * light.color();
    }

    return L;
}

Vector3 FrostedGlass::shadeReflectance(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0, 0, 0);

    Vector3 rndVec;
    do
    {
        rndVec = Vector3(Random::uniformRand(), Random::uniformRand(), Random::uniformRand());
    }
    while (rndVec.length2() < epsilon);
    rndVec = 0.5 * rndVec.normalized();

    Vector3 n = (hit.N + rndVec).normalized();
    HitInfo nHit = hit;
    nHit.N = n;

    float n1 = ray.mediumOfTravel.indexOfRefraction, 
          n2 = indexOfRefraction, 
          R  = reflectance(dot(nHit.N, -ray.d), n1, n2);
    
    bool outside = dot(-ray.d, nHit.N) > 0;

    // Assume going back to air when hitting from the inside
    if (!outside)
    {
        n1 = indexOfRefraction;
        n2 = 1.0; 
        R  = reflectance(dot(nHit.N, ray.d), n1, n2);
    }

    Vector3 refractVector = refract(ray, nHit, n1, n2);
    
    if (refractVector != Vector3(0))
    {
        Ray refractionRay;
        refractionRay.o = hit.P;
        refractionRay.d = refractVector;
        refractionRay.mediumOfTravel.indexOfRefraction = n2;
        HitInfo refractionHit;

        if (scene.trace(refractionHit, refractionRay, epsilon))
        {
            Vector3 refractedColor = refractionHit.material->shade(refractionRay, refractionHit, scene, depth + 1);
            L += Rt() * (1 - R) * refractedColor;
        }
    }
    
    HitInfo refObjHit;
    Ray refRay;
    refRay.o = hit.P;
    refRay.d = reflect(ray, nHit);

    if (outside && scene.trace(refObjHit, refRay, epsilon)) 
    {
        Vector3 reflectedColor = refObjHit.material->shade(refRay, refObjHit, scene, depth + 1);
        L += R * reflectedColor;
    }
    return L;
}
