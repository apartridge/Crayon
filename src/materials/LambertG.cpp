#pragma once

#include "../../headers/materials/Shiny.h"
#include "../../headers/geometry/Ray.h"
#include "../../headers/geometry/Scene.h"
#include "../../headers/materials/LambertG.h"
#include "../../headers/sysutils/Random.h"
#include "../../headers/lights/Light.h"
#include <iostream>
#include <cassert>

LambertG::LambertG(const Vector3 & kd) :
			Material(kd, Medium(1), 1)
{

}

LambertG::~LambertG()
{
}





Vector3 LambertG::shade(const Ray& ray, const HitInfo& hit, const Scene& scene, float alpha, const int depth) const
{
	if(depth > 5 )
	{
		return Vector3(0,0,0);
	}
	
	if(alpha < 0.0001)
	{
		return Vector3(0,0,0);
	}
	
	float Rd = getOpacity();

    Vector3 L(0); // = alpha; // * m_ka;
    
    const Vector3 viewDir = -ray.d; // d is a unit vector
    
    const Lights *lightlist = scene.lights();

    float rayDotN = dot(ray.d, hit.N);

	
	float n1 = ray.mediumOfTravel.indexOfRefraction,
	      n2 = this->getMedium().indexOfRefraction;

	if (rayDotN > 0) // Hit from inside
	{
		n1 = this->getMedium().indexOfRefraction;
		n2 = 1.0;
	}


    /*
	// loop over all of the lights. Do some number of samples towards the light
	*/

	Lights::const_iterator lightIter;
	for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
	{

		Vector3 LL = Vector3(0.0f, 0.0f, 0.0f);

		Light* pLight = *lightIter;
   
		int lightSamples = 20/(depth+1);

		for(int i = 0; i < lightSamples; i++)
		{

			Vector3 l = pLight->getPosition() - hit.P; // Random Position Towards Light

			// the inverse-squared falloff
			float falloff = l.length2();
        		
			// In shadow?

			Ray towards_light (hit.P, l, n1);
			HitInfo towards_light_hit;

			float tmin = 0.001;
			if(scene.trace(towards_light_hit, towards_light, tmin))
			{
				Vector3 ray = hit.P + towards_light_hit.t*l;

				if(ray.length2() < falloff)
				{
					continue;
				}
			}

			float falloffsq = sqrt(falloff);
			l /= falloffsq;

			/*
				Diffuse
			*/

			float nDotL = dot(hit.N, l);

			LL += alpha * std::max(0.0f, nDotL/falloff * pLight->power() / PI) * pLight->color() * this->getDiffuseColor() * Rd;

		}

		// Divide by the number of lightsamples

		L += LL / lightSamples;
	}




	/*
	// Random Lambert Reflection, Path Tracing
	*/

	Vector3 normal = hit.N;
	Vector3 uaxis = normal.perpendicular();
	Vector3 vaxis = cross(hit.N, normal.perpendicular());

	float azimuth = Random::uniformRand()*2*PI;
	float polar = asin(  sqrt(Random::uniformRand() ));

	Vector3 lambertReflection = cos(azimuth)*sin(polar)*uaxis + sin(azimuth)*sin(polar)*vaxis + cos(polar)*normal ;

	lambertReflection.normalize();

	Ray lambert_reflection_ray (hit.P, lambertReflection, ray.mediumOfTravel);
	HitInfo lambert_reflection_hit;

	if(scene.trace(lambert_reflection_hit, lambert_reflection_ray, 0.001))
	{
		L += Rd*this->getDiffuseColor()*lambert_reflection_hit.material->shade(lambert_reflection_ray, lambert_reflection_hit, scene, alpha, depth+1);
	}






	/*
	if(!(  ISZERO(dot(normal, uaxis)) && ISZERO(dot(normal, vaxis)) && ISZERO(dot(uaxis, vaxis)))  ){

		printf("Dot between normal and uaxis is %g\n ", dot(normal, uaxis));
		printf("Dot between uaxis and vaxis is %g\n ", dot(uaxis, vaxis));
		printf("Dot between normal and vaxis is %g\n ", dot(normal, vaxis));

		std::cout << normal << std::endl;
		std::cout << uaxis << std::endl;
		std::cout << vaxis << std::endl;


	}
	*/

	
    return L;
}