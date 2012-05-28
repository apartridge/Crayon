#include "materials/Mirror.h"
#include "geometry/Vector3.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"

Mirror::Mirror() : Material(Vector3(0), Vector3(1), Vector3(0))
{
    shininess = 500;
}


Vector3 Mirror::shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0);
    Vector3 l = (light.getPosition() - hit.P).normalized();
    
    HitInfo shadowHit;
    Ray shadowRay;
    shadowRay.d = l; 
    shadowRay.o = hit.P;
    bool inShadow = scene.trace(shadowHit, shadowRay, epsilon);

    // Specular highlight (Phong)
    if (!inShadow)
    {
        Vector3 wr = reflect(ray, hit);
        L += Rs() * (pow(dot(wr, l), shininess) / dot(hit.N, l)) * light.color();
    }

    return L;
}



Vector3 Mirror::shadeReflectance(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0);

    Ray reflectRay;
    reflectRay.o = hit.P;
    reflectRay.d = Material::reflect(ray, hit);
    reflectRay.mediumOfTravel.indexOfRefraction = ray.mediumOfTravel.indexOfRefraction;

    HitInfo reflectHit;

    if (scene.trace(reflectHit, reflectRay, epsilon))
    {
        L += Rs() * reflectHit.material->shade(reflectRay, reflectHit, scene, depth+1);
    }

    return L;
}
