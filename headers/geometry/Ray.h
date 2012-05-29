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
public:
    Vector3 o,      //!< Origin of ray
            d;      //!< Direction of ray
	Vector3 dInverse;


	Medium mediumOfTravel;

    Ray() : o(), d(Vector3(0.0f,0.0f,1.0f)), mediumOfTravel(Medium(1.0f))
    {
		addRayToStatistics();
		dInverse = d;
    }

	Ray(const Vector3& o, const Vector3& d, const Medium & m) : o(o), d(d), mediumOfTravel(m)
    {
		addRayToStatistics();
		dInverse = Vector3(1/d.x, 1/d.y, 1/d.z);
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
