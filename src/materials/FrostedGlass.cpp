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
    /*float v
    if (light.visibility(hit.P, scene) > 0)
        float costheta = dot(hit.N, l);
        Vector3 wr = reflect(ray, hit);
        L += Rs * (pow(dot(wr, l), shininess) / costheta) * light.color();*/
    return L;
}


/*
 * Microfacet reflectance, holy crap :S
 * http://www.cs.cornell.edu/~srm/publications/EGSR07-btdf.pdf
 */
Vector3 FrostedGlass::shadeReflectance(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0, 0, 0);

    // Sharpness
    const float e = 500;
    
    // Reflecting direction
    Vector3 r = Material::reflect(ray, hit);

    // Find (u, v) for local coordinate system around r
    Vector3 u = r.perpendicular();
    Vector3 v = cross(r, u).normalized();

    // Generate random angles proportional to cos(theta)^n
    const float phi = 2*PI*Random::uniformRand();
    const float theta = acos(pow(Random::uniformRand(), 1/(float)e));

    Vector3 d = (cos(phi) * sin(theta) * u + 
                 sin(phi) * sin(theta) * v +
                 cos(theta) * r).normalized();

    // Mirror direction if pointing inside surface
    if (dot(hit.N, d) < 0)
        d = Vector3(-r.x, -r.y, r.z);

    HitInfo reflectHit;
    Ray reflectRay(hit.P, d, ray.mediumOfTravel);

    if (scene.trace(reflectHit, reflectRay, epsilon))
        L += reflectHit.material->shade(reflectRay, reflectHit, scene, depth + 1);

    return L;
}
