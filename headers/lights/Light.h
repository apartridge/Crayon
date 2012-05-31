#pragma once

#include <vector>
#include "geometry/Vector3.h"

class Scene;

class Light
{
public:
    void setColor(const Vector3& v)
    {
        m_color = v;
    }

    void setPower(float f) {
        m_wattage = f;
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
    virtual Vector3 emitPhoton() const = 0;

	virtual void renderGL() = 0;

protected:
    Vector3 m_color;
    float m_wattage;
};

typedef std::vector<Light*> Lights;

