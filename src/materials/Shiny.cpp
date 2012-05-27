#include "materials/Shiny.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include <iostream>

Shiny::Shiny(const Vector3 & kd, const Medium & medium, float opacity, const Vector3 & ka, float reflectiveness) :
			Material(kd, medium, opacity), m_ka(ka), m_reflectiveness(reflectiveness)
{

}

Shiny::~Shiny()
{
}

Vector3 Shiny::shade(const Ray& ray, const HitInfo& hit, const Scene& scene, float alpha, const int depth) const
{
	if(depth > 10 )
	{
		return Vector3(0,0,0);
	}
	
	if(alpha < 0.0001)
	{
		return Vector3(0,0,0);
	}
	
	float Rd = getOpacity();

    Vector3 L = alpha * m_ka;
    
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
   
		int lightSamples = 50/(depth+1);

		for(int i = 0; i < lightSamples; i++)
		{

			Vector3 LL2;

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

			LL2 += std::max(0.0f, nDotL/falloff * pLight->power() / PI) * pLight->color() * this->getDiffuseColor() * Rd;

			/*
				Specular
			*/

			Vector3 specularHighlight = (-l) + 2*hit.N * nDotL; // OUTWARDS
			specularHighlight.normalize();
			float specularBase = dot(specularHighlight, viewDir);
			if(specularBase < 0)
			{
				specularBase = 0;
			}

			float specular = 0.5*pow(specularBase, 300);

			LL2 += specular;

			LL +=  alpha * LL2;

		}

		// Divide by the number of lightsamples

		L += LL / lightSamples;
	}











	float flipNormal = rayDotN > 0 ? -1 : 1;

	float n = n1/n2;
	float cosi = -flipNormal*rayDotN;
	float sin2t = n*n*(1 - cosi*cosi);

	if(sin2t <= 1)
	{

		float cost = sqrt(1.0 - sin2t);

		// Fresnel Reflection/Refratcion Factors


		float fr_rs = (n1*cosi - n2*cost) / (n1*cosi + n2*cost);
		float fr_rp = (n1*cost - n2*cosi) / (n1*cost + n2*cosi);
		float fr_r = (fr_rs*fr_rs + fr_rp*fr_rp) / 2 ;

		float Rr = (1-Rd)*fr_r + 0*m_reflectiveness; //  Reflected
		float Rt = (1-Rd)*(1-fr_r); // Refracted/Transmitted

		//return Vector3(Rr, 0, 0);

		/*
		// Refraction
		*/

		if(Rt > 0)
		{
			if(sin2t >= 0)
			{
				Vector3 refraction;
				refraction = n*ray.d + (n*cosi - cost)*hit.N*flipNormal;
				Ray refracted_ray (hit.P, refraction, n2);
				HitInfo refracted_hit;

				if(scene.trace(refracted_hit, refracted_ray, 0.001))
				{
					L += refracted_hit.material->shade(refracted_ray, refracted_hit, scene, Rt*alpha, depth+1);
				}

			}

		}
	
		/*
			Reflection
		*/

		if(Rr > 0)
		{
			Vector3 reflection = ray.d - 2*hit.N * rayDotN;
			reflection.normalize();
			Ray reflected_ray (hit.P, reflection, n1);
			HitInfo reflected_hit;

			if(scene.trace(reflected_hit, reflected_ray, 0.001))
			{
				L += reflected_hit.material->shade(reflected_ray, reflected_hit, scene, Rr*alpha, depth+1);
			}
		}
	
	}

    return L;
}