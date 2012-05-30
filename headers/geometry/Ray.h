#ifndef CRAYON_RAY_H_INCLUDED
#define CRAYON_RAY_H_INCLUDED

#include "Vector3.h"
#include "../geometry/Medium.h"
#include "../renderer/RenderingStats.h"
#include "../materials/Material.h"

#if RENDERING_STATS
extern RenderingStats* rendering_statistics;
#endif

class Ray
{
private:
    Vector3 m_origin,      //!< Origin of ray
            m_direction;      //!< Direction of ray
	Vector3 m_dInverse;

public:

	Medium mediumOfTravel;

	const Vector3& origin() const
	{
		return m_origin;
	}

	const Vector3& direction() const
	{
		return m_direction;
	}

	const Vector3& dInverse() const
	{
		return m_dInverse;
	}

    Ray() : m_origin(), m_direction(Vector3(0.0f,0.0f,1.0f)), mediumOfTravel(Medium(1.0f))
    {
		addRayToStatistics();
		m_dInverse = m_direction;
    }

	Ray(const Vector3& o, const Vector3& d) : m_origin(o), m_direction(d)
    {
		addRayToStatistics();
		m_dInverse = Vector3(1/d.x, 1/d.y, 1/d.z);
    }

	Ray(const Vector3& o, const Vector3& d, const Medium & m) : m_origin(o), m_direction(d), mediumOfTravel(m)
    {
		addRayToStatistics();
		m_dInverse = Vector3(1/d.x, 1/d.y, 1/d.z);
    }

	void addRayToStatistics()
	{
		
#if RENDERING_STATS
		rendering_statistics->lock();
		rendering_statistics->rays++;
		rendering_statistics->unlock();
#endif
	}
};


//! Contains information about a ray hit with a surface.
/*!
    HitInfos are used by object intersection routines. They are useful in
    order to return more than just the hit distance.
*/
class HitInfo
{
public:
    float t;                            //!< The hit distance
    Vector3 P;                          //!< The hit point
    Vector3 N;                          //!< Shading normal vector
    const Material* material;           //!< Material of the intersected object

	float textureU;
	float textureV;

    //! Default constructor.
    explicit HitInfo(float t = 0.0f,
                     const Vector3& P = Vector3(),
                     const Vector3& N = Vector3(0.0f, 1.0f, 0.0f)) :
        t(t), P(P), N(N), material (0), textureU(0), textureV(0)
    {
        // empty
		//intersections = 0;
    }
};

#endif // CRAYON_RAY_H_INCLUDED
