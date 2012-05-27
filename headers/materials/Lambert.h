#ifndef CRAYON_LAMBERT_H_INCLUDED
#define CRAYON_LAMBERT_H_INCLUDED

#include "materials/Material.h"

class Lambert : public Material
{
public:
    Lambert(const Vector3 & kd = Vector3(0.5),
            const Vector3 & ka = Vector3(0));
    virtual ~Lambert();

    virtual void preCalc() {}

    virtual Vector3 shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;
    virtual Vector3 shadeGlobalIllumination(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;

protected:
    Vector3 m_kd;
};

/*
class Lambert : public NMaterial
{
public:
    Lambert(const Vector3 & kd = Vector3(1),
			float opacity = 1,
            const Vector3 & ka = Vector3(0));
    virtual ~Lambert();

    //const Vector3 & kd() const {return m_kd;}
    const Vector3 & ka() const {return m_ka;}

    //void setKd(const Vector3 & kd) {m_kd = kd;}
    void setKa(const Vector3 & ka) {m_ka = ka;}

    virtual void preCalc() {}
    
    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene, float alpha, const int depth) const;
protected:
    //Vector3 m_kd;
    Vector3 m_ka;
};*/

#endif // CRAYON_LAMBERT_H_INCLUDED
