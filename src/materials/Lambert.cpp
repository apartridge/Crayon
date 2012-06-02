#include "materials/Lambert.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include "sysutils/Random.h"

Lambert::Lambert(const Vector3& d) :
    Material(d) /*, m_kd(d)*/
{
}

Lambert::~Lambert()
{
}

Vector3 Lambert::shadeLight(const Light& light, const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const
{
    /*Vector3 L = Vector3(0);
    Vector3 l = light.getPosition() - hit.P;

    // Diffuse shading based on visibility
    float visibility = light.visibility(hit.P, scene);
    float falloff = l.length2();
    l /= sqrt(falloff);
    L += Rd() * visibility * std::max(0.0f, dot(hit.N, l)/falloff * light.power() / PI) * light.color();*/

	return diffuseColor(hit)*lightDiffuseVisiblity(light, hit, scene);
}

