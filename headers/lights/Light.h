#pragma once

#include <vector>
#include "geometry/Vector3.h"
#include "sysutils/Random.h"

class Scene;
class LightTarget;

struct PhotonRay
{
    Vector3 origin;
    Vector3 direction;
    Vector3 power;
};

class Light
{
public:
    Light() : _target(NULL)
    {
    }

    void setColor(const Vector3& v)
    {
        m_color = v;
    }

    void setPower(float f) 
    {
        m_wattage = f;
    }

    void setTarget(LightTarget* target)
    {
        _target = target;
    }
    
    float power() const 
    {
        return m_wattage;
    }
    
    const Vector3 & color() const 
    {
        return m_color;
    }
    
    virtual void preCalc() {}

    // Returns a random position on light surface, t
	virtual const Vector3 getPosition() const = 0; 

    // Approximate visibility from a given point
    virtual float visibility(const Vector3& p, const Scene& scene) const;

    // Generate outgoing direction to trace photon path
    virtual PhotonRay emitPhoton() const = 0;

	virtual void renderGL() = 0;

protected:
    Vector3 m_color;
    float m_wattage;
    LightTarget* _target;
};

// Handles selective photon tracing towards some bounding sphere
class LightTarget
{
public:
    LightTarget(Vector3 point, float radius) : p(point), r(radius)
    {
    }

    // Find a point on target sphere (disc) seen from a given point
    Vector3 samplePoint(const Vector3& o)
    {
        Vector3 n = (o - p).normalized();
	    Vector3 u = n.perpendicular();
        Vector3 v = cross(n, u).normalized();
        Vector3 dir;
        
        // Use simple rejection saampling on disc
        // TODO: Be more clever
        do
        {
            float x = 2*Random::uniformRand() - 1;
            float y = 2*Random::uniformRand() - 1;
            dir = u*x + v*y;
        }
        while (dot(dir, dir) > 1);

        return p + dir*r;
    }

    Vector3 p;
    float r;
};


typedef std::vector<Light*> Lights;
