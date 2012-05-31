#include "materials/Glass.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include "sysutils/Random.h"


FrostedGlass::FrostedGlass(Vector3 s, Vector3 t) : Material(Vector3(0), s, t)
{
    this->shininess = 500;
    indexOfRefraction = 1.5;
}

Vector3 FrostedGlass::shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0);
    float visibility = light.visibility(hit.P, scene);
    if (visibility > 0)
    {
        Vector3 l = (light.getPosition() - hit.P).normalized();
        float costheta = dot(hit.N, l);
        Vector3 wr = reflect(ray, hit);
        L += Rs() * (pow(dot(wr, l), shininess) / costheta) * light.color();
    }	
	
	if(L.x > 2 || L.y > 2 || L.z > 2)
	{
		printf("ShadeLight L is %g, %g, %g", L.x, L.y, L.z);
		throw 110;
	}
    
	return L;
}


/*
 * Microfacet reflectance, holy crap :S
 * http://www.cs.cornell.edu/~srm/publications/EGSR07-btdf.pdf
 * Use hax solution instead
 */
Vector3 FrostedGlass::shadeReflectance(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0, 0, 0);

    // Refraction
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
        //Ray refRay (hit.P, refractVector);
        //refractionRay.mediumOfTravel.indexOfRefraction = n2;
        HitInfo refractionHit;

        // Alter ray randomly
        const float e = 1000;
    
        // Reflecting direction
        Vector3 r = refractVector;

        // Find (u, v) for local coordinate system around r
        Vector3 u = r.perpendicular();
        Vector3 v = cross(r, u).normalized();

        // Generate random angles proportional to cos(theta)^n
        const float phi = 2*PI*Random::uniformRand();
        const float theta = acos(pow(Random::uniformRand(), 1/(float)(e+1)));

        Vector3 d = (cos(phi) * sin(theta) * u + 
                     sin(phi) * sin(theta) * v +
                     cos(theta) * r).normalized();

        Ray refractionRay(hit.P, d, Medium(n2));

        if (scene.trace(refractionHit, refractionRay, epsilon))
        {
            Vector3 refractedColor = refractionHit.material->shade(refractionRay, refractionHit, scene, depth + 1);
            L += 1.3*Rt() * (1 - R) * refractedColor;
        }
    }

    // Reflect
    if (outside)
    {
        // Sharpness
        const float e = 700;
    
        // Reflecting direction
        Vector3 r = Material::reflect(ray, hit);

        // Find (u, v) for local coordinate system around r
        Vector3 u = r.perpendicular();
        Vector3 v = cross(r, u).normalized();

        // Generate random angles proportional to cos(theta)^n
        const float phi = 2*PI*Random::uniformRand();
        const float theta = acos(pow(Random::uniformRand(), 1/(float)(e+1)));

        Vector3 d = (cos(phi) * sin(theta) * u + 
                     sin(phi) * sin(theta) * v +
                     cos(theta) * r).normalized();

        // Mirror direction if pointing inside surface
        if (dot(hit.N, d) < 0)
            d = Vector3(-r.x, -r.y, r.z);

        HitInfo reflectHit;
        Ray reflectRay(hit.P, d, ray.mediumOfTravel);

        if (scene.trace(reflectHit, reflectRay, epsilon))
            L += Rt() * R * reflectHit.material->shade(reflectRay, reflectHit, scene, depth + 1);
    }

	if(L.x > 2 || L.y > 2 || L.z > 2)
	{
		printf("ShadeReflectance L is %g, %g, %g", L.x, L.y, L.z);
		throw 110;
	}

    return L;
}
