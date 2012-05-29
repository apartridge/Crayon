#pragma once

#include "materials/Lambert.h"
#include "materials/TextureImage.h"

class Texture : public Lambert
{
public:
    Texture();
    virtual ~Texture();

    virtual void preCalc() {}

	void setDiffuse(TextureImage& image);
	void setNormalMap(TextureImage& image);

    virtual Vector3 shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;
    //virtual Vector3 shadeGlobalIllumination(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;

protected:
    Vector3 m_kd;
};
