#include "materials/Metal.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include "sysutils/Random.h"

Vector3 Metal::shadeLight(const Light& light, const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const
{
	float specularPower = 40;
    //Vector3 L = Vector3(0);

   /* Vector3 l = light.getPosition() - hit.P;

    // Diffuse shading based on visibility
    float visibility = light.visibility(hit.P, scene);
    float falloff = l.length2();
    l /= sqrt(falloff);
    L += Rd() * visibility * std::max(0.0f, dot(hit.N, l)/falloff * light.power() / PI) * light.color();
	*/

	Vector3 L = diffuseColor(hit) * Material::lightDiffuseVisiblity(light, hit, scene);

	if(m_specularIntensity > 0)
	{
		Vector3 reflect = Material::reflect(ray, hit);
		//Vector3 halfway = (l - ray.direction()).normalize();
		if(dot(hit.N, reflect) > 0)
		{
			L += 0.1*m_specularIntensity*pow(dot(hit.N, reflect), specularPower)*Rs()*(specularPower+1)/(2*PI);
		}
	}


    return L;
}

Vector3 Metal::shadeReflectance(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0);

   /* Ray reflectRay (hit.P, Material::reflect(ray, hit));
    reflectRay.mediumOfTravel.indexOfRefraction = ray.mediumOfTravel.indexOfRefraction;

    HitInfo reflectHit;

    if (scene.trace(reflectHit, reflectRay, epsilon))
    {
        L += Rs() * reflectHit.material->shade(reflectRay, reflectHit, scene, depth+1);
    }*/

    return L;
}
