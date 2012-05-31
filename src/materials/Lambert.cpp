#include "materials/Lambert.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include "sysutils/Random.h"

Lambert::Lambert(const Vector3& d, const Vector3& a) :
    Material(d), m_kd(d)
{
}

Lambert::~Lambert()
{
}

Vector3 Lambert::shadeLight(const Light& light, const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0);
    Vector3 l = light.getPosition() - hit.P;

    // Diffuse shading based on visibility
    float visibility = light.visibility(hit.P, scene);
    float falloff = l.length2();
    l /= sqrt(falloff);
    L += Rd() * visibility * std::max(0.0f, dot(hit.N, l)/falloff * light.power() / PI) * light.color();

    return L;
}

Vector3 Lambert::shadeGlobalIllumination(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0);

    if (true)
    {
        Vector3 irradiance;
        scene.photonMap()->irradiance_estimate(&irradiance[0], &hit.P[0], &hit.N[0], 0.5, 10);

        L += Rd() * irradiance; // ??
    }
    else
    {
		// Find (u, v) for local coordinate system
		Vector3 n = hit.N;
		Vector3 u = n.perpendicular();
		Vector3 v = cross(n, u).normalized();

		// Generate random angles proportional to cos(theta)
		const float phi = 2*PI*Random::uniformRand();
		const float theta = asin(sqrt(Random::uniformRand()));

		Vector3 d = (cos(phi) * sin(theta) * u + 
					 sin(phi) * sin(theta) * v +
					 cos(theta) * n).normalized();

		// Trace randomly generated reflected ray
		HitInfo pathHit;
		Ray pathRay (hit.P, d);
		/*pathRay.d = d; 
		pathRay.o = hit.P;*/
		if (scene.trace(pathHit, pathRay, epsilon))
		{
			if (pathHit.material != NULL)
				L += Rd() * (1/PI) * pathHit.material->shade(pathRay, pathHit, scene, depth + 1);
		}
    }


    return L;
}