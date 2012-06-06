#include "materials/LambertTwoColor.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include "sysutils/Random.h"


Vector3 LambertTwoColor::diffuseColor(const HitInfo& hit) const
{
    Vector3 color;
    float sinf = sin((hit.P.x + hit.P.z)*m_scale) + m_wideness;

	if(sinf > 0)
	{
		color = m_firstColor;
	}
	else
	{
		color = m_secondColor;
	}
    return color;
}