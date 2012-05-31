#pragma once

#include "materials/Lambert.h"
#include "materials/TextureImage.h"
#include "geometry/Matrix4x4.h"


class Texture : public Lambert
{
public:
    Texture() : Lambert(Vector3(0.5))
	{
		m_hasNormalMap = false;
	}

    virtual ~Texture();

    virtual void preCalc() {}

	void setDiffuse(TextureImage* image)
	{
		this->m_diffuseTexture = image;
	}
	void setNormalMap(TextureImage* image, Matrix4x4 objectToWorld, float normalScale)
	{
		this->m_normalMapTexture = image;
		this->m_normalScale = normalScale;
		this->m_objectToWorld = objectToWorld;
		m_hasNormalMap = true;
	}

    virtual Vector3 shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;
    virtual Vector3 shadeGlobalIllumination(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;

protected:
    TextureImage* m_diffuseTexture;
	
	bool m_hasNormalMap;
	TextureImage* m_normalMapTexture;
	float m_normalScale;

	Matrix4x4 m_objectToWorld;

};
