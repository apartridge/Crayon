#include "materials/Texture.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include "sysutils/Random.h"

Texture::~Texture()
{
}

Vector3 Texture::shadeLight(const Light& light, const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L = m_diffuseTexture->sampleImage(hit.textureU, hit.textureV);
    return L * Lambert::shadeLight(light, ray, hit, scene, depth);
}

Vector3 Texture::shadeGlobalIllumination(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    Vector3 L = m_diffuseTexture->sampleImage(hit.textureU, hit.textureV);
    return L * Lambert::shadeGlobalIllumination(ray, hit, scene, depth);
}
