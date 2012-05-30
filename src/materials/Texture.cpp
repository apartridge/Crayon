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
    Vector3 l = light.getPosition() - hit.P;

    // Diffuse shading based on visibility

	Vector3 normal = hit.N; // world space
	/*if(m_hasNormalMap)
	{
		Vector3 objSpaceNormal = m_normalMapTexture->sampleImage(hit.textureU, hit.textureV);
		Vector3 worldSpaceNormal = m_objectToWorld * objSpaceNormal;
		normal = worldSpaceNormal.normalized();
	}*/

	//return normal;

    float visibility = light.visibility(hit.P, scene);
    float falloff = l.length2();
    l /= sqrt(falloff);

    return L * visibility * std::max(0.0f, dot(normal, l)/falloff * light.power() / PI) * light.color();
}
