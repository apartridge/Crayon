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
	float noise = PerlinNoise::noise(hit.P.x*10, hit.P.y*10, hit.P.z*10);
	if(noise < 0)
	{
		noise = -noise;
	}
	float g = noise - int(noise); // 0 - 1


	float highFreqNoise = PerlinNoise::noise(hit.P.x*50, hit.P.y*50, hit.P.z*50);
	if(highFreqNoise < 0)
	{
		highFreqNoise = -highFreqNoise;
	}
	float gFine = highFreqNoise - int(highFreqNoise); // 0 - 1
	gFine = (gFine > 0.5) ? 0 : 1;

	return gFine*m_color*(0.7+0.3*g);

}
