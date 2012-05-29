#pragma once

#include <vector>

#include "lights/Light.h"
#include "renderer/glut.h"
#include "sysutils/Random.h"

class SquareLight : public Light
{
public:
	void setPosition(const Vector3& min, const Vector3& max)
	{
		m_min = min;
		m_max = max;
	}

	const Vector3 getPosition() const
	{
		return Vector3 ( m_min.x + Random::uniformRand() * (m_max.x - m_min.x),
						 m_min.y + Random::uniformRand() * (m_max.y - m_min.y),
						 m_min.z + Random::uniformRand() * (m_max.z - m_min.z) );  

		return m_min;
	}

    virtual float visibility(const Vector3& p, const Scene& scene) const;

	void renderGL()
	{

		glColor4f(1,1,0, 1);

		glBegin(GL_QUADS);
		const float padding = 0.0;
		// side min.z
		glVertex3f(m_min.x, m_min.y, m_min.z);
		glVertex3f(m_min.x, m_max.y, m_min.z);
		glVertex3f(m_max.x, m_max.y, m_min.z);
		glVertex3f(m_max.x, m_min.y, m_min.z);
		// side m_max.z
		glVertex3f(m_min.x, m_min.y, m_max.z);
		glVertex3f(m_min.x, m_max.y, m_max.z);
		glVertex3f(m_max.x, m_max.y, m_max.z);
		glVertex3f(m_max.x, m_min.y, m_max.z);
		// Side m_max.x
		glVertex3f(m_max.x, m_min.y, m_min.z);
		glVertex3f(m_max.x, m_max.y, m_min.z);
		glVertex3f(m_max.x, m_max.y, m_max.z);
		glVertex3f(m_max.x, m_min.y, m_max.z);
		// Side m_min.x
		glVertex3f(m_min.x, m_min.y, m_min.z);
		glVertex3f(m_min.x, m_max.y, m_min.z);
		glVertex3f(m_min.x, m_max.y, m_max.z);
		glVertex3f(m_min.x, m_min.y, m_max.z);
		// Side m_max.y
		glVertex3f(m_min.x, m_max.y, m_min.z);
		glVertex3f(m_max.x, m_max.y, m_min.z);
		glVertex3f(m_max.x, m_max.y, m_max.z);
		glVertex3f(m_min.x, m_max.y, m_max.z);
		// Side m_min.y
		glVertex3f(m_min.x, m_min.y, m_min.z);
		glVertex3f(m_max.x, m_min.y, m_min.z);
		glVertex3f(m_max.x, m_min.y, m_max.z);
		glVertex3f(m_min.x, m_min.y, m_max.z);
		glEnd();
	}

private:
	Vector3 m_min;
	Vector3 m_max;
};
