#pragma once

#include <cstdlib>
#include <ctime>

#include "renderer/Miro.h"
#include "geometry/Vector3.h"

class Random
{
public:
	static void seed()
	{
		srand(time(0));
	}

	static float uniformRand()
	{
		return rand() / double(RAND_MAX + 1);
	}

    // Return random direction equidistributed in all directions
    static Vector3 sampleSpere()
    {
        Vector3 dir;
        do {
            dir.x = 2*Random::uniformRand() - 1;
            dir.y = 2*Random::uniformRand() - 1;
            dir.z = 2*Random::uniformRand() - 1;
        } while (dot(dir, dir) > 1);
        dir.normalize();
        return dir;
    }

    // Sample hemisphere with cosine distribution around some normal. 
    static Vector3 sampleHemisphere(const Vector3& n, float a = 0)
    {
        // Find (u, v) for local coordinate system around n
        Vector3 u = n.perpendicular();
        Vector3 v = cross(n, u).normalized();

        // Generate random angles proportional to cos(theta)^n
        const float phi = 2*PI*Random::uniformRand();
        const float theta = (a == 0) ? asin(sqrt(Random::uniformRand())) // Normal cosine
                                     : acos(pow(Random::uniformRand(), 1/(float)(a+1))); // Blurring

        return (cos(phi) * sin(theta) * u + 
                sin(phi) * sin(theta) * v +
                cos(theta) * n).normalized();
    }
};