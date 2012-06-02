#include "lights/Light.h"
#include "lights/PointLight.h"
#include "lights/SquareLight.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include "sysutils/Random.h"


/*
 * Point Light
 */

float Light::visibility(const Vector3& p, const Scene& scene) const
{
    Vector3 l = getPosition() - p;
    const float distance = l.length();

    HitInfo shadowHit;
    Ray shadowRay (p, l.normalized());

    return scene.trace(shadowHit, shadowRay, epsilon, distance - 5*epsilon) ? 0 : 1;
}

PhotonRay PointLight::emitPhoton() const
{
    const Vector3& o = getPosition();

    PhotonRay res;
    res.origin = o;

    if (_target != NULL)
    {
        Vector3 p = _target->samplePoint(o);
        const float r_2 = (_target->r) * (_target->r);
        const float d_2 = (_target->p - o).length2();
        res.direction = (_target->samplePoint(o) - o).normalized();
        res.power = (r_2 / float(4*d_2)) * power() * color();
    }
    else
    {
        res.direction = Random::sampleSpere();
        res.power = power();
    }

    return res;
}


/*
 * Area Light
 */

float SquareLight::visibility(const Vector3& p, const Scene& scene) const
{
    float percent = 0;
    const int shadowRays = 10;
    for (int i = 0; i < shadowRays; ++i)
    {
        // Sample random locations
        percent += Light::visibility(p, scene) / (float)shadowRays;
    }
    return percent;
}

// Sample diffuse square light like in slides (cse168_slides_pmap2.pdf)
PhotonRay SquareLight::emitPhoton() const
{
    PhotonRay res;
    res.origin = getPosition();

    // Find (u, v) for local coordinate system around n
    Vector3 n = _normal;
    Vector3 d;
    do
    {
        const float u = Random::uniformRand();
        const float v = 2*PI*Random::uniformRand();
        
        d = Vector3( cos(v)*sqrt(u), sin(v)*sqrt(u), sqrt(1 - u) ).normalized();
    }
    while (dot(n, d) < 0);

    res.direction = d;
    res.power = power();

    return res;
}


