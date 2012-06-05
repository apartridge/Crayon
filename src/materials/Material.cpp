#include "materials/Material.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"


Material::Material(const Vector3& d, const Vector3& s, const Vector3& t) : /* ambient(d),*/ rd(d), rs(s), rt(t)
{
    indexOfRefraction = 1.0;
    shininess = 0.0;
}

Material::~Material()
{
}

Vector3 Material::shade(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
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

    return L;
}


/*
Global Illumination
*/

Vector3 Material::shadeGlobalIllumination(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0);

    if (true)
    {
        Vector3 irradiance;
        scene.photonMap()->irradiance_estimate(&irradiance[0], &hit.P[0], &hit.N[0], 0.1, 80);

        L += diffuseColor(hit) * irradiance; // ??
    }
    else
    {
		// Find (u, v) for local coordinate system
		Vector3 n = hit.N;
		Vector3 u = n.perpendicular();
		Vector3 v = cross(n, u);

		// Generate random angles proportional to cos(theta)
		const float phi = 2*PI*Random::uniformRand();
		const float theta = asin(sqrt(Random::uniformRand()));

		Vector3 d = (cos(phi) * sin(theta) * u + 
					 sin(phi) * sin(theta) * v +
					 cos(theta) * n).normalized();

		// Trace randomly generated reflected ray
		HitInfo pathHit;
		Ray pathRay (hit.P, d);

		if (scene.trace(pathHit, pathRay, epsilon))
		{
			if (pathHit.material != NULL)
				L += diffuseColor(hit) * (1/PI) * pathHit.material->shade(pathRay, pathHit, scene, depth + 1);
		}
    }


    return L;
}













Vector3 Material::lightDiffuseVisiblity(const Light& light, const HitInfo& hit, const Scene& scene)
{
	Vector3 L = Vector3(0);
    Vector3 l = light.getPosition() - hit.P;
    float visibility = light.visibility(hit.P, scene);
    float falloff = l.length2();
    l /= sqrt(falloff);
    return visibility * std::max(0.0f, dot(hit.N, l)/falloff * light.power() / PI) * light.color();
}

Vector3 Material::reflect(const Ray& ray, const HitInfo& hit)
{
    return (ray.direction() - 2*dot(ray.direction(), hit.N)*hit.N).normalized();
}

Ray Material::refractRay(const Ray& ray, const HitInfo& hit) const
{
    float n1 = ray.mediumOfTravel.indexOfRefraction;
    float n2 = this->indexOfRefraction;

	Vector3 direction;

    Vector3 n = hit.N;
    if (dot(ray.direction(), n) > 0) // Hit from inside
        n *= -1;
    const float n1n2 = (n1/n2);
    const float cosi = dot(-ray.direction(), n);
    const float sin2t = n1n2*n1n2*(1 - cosi*cosi);

    if (sin2t > 1.0) 
        direction = Vector3(0.0); // No refraction
    else
        direction = (n1n2*ray.direction() + (n1n2*cosi - sqrt(1.0 - sin2t))*n).normalized();




	Ray refractRay (hit.P, direction) ;
    /*refractRay.o = hit.P;
    refractRay.mediumOfTravel = n2;*/


    return refractRay;
}

Vector3 Material::refract(const Ray& ray, const HitInfo& hit, float n1, float n2)
{
    Vector3 n = hit.N;
    if (dot(ray.direction(), n) > 0) // Hit from inside
        n *= -1;
    const float n1n2 = (n1/n2);
    const float cosi = dot(-ray.direction(), n);
    const float sin2t = n1n2*n1n2*(1 - cosi*cosi);
    if (sin2t > 1.0) 
        return Vector3(0); // No refraction
    return (n1n2*ray.direction() + (n1n2*cosi - sqrt(1.0 - sin2t))*n).normalized();
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
