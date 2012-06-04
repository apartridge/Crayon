#pragma once

#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include "geometry/Vector3.h"
#include "materials/Lambert.h"
#include "materials/Worley.h"
#include "materials/Perlin.h"

class Wood : public Lambert
{
public:
	Wood(Vector3 baseColor, Vector3 highColor, float scale, Vector3 stripesDirection = Vector3(1,0,0), float stripesScale = 3);
    virtual ~Wood();
    virtual void preCalc() {};
    
    virtual Vector3 shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;
    //virtual Vector3 shadeGlobalIllumination(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;
	virtual Vector3 diffuseColor(const HitInfo& hit) const;
	void setGlossiness(int power, float factor, Vector3 glossColor = Vector3(1))
	{
		m_glossPower = power;
		m_glossFactor = factor;
		m_glossColor = glossColor;
	}// Blinn Phong Specular

protected:
    //Vector3 proceduralColor(const HitInfo& hit) const; 

private:
    Vector3 WoodTexture(const HitInfo& hit) const; // Where is this? oO
	Vector3 m_baseColor;
	Vector3 m_highColor;
	float m_scale;
	float m_perlinScale;

	float m_glossPower;
	float m_glossFactor;
	Vector3 m_glossColor;


	Vector3 m_stripesDirection;
	float m_stripesScale;

};
