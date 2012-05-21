#include "../../headers/materials/Lambert.h"
#include "../../headers/geometry/Ray.h"
#include "../../headers/geometry/Scene.h"
#include "../../headers/sysutils/Random.h"

Lambert::Lambert(const Vector3 & d, const Vector3 & a) :
    Material(a), m_kd(d)
{
}

Lambert::~Lambert()
{
}

Vector3 Lambert::shadeLight(const Light& light, const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const
{
    const bool SHADOWS = false;

    Vector3 L(0);
    Vector3 l = light.getPosition() - hit.P;

    // Find visibility of light
    //float visibility = light.visibility(hit.P, scene);
    
    bool inShadow = false;
    if (SHADOWS)
    {
        HitInfo shadowHit;
        Ray shadowRay;
        shadowRay.d = l.normalized(); 
        shadowRay.o = hit.P;
        inShadow = scene.trace(shadowHit, shadowRay, epsilon, l.length());
    }

    // Diffuse component from the light source
    if (!inShadow) 
    {
        float falloff = l.length2();
        l /= sqrt(falloff);

        float nDotL = dot(hit.N, l);
        L += m_kd * std::max(0.0f, nDotL/falloff * light.power() / PI) * light.color();
    }

    return L;
}

Vector3 Lambert::shadeGlobalIllumination(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0);

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
    Ray pathRay;
    pathRay.d = d; 
    pathRay.o = hit.P;
    if (scene.trace(pathHit, pathRay, epsilon))
    {
        if (pathHit.material != NULL)
            L += m_kd * (1/PI) * pathHit.material->shade(pathRay, pathHit, scene, depth + 1);
    }
    return L;
}
