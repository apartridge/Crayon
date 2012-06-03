#include "materials/Texture.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include "sysutils/Random.h"

Texture::~Texture()
{
}

Vector3 Texture::diffuseColor(const HitInfo& hit) const
{
	return (m_diffuseTexture->sampleImage(hit.textureU, hit.textureV));
}

/*
Vector3 Texture::shadeLight(const Light& light, const Ray& ray, 
        const HitInfo& hit, const Scene& scene, const int depth) const
{
	return diffuseColor(hit) * Material::lightDiffuseVisiblity(light,  hit, scene);
}*/
/*
Vector3 Texture::shadeGlobalIllumination(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
    return Lambert::shadeGlobalIllumination(ray, hit, scene, depth);
}
*/