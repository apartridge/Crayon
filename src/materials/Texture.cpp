#include "materials/Texture.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include "sysutils/Random.h"

Texture::Texture() :
    Lambert(Vector3(1, 0, 0), Vector3(1, 0, 0))
{
}

Texture::~Texture()
{
}

Vector3 Texture::shadeLight(const Light& light, const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L(0);
    Vector3 l = light.getPosition() - hit.P;

    // Diffuse shading based on visibility
    float visibility = light.visibility(hit.P, scene);
    float falloff = l.length2();
    l /= sqrt(falloff);
    L += Rd() * visibility * std::max(0.0f, dot(hit.N, l)/falloff * light.power() / PI) * light.color();

	return Vector3(hit.textureU/2.0, hit.textureV/2.0, 0);

    return L;
}
