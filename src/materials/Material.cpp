#include "../../headers/materials/Material.h"
#include "../../headers/geometry/Ray.h"
#include "../../headers/geometry/Scene.h"


Material::Material(const Vector3& a) : ambient(a)
{
}

Material::~Material()
{
}

Vector3 Material::shade(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    const float GAIN = 1.0f;

    Vector3 L(0);
    if (depth > Material::RecursionLimit)
        return L;
    
    const Lights *lightlist = scene.lights();
    Lights::const_iterator lightIter;
    
    for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
    {
        Light* pLight = *lightIter;

        L += shadeLight(*pLight, ray, hit, scene, depth);
    }

    L += shadeGlobalIllumination(ray, hit, scene, depth);

    L += shadeReflectance(ray, hit, scene, depth);

    //L += ambient; // probably not ....

    if (depth == 0)
        return Vector3(pow(L[0], (1/2.2f)),
                       pow(L[1], (1/2.2f)),
                       pow(L[2], (1/2.2f)));
    else
        return L;
}

Vector3 Material::reflect(const Ray& ray, const HitInfo& hit)
{
    return (ray.d - 2*dot(ray.d, hit.N)*hit.N).normalized();
}

Vector3 Material::refract(const Ray& ray, const HitInfo& hit, float n1, float n2)
{
    Vector3 n = hit.N;
    if (dot(ray.d, n) > 0) // Hit from inside
        n *= -1;
    const float n1n2 = (n1/n2);
    const float cosi = dot(-ray.d, n);
    const float sin2t = n1n2*n1n2*(1 - cosi*cosi);
    if (sin2t > 1.0) 
        return Vector3(0); // No refraction
    return (n1n2*ray.d + (n1n2*cosi - sqrt(1.0 - sin2t))*n).normalized();
}

/* Use simplified Fresnel equation 
 * Amount of incoming light that is reflected.
 * Transmitted light is T = 1 - R
 * Source: http://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
 */
float Material::reflectance(float costheta, float n1, float n2)
{
    float r0 = (n1 - n2) / (n1 + n2);
    r0 *= r0;
    if (n1 > n2)
    {
        const float n = n1 / n2;
        const float sint2 = n * n * (1 - costheta * costheta);
        if (sint2 > 1)
            return 1; // Everything is reflected
        costheta = sqrt(1 - sint2);
    }
    const float x = 1 - costheta;
    return r0 + (1 - r0) * x * x * x * x * x;
}
