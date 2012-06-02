#ifndef CRAYON_POINTLIGHT_H_INCLUDED
#define CRAYON_POINTLIGHT_H_INCLUDED

#include <vector>

#include "Light.h"
#include "renderer/glut.h"

class PointLight : public Light
{
public:
	void setPosition(const Vector3& v) 
    {
        m_position = v;
    }

	const Vector3 getPosition() const
	{
		return m_position;
	}

    virtual PhotonRay emitPhoton() const;

	void renderGL()
	{
		glColor4f(1,1,0, 1);
		glTranslatef(getPosition().x, getPosition().y, getPosition().z);
		glutSolidSphere(0.5, 20, 20);
		glTranslatef(-getPosition().x, -getPosition().y, -getPosition().z);
	}

protected:
    Vector3 m_position;
};

#endif
