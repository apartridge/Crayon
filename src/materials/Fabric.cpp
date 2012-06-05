#include "materials/Fabric.h"

#include <cassert>

Fabric::Fabric(Vector3 color) : Lambert(color)
{
	m_color = color;
}

Fabric::~Fabric()
{

}

Vector3 Fabric::diffuseColor(const HitInfo& hit) const
{

	float highFreqNoise = fabs(PerlinNoise::noise(hit.P.x*50, hit.P.y*50, hit.P.z*50));
	float gFine = 0.2 + 0.8*powf(highFreqNoise - int(highFreqNoise), 1.0/10.0); // 0 - 1
	return gFine*m_color;

}
