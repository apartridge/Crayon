#include "lights/Light.h"
#include "lights/PointLight.h"
#include "lights/SquareLight.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"


float Light::visibility(const Vector3& p, const Scene& scene) const
{
    Vector3 l = getPosition() - p;
    const float distance = l.length();

    HitInfo shadowHit;
    Ray shadowRay (p, l.normalized()) ;

    /*shadowRay.d = l.normalized(); 
    shadowRay.o = p;
	*/

    return scene.trace(shadowHit, shadowRay, epsilon, distance - 5*epsilon) ? 0 : 1;
}


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

